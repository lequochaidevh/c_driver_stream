#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> v{1, 2, 3, 4};

    std::for_each(v.begin(), v.end(), [](int x) { std::cout << x << " "; });

    std::cout << "\n";

    int sum = 0;

    std::for_each(v.begin(), v.end(), [&sum](int x) { sum += x; });

    std::cout << "Sum = " << sum << "\n";

    int  res;
    auto f = std::for_each(v.begin(), v.end(), [](int& x) { x *= 2; });

    f(res);
    std::cout << "res = " << res << "\n";
}

/**
 * g++ -std=c++17  for_each_basic.cpp -o build/a.out
 */