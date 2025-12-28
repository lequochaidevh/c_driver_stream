#include <iostream>
#include <vector>
#include <algorithm>
struct Counter {
    int  count = 0;
    void operator()(int) { ++count; }
};

int main() {
    std::vector<int> v{1, 2, 3, 4};
    Counter          c = std::for_each(v.begin(), v.end(), Counter{});
    std::cout << c.count << "\n";
}

/**
 * g++ -std=c++17  for_each_cntStruct.cpp -o build/a.out && ./build/a.out
 */