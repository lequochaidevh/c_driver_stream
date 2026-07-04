#pragma once
#include "sfd_policy.h"
#include "sfd_linux_api.h"
#include "sfd_session_guard.h"

// --- COMPILE-TIME TAG TO FORCE LINUX SYSTEM CALL PATH ---
// Empty struct used as a compile-time signal for the constructor
struct LinuxArgs {};

template <typename Policy = FilePolicy>
class UniqueFileDescription {
private:
    int _fd = -1;

public:
    UniqueFileDescription() = default;

    // Traditional raw fd constructor
    explicit UniqueFileDescription(int fd) : _fd(fd) {}
    
    // Variadic constructor triggered ONLY when LinuxArgs tag is passed first
    template <typename... Args>
    explicit UniqueFileDescription(LinuxArgs, Args&&... args) {
        _fd = Policy::open(std::forward<Args>(args)...);
    }

    ~UniqueFileDescription() { 
        release(); 
    }

    // Copying is prohibited to guarantee unique ownership
    UniqueFileDescription(const UniqueFileDescription&) = delete;
    UniqueFileDescription& operator=(const UniqueFileDescription&) = delete;

    // Move semantics transfer ownership safely on the stack
    UniqueFileDescription(UniqueFileDescription&& other) noexcept : _fd(other._fd) {
        other._fd = -1;
    }

    UniqueFileDescription& operator=(UniqueFileDescription&& other) noexcept {
        if (this != &other) {
            release();
            _fd = other._fd;
            other._fd = -1;
        }
        return *this;
    }

    int get() const { return _fd; }
    explicit operator bool() const { return _fd >= 0; }
    int operator*() const { return _fd; }

    void release() {
        if (_fd >= 0) {
            Policy::close(_fd);
            _fd = -1;
        }
    }

    int release_ownership() {
        int temp_fd = _fd;
        _fd = -1; // Reset to prevent the destructor from calling system close()
        return temp_fd;
    }
};

// Forward declaration of the atomic registry used internally
// template <size_t MaxSlots> class StaticFileDescriptionRegistry;

template <typename Policy = FilePolicy, size_t MaxSlots = 256>
class SharedFileDescription {
private:
    int _fd = -1;
    using Registry = StaticFileDescriptionRegistry<MaxSlots>;

public:
    SharedFileDescription() = default;

    explicit SharedFileDescription(int fd) : _fd(fd) {
        if (_fd >= 0) Registry::retain(_fd);
    }

    // Variadic constructor triggered ONLY when LinuxArgs tag is passed first
    // Completely eliminates ambiguity with copy/move/conversion constructors
    template <typename... Args>
    explicit SharedFileDescription(LinuxArgs, Args&&... args) {
        _fd = Policy::open(std::forward<Args>(args)...);
        if (_fd >= 0) Registry::retain(_fd);
    }

    // Conversion constructor utilizes release_ownership() instead of pointer hacking
    explicit SharedFileDescription(UniqueFileDescription<Policy>&& unique_file_desc) 
        : _fd(unique_file_desc.release_ownership()) {
        if (_fd >= 0) {
            Registry::retain(_fd);
        }
    }

    ~SharedFileDescription() { reset(); }

    SharedFileDescription(const SharedFileDescription& other) : _fd(other._fd) {
        if (_fd >= 0) Registry::retain(_fd);
    }

    SharedFileDescription& operator=(const SharedFileDescription& other) {
        if (this != &other) {
            reset();
            _fd = other._fd;
            if (_fd >= 0) Registry::retain(_fd);
        }
        return *this;
    }

    SharedFileDescription(SharedFileDescription&& other) noexcept : _fd(other._fd) {
        other._fd = -1;
    }

    SharedFileDescription& operator=(SharedFileDescription&& other) noexcept {
        if (this != &other) {
            reset();
            _fd = other._fd;
            other._fd = -1;
        }
        return *this;
    }

    void reset() {
        if (_fd >= 0) {
            // If release returns true under mutex protection, safely close the descriptor
            if (Registry::release(_fd)) {
                Policy::close(_fd);
            }
            _fd = -1;
        }
    }

    // Acquires a locked session proxy for exclusive read/write operations
    FileDescriptionSession<Policy> lock() {
        // Fetches the dedicated fine-grained mutex for this fd and returns the active session
        return FileDescriptionSession<Policy>(_fd, Registry::get_mutex(_fd));
    }


    int get() const { return _fd; }
    size_t use_count() const { return Registry::get_count(_fd); }
    explicit operator bool() const { return _fd >= 0; }
    int operator*() const { return _fd; }
};
