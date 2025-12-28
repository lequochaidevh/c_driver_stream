#include <iostream>
#include <type_traits>

////-----------------0-----------------
template <typename T>
void printValue(const T& value) {
    if constexpr (std::is_integral<T>::value) {
        std::cout << "Integral: " << value << "\n";
    } else {
        std::cout << "Not integral\n";
    }
}

////-----------------1-----------------
template <bool Enable>
void log(const char* msg) {
    if constexpr (Enable) {
        std::cout << msg << "\n";
    }
}

////-----------------2-----------------
void foo() {
    if (false) {
        // int x = "hello";  // ❌ compile error
    }
}

template <bool Debug>
void foo() {
    if constexpr (Debug) {
        int x = "hello";  // only compile when Debug=true
    }
}

////-----------------3-----------------
// Integral
// template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
// void printV(T value) {
//     std::cout << "Integral: " << value << "\n";
// }

// Floating-point
template <typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
void printV(T value) {
    std::cout << "Floating: " << value << "\n";
}
// Default template arguments NOT joint to overload resolution
////-----------------4-----------------
// Integral
template <typename T>
std::enable_if_t<std::is_integral<T>::value> printE(T value) {
    std::cout << "Integral: " << value << "\n";
}

// Floating
template <typename T>
std::enable_if_t<std::is_floating_point<T>::value> printE(T value) {
    std::cout << "Floating: " << value << "\n";
}

// non-type template parameter (NTTP).
template <typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
void printE2(T value) {
    std::cout << "Integral: " << value << "\n";
}

template <typename T, std::enable_if_t<std::is_floating_point<T>::value, int> = 0>
void printE2(T value) {
    std::cout << "Floating: " << value << "\n";
}

int main() {
    ////-----------------0-----------------
    printValue(10);
    printValue(3.14);

    ////-----------------1-----------------
    log<true>("Camera started");
    log<false>("This will not be compiled");

    ////-----------------2-----------------
    foo<false>();

    ////-----------------3-----------------
    // printV(20);     // ERROR: SFINAE : Substitution Failure Is Not An Error.
    printV(2.22);  // ERROR: SFINAE : Substitution Failure Is Not An Error.

    ////-----------------4-----------------
    printE(10);    // Integral
    printE(3.14);  // Floating

    printE2(10);    // Integral
    printE2(3.14);  // Floating
}
