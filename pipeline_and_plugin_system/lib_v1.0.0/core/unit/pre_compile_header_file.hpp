#pragma once

#include <iostream>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <atomic>

#include <unordered_map>
#include <map>
#include <stack>
#include <cstring>
#include <list>
#include <unordered_set>
#include <stack>

#include <filesystem>
#include <dlfcn.h>

#include <cstdarg>

template <typename T>
using Shared = std::shared_ptr<T>;
template <typename T>
using Unique = std::unique_ptr<T>;

// Build share library
#if ON_VIPLUGS_ENGINE
#if DYNAMIC_BUILD
#ifdef _MSC_VER
#define VIPLUGS_API __declspec(dllexport)
#else
#define VIPLUGS_API __attribute__((visibility("default")))
#endif
#else
#define VIPLUGS_API
#endif
#else
#if DYNAMIC_IMPORT
#ifdef _MSC_VER
#define VIPLUGS_API __declspec(dllimport)
#else
#define VIPLUGS_API
#endif
#else
#define VIPLUGS_API
#endif
#endif

// Runtime assert
#define VIPLUGS_ASSERT assert

// Static assert when compile C2607
#if defined(__clang__) || defined(__gcc__)
#define VIPLUGS_STATIC_ASSERT _Static_assert
#else
#define VIPLUGS_STATIC_ASSERT static_assert
#endif

// Static cast
#define VIPLUGS_BASE_CLASS_ASSERT(baseClass, derivedClass, message) \
    VIPLUGS_STATIC_ASSERT(std::is_base_of<baseClass, derivedClass>::value &&message)

// Inline function
#if defined(__clang__) || defined(_gcc__)
#define VIPLUGS_FORCE_INLINE __attribute__((always_inline)) inline
#define VIPLUGS_NOINLINE     __attribute__((noinline))
#elif defined(_MSC_VER)
#define VIPLUGS_FORCE_INLINE __forceinline
#define VIPLUGS_NOINLINE     __declspec(noinline)
#else
#define VIPLUGS_FORCE_INLINE inline
#define VIPLUGS_NOINLINE
#endif

// Free memory
#define VIPLUGS_FREE_MEMORY(memory) \
    if (memory != nullptr) {        \
        delete memory;              \
        memory = nullptr;           \
    }

#define BIND_EVENT_FUNCTION(function) \
    [this](auto &... args) -> decltype(auto) { return this->function(std::forward<decltype(args)>(args)...); }

#define INVALID_ID 0

namespace ViPlugsEngine {
using UUID = size_t;

UUID VIPLUGS_API getUUID();

template <typename T>
UUID VIPLUGS_API getTypeUUID() {
    static UUID uuid = getUUID();  // static Func call one time
    return uuid;
}

}  // namespace ViPlugsEngine