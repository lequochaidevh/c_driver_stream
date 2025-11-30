#include <iostream>
#include <string>
#include <vector>

template <class T>
void f(T&& x);  // universal reference

template <typename T>
void foo(T&& x) {
    // x Can transform to lvalue or rvalue depend INPUT <T>
}

int a = 5;

class A {
    void foo(A&& other);  // rvalue reference,
};

template <typename T>
std::vector<T>&& vt = std::vector<T>{};

int main() {
    foo(a);   // T = int& → T&& = int& && → int& (lvalue reference!)
    foo(10);  // T = int  → T&& = int&&    (rvalue reference)

    auto&& x = a;   // a is lvalue → x is lvalue reference
    auto&& y = 10;  // 10 is rvalue → y is rvalue reference

    int            b = 5;
    decltype(auto) z = (b);  // z is int&

    // Alway var is rvalue
    int&& c = 10;  // rvalue reference
    // std::string&& s = getName();

    vt<int>;     // OK
    vt<double>;  // OK
    // vt<int&>;      // ERROR Compile Time
    vt<int>.push_back(1);
}
