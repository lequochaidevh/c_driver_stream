#include <algorithm>
#include <complex>
#include <iostream>
#include <vector>

#include "public/matrix.hpp"
using namespace math;

int main() {
    Matrix A(2, 3);
    Matrix B(3, 4);

    std::fill(A.data(), A.data() + 6, 1.f);
    std::fill(B.data(), B.data() + 12, 2.f);

    // Matrix C = add(A, B);
    Matrix C = multi(A, B);

    // test result
    for (size_t i = 0; i < C.rows(); i++) {
        for (size_t j = 0; j < C.cols(); j++) {
            std::cout << C.data()[i * C.cols() + j] << " ";
        }
        std::cout << "\n";
    }

    return 0;
}

// export CPP_LIST=$(find . -name "*.cpp") &&  g++ -std=c++17 $CPP_LIST -I"$(pwd)/math" -o test