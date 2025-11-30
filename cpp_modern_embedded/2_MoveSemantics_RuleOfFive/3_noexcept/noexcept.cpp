#include <chrono>
#include <cstring>
#include <iostream>
#include <vector>

#define VALUE_FILL 1
int counter = 0;

class MyBuf {
 public:
    size_t size;
    int   *data;

    MyBuf(size_t s = 0) : size(s), data(nullptr) {
        if (size > 0) {
            data = new int[size];
            std::fill(data, data + size, VALUE_FILL);
        }
    }

    // Copy constructor
    MyBuf(const MyBuf &other) : size(other.size) {
        data = new int[size];
        std::fill(data, data + size, VALUE_FILL);
    }

    // DEMO_1 DEMO_2: Move constructor
    MyBuf(MyBuf &&other) noexcept : size(other.size), data(other.data) {  // noexcept STL (vector, deque, string…)
        other.size = 0;
        other.data = nullptr;
        // std::cout << "Move \n";
    }

    ~MyBuf() { delete[] data; }
};

int main() {
    constexpr int N        = 500'000;  // N elements in the vector
    constexpr int BUF_SIZE = 5'000;    // size of buffer

    std::vector<MyBuf> v;
    MyBuf              src(BUF_SIZE);

    // Force reallocation by push element after reserve is exceeded
    auto t1 = std::chrono::high_resolution_clock::now();

    // DEMO_2:
    // v.reserve(N);  // advoid reallocate when init

    for (int i = 0; i < N; ++i) {
        v.push_back(MyBuf(BUF_SIZE));  // Not need std::move(MyBuf(BUF_SIZE))
    }

    auto t2 = std::chrono::high_resolution_clock::now();

    double elapsed = std::chrono::duration<double, std::milli>(t2 - t1).count();

    std::cout << "Vector reallocate push_back time = " << elapsed << " ms\n";

    std::cout << "Vector do_back time = " << elapsed << " ms\n";
    std::cout << "Vector v[N-1].data  = " << *(v[N - 1].data) << " \n";

    std::vector<MyBuf> v2;
    t1      = std::chrono::high_resolution_clock::now();
    v2      = std::move(v);
    t2      = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration<double, std::milli>(t2 - t1).count();
    std::cout << "Vector2 move(v) time = " << elapsed << " ms\n";
    std::cout << "Vector v2[N-1].data  = " << *(v2[N - 1].data) << " \n";

    return 0;
}
