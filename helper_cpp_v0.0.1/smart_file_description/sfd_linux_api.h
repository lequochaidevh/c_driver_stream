#pragma once
#include "sfd_static_registry.h"

// Compile-time tag to invoke Linux open()
struct LinuxStandardFile {
    int operator()(const char* pathname, int flags) const {
        return ::open(pathname, flags);
    }
    int operator()(const char* pathname, int flags, mode_t mode) const {
        return ::open(pathname, flags, mode);
    }
};

// Compile-time tag to invoke Linux socket()
struct LinuxSocket {
    int operator()(int domain, int type, int protocol) const {
        return ::socket(domain, type, protocol);
    }
};

// Compile-time tag to invoke Linux pipe()
struct LinuxPipe {
    int operator()(int pipefd[2]) const {
        return ::pipe(pipefd);
    }
};
