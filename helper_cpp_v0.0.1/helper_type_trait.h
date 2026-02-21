#pragma once

#include <type_traits>
#include <vector>
#include <memory>
#include <string>

namespace helper {

// ================================
// 1. is_vector
// ================================

template <typename T>
struct is_vector : std::false_type {};

template <typename T, typename Allocator>
struct is_vector<std::vector<T, Allocator>> : std::true_type {};

template <typename T>
inline constexpr bool is_vector_v = is_vector<T>::value;

// ================================
// 2. is_shared_ptr
// ================================

template <typename T>
struct is_shared_ptr : std::false_type {};

template <typename T>
struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};

template <typename T>
inline constexpr bool is_shared_ptr_v = is_shared_ptr<T>::value;

// ================================
// 3. remove_cvref
// (C++20 có sẵn std::remove_cvref_t)
// ================================

template <typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

// ================================
// 4. is_string
// ================================

template <typename T>
struct is_string : std::is_same<remove_cvref_t<T>, std::string> {};

template <typename T>
inline constexpr bool is_string_v = is_string<T>::value;

// ================================
// 5. enable_if_integral
// Helper alias cho SFINAE
// ================================

template <typename T>
using enable_if_integral_t = std::enable_if_t<std::is_integral_v<T>, int>;

}  // namespace helper
