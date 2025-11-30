#include <iostream>
#include <utility>
#include <vector>

class MyBuf {
 public:
    size_t size;
    int   *data;

    // Constructor
    MyBuf(size_t s = 0) : size(s), data(nullptr) {
        if (size > 0) data = new int[size];
        std::cout << "Constructor\n";
    }

    // Copy constructor
    MyBuf(const MyBuf &other) : size(other.size) {
        data = (size > 0) ? new int[size] : nullptr;
        std::cout << "Copy constructor\n";
    }

    // Copy assignment
    MyBuf &operator=(const MyBuf &other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = (size > 0) ? new int[size] : nullptr;
        }
        std::cout << "Copy assignment\n";
        return *this;
    }

    // Move constructor
    MyBuf(MyBuf &&other) noexcept : size(other.size), data(other.data) {
        other.size = 0;
        other.data = nullptr;
        std::cout << "Move constructor\n";
    }

    // Move assignment
    MyBuf &operator=(MyBuf &&other) noexcept {
        if (this != &other) {
            delete[] data;

            size = other.size;
            data = other.data;

            other.size = 0;
            other.data = nullptr;
        }
        std::cout << "Move assignment\n";
        return *this;
    }

    ~MyBuf() {
        delete[] data;
        std::cout << "Destructor\n";
    }
};

MyBuf makeBuf() {
    MyBuf tmp(100);
    return tmp;  // RVO or Move constructor
}

int main() {
    MyBuf A(50);  // Constructor

    MyBuf B = A;  // Copy constructor

    MyBuf C = std::move(A);  // Move constructor
    // std::cout << A.data[1] << "\n"; // Segmentation fault (core dumped)

    MyBuf D;
    D = std::move(B);  // Move assignment

    MyBuf F(std::move(D));  // Move constructor

    MyBuf E = makeBuf();  // RVO or Move constructor

    // Capacitor
    std::vector<MyBuf> v;
    v.reserve(10);  // advoid reallocate when init

    std::cout << "--------------------------------------------------\n";
    std::cout << "push_back(100)\n";
    v.push_back(100);

    std::cout << "push_back(std::move(MyBuf(100)))\n";
    v.push_back(std::move(MyBuf(100)));  // MT1

    std::cout << "push_back(MyBuf(100))\n";
    v.push_back(MyBuf(100));

    std::cout << "emplace_back(100)\n";
    v.emplace_back(100);  // COPY constructor

    std::cout << "emplace_back(std::move(MyBuf(100)))\n";
    v.emplace_back(std::move(MyBuf(100)));

    std::cout << "emplace_back(MyBuf(100))\n";
    v.emplace_back(MyBuf(100));  //
    std::cout << "--------------------------------------------------\n";
}
