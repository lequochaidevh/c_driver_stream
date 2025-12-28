#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

#include <execution>
/**
std::execution::seq   //
std::execution::par   // parallel (multi-thread)
std::execution::par_unseq // parallel + vectorize


|             | par         | par_unseq    |
| ----------- | ----------- | ------------ |
| Thread      | ✔           | ✔            |
| SIMD        | ❌          | ✔            |
| Atomic      | ✔           | ❌           |
| Mutex       | ✔           | ❌           |
| Side-effect | not good    | ❌           |
| Performance | high        | **HIGHEST**  |

 */
// algorithm(policy, begin, end, ...);

int main() {
    std::vector<int> src{1, 2, 3, 4};
    std::vector<int> dst(src.size());
    std::transform(std::execution::par, src.begin(), src.end(), dst.begin(), [](int x) {
        std::cout << x << "\n";
        return x * 2;
    });
    std::cout << "d3 = " << dst[3] << "\n";  // WRONG VALUE
    std::vector<int> v(1'000'000, 1);
    int              sum = 0;  // side-effect

    std::for_each(std::execution::par, v.begin(), v.end(), [&](int x) {
        sum += x;  // ❌ data race => side-effect
    });

    std::cout << sum << "\n";  // WRONG VALUE

    sum = 0;

    sum = std::reduce(std::execution::par, v.begin(), v.end(), 0);
    std::cout << sum << "\n";
}

/*
### g++ -std=c++17  algorithm_parallel.cpp -o build/a.out -ltbb && ./build/a.out
102280
### ./build/a.out
102312
### ./build/a.out
109527
### ./build/a.out
111137

parallel + side-effect = BUG
*/

/**
 * std::reduce(policy, first, last, init, binary_op)
 * (((init operator a1) op a2) op a3) ... op an (op + - * /)
 * binary_op default:    = std::plus<>
 */

/*
multi
int prod = std::reduce(v.begin(), v.end(), 1,
                      std::multiplies<>());

OR bit
int mask = std::reduce(v.begin(), v.end(), 0,
                      std::bit_or<>());

Max
int m = std::reduce(v.begin(), v.end(),
                   std::numeric_limits<int>::min(),
                   [](int a, int b) {
                       return std::max(a, b);
                   });

| Purpose  | op    | init      |     |
| -------- | ----- | --------- | --- |
| Sum      | `+`   | `0`       |     |
| Multi    | `*`   | `1`       |     |
| Max      | `max` | `INT_MIN` |     |
| Min      | `min` | `INT_MAX` |     |
| OR bit   | `     | `         | `0` |
| AND bit  | `&`   | `~0`      |     |
| XOR      | `^`   | `0`       |     |

*/