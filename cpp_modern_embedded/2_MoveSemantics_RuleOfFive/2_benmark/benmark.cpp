#include <chrono>
#include <cstring>
#include <iostream>
#include <vector>

#define VALUE_FILL 1

class MyBuf {
 public:
    size_t size;
    int   *data;
    MyBuf(size_t s = 0) : size(s), data(nullptr) {
        if (size > 0) {
            data = new int[size];
            std::fill(data, data + size, VALUE_FILL);  // #define VALUE_FILL 1
        }
    }
    // Copy constructor
    MyBuf(const MyBuf &other) : size(other.size) {
        data = new int[size];
        std::fill(data, data + size, VALUE_FILL);
    }
    // Move constructor
    MyBuf(MyBuf &&other) noexcept : size(other.size), data(other.data) {  // noexcept STL (vector, deque, string…)
        other.size = 0;
        other.data = nullptr;
        std::cout << "Move constructor\n";
    }
    // Move assignment
    // MyBuf &operator=(MyBuf &&other) noexcept {
    //   if (this != &other) {
    //     delete[] data;
    //     size = other.size;
    //     data = other.data;
    //     other.size = 0;
    //     other.data = nullptr;
    //   }
    //   std::cout << "Move assignment\n";
    //   return *this;
    // }
    ~MyBuf() { delete[] data; }
};

int main() {
    const size_t BIG = 50'000'000;  //
    MyBuf        src(BIG);

    // -------- COPY BENCHMARK --------
    auto  t1 = std::chrono::high_resolution_clock::now();
    MyBuf copyBuf(src);  // copy constructor
    auto  t2 = std::chrono::high_resolution_clock::now();

    auto copy_time = std::chrono::duration<double, std::milli>(t2 - t1).count();

    // -------- MOVE BENCHMARK --------
    MyBuf temp(BIG);  // make a large temporary

    auto  t3 = std::chrono::high_resolution_clock::now();
    MyBuf moveBuf(std::move(temp));  // move constructor
    // MyBuf moveBuf = std::move(temp); // move constructor

    // ------------------------- move assignment
    // MyBuf moveBuf;
    // moveBuf = std::move(temp);
    auto t4 = std::chrono::high_resolution_clock::now();

    auto move_time = std::chrono::duration<double, std::milli>(t4 - t3).count();

    // -------- OUTPUT --------
    std::cout << "Copy time: " << copy_time << " ms\n";
    std::cout << "Move time: " << move_time << " ms\n";

    // Check data
    std::cout << "Move buffer value = " << moveBuf.data[5] << " \n";

    return 0;
}
