#include <chrono>
#include <cstring>
#include <iostream>
#include <vector>

#define DEMO_3

#define VALUE_FILL 1
int counter = 0;

#ifdef DEMO_3
# define STDCOUT \
  if (true) {    \
  } else         \
   std::cout
#else
# define STDCOUT std::cout
#endif

class MyBuf {
 public:
    size_t size;
    int*   data;
    int    id;  // <-- add ID

    static int global_id;

    MyBuf(size_t s = 0) : size(s), data(nullptr), id(global_id++) {
        if (size > 0) {
            data = new int[size];
            std::fill(data, data + size, VALUE_FILL);

            // std::cout << "  Native Constructor MyBuf Set id = " << id << "\n";
            // STDCOUT << "  Native Constructor MyBuf Set id = " << id << "\n";
        }
    }

    MyBuf(const MyBuf& other) : size(other.size), id(++global_id) {
        data = new int[size];
        memcpy(data, other.data, size * sizeof(int));
    }

    // DEMO_2 noexcept
    MyBuf(MyBuf&& other) noexcept
        : size(other.size),
          data(other.data),
          id(other.id + 1)  // wrong
    {
        other.size = 0;
        other.data = nullptr;
        // std::cout << "  Move Constructor MyBuf Set id = " << id << "\n";
        // STDCOUT << "  Move Constructor MyBuf Set id = " << id << "\n";
    }

    ~MyBuf() {  //
        ++counter;
        STDCOUT << counter << "  Move Destructor MyBuf id = " << id << "\n";
    }
};

int MyBuf::global_id = 0;

int main() {
    // DEMO_3 - Allocator after change 100'000

    constexpr int BUF_SIZE = 5'000;  // size of buffer
#ifdef DEMO_3
    constexpr int N = 100'000;  // N buffer in the vector
#else
    constexpr int N = 5;  // N buffer in the vector
#endif

    std::vector<MyBuf> v;

    auto t1 = std::chrono::high_resolution_clock::now();

// DEMO_3:
#ifdef DEMO_2
    // Force reallocation by push element after reserve is exceeded
    v.reserve(N);  // advoid reallocate when init
#endif

// DEMO_3:
#ifdef DEMO_2
    // Force reallocation by push element after reserve is exceeded
    v.reserve(N);  // advoid reallocate when init
#endif

    for (int i = 1; i < N + 1; ++i) {
        STDCOUT << i << " ------ push \n";

#ifdef DEMO_3
        // ~ with 1 LOG
        // v.push_back(std::move(MyBuf(BUF_SIZE)));  // MT1

        // ~ with 1 LOG
        // v.push_back(MyBuf(BUF_SIZE));

        // v.emplace_back(BUF_SIZE);  // COPY constructor

        // Samp samp MT1 // Faster MT1 if not print log
        v.emplace_back(std::move(MyBuf(BUF_SIZE)));

        // v.emplace_back(MyBuf(BUF_SIZE));  // low MT1 if not print log

        // ~800ms with 1 LOG
        // v.push_back(BUF_SIZE);

        // v.push_back(MyBuf(BUF_SIZE));  //
#else
        // v.push_back(std::move(MyBuf(BUF_SIZE)));  // OK
        v.push_back(MyBuf(BUF_SIZE));
#endif
    }

    auto t2 = std::chrono::high_resolution_clock::now();

    double elapsed = std::chrono::duration<double, std::milli>(t2 - t1).count();

    std::cout << "Vector do_back time = " << elapsed << " ms\n";
    // std::cout << "Vector v[49999].data  = " << *(v[49999].data) << " \n";

    std::vector<MyBuf> v2;
    t1      = std::chrono::high_resolution_clock::now();
    v2      = std::move(v);
    t2      = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration<double, std::milli>(t2 - t1).count();
    std::cout << "Vector2 move(v) time = " << elapsed << " ms\n";
    // std::cout << "Vector v2[49999].data  = " << *(v2[49999].data) << " \n";

    return 0;
}
