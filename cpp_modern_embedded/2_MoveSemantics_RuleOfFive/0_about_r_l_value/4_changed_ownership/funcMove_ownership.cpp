#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <memory>

void foo(std::shared_ptr<int>&& p) {
    // No change ownership;
}

std::shared_ptr<int> foo2(std::shared_ptr<int>&& p) {
    return std::move(p); // In order to change ownership;
}

int* foo3(int*&& p) {
    return std::move(p);
}

std::string foo4(std::string&& p) {
    return std::move(p);
}

int main() {
    std::shared_ptr<int> a = std::make_shared<int>(10); // can use unique
    std::shared_ptr<int> b = std::move(a); // OK

    std::cout << (a ? "a not null" : "a is null") << "\n"; // a is null
    std::cout << (b ? "b not null" : "b is null") << "\n"; // b not null
    
    foo(std::move(b)); // OK
    std::cout << (b ? "b not null" : "b is null") << "\n"; // b not null

    foo2(std::move(b)); // OK
    std::cout << (b ? "b not null" : "b is null") << "\n"; // b is null

    int* c = new int(12);
    int* d;
    d = foo3(std::move(c)); // RAW pointer move = copy. -> d = c;  // copy
    std::cout << (c ? "c not null" : "c is null") << "\n"; // c not null
    // RAW pointer NOT HAVE “move transferred ownership”.
    // cast c from int*& to int*&&

    std::string e = "Hello";
    std::string f;
    f = foo4(std::move(e)); // OK
    std::cout << (e!="" ? "data e not null" : "data e is null") << std::endl; // data e is null -> e==""
    std::cout << "data e = " << e << std::endl;
    std::cout << "data f = " << f << std::endl;

}

/*
g++ funcMove_ownership.cpp -o tfunc && ./tfunc 
a is null
b not null
b not null
b is null
c not null
data e is null
data e = 
data f = Hello
*/