#pragma once
#include "sfd_static_registry.h"

// --- INTEGRATED COMPILE-TIME POLICIES (OPEN + CLOSE) ---

// Policy for standard files, pipes, and devices (/dev/...)
struct FilePolicy {
    template <typename... Args> 
    static int open(Args&&... args) { return ::open(std::forward<Args>(args)...); }
    
    static void close(int fd) { if (fd >= 0) ::close(fd); }

    // Standard POSIX write
    static ssize_t write(int fd, const void* buffer, size_t count) {
        return ::write(fd, buffer, count);
    }

    // Standard POSIX read
    static ssize_t read(int fd, void* buffer, size_t count) {
        return ::read(fd, buffer, count);
    }
};

// Policy for network sockets (TCP/UDP)
struct SocketPolicy {
    template <typename... Args> 
    static int open(Args&&... args) { return ::socket(std::forward<Args>(args)...); }
    
    static void close(int fd) { 
        if (fd >= 0) { ::shutdown(fd, SHUT_RDWR); ::close(fd); } 
    }

    // Specialized socket write using send() with default flags (0)
    static ssize_t write(int fd, const void* buffer, size_t count) {
        return ::send(fd, buffer, count, 0); 
    }

    // Specialized socket read using recv() with default flags (0)
    static ssize_t read(int fd, void* buffer, size_t count) {
        return ::recv(fd, buffer, count, 0);
    }
    
    // Custom socket-only extended API (Perfect Forwarding for flags support)
    template <typename... Args>
    static ssize_t send_custom(int fd, const void* buffer, size_t count, int flags) {
        return ::send(fd, buffer, count, flags);
    }
};
