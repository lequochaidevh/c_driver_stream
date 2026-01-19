#include <algorithm>
#include <complex>
#include <iostream>
#include <vector>

#include "public/matrix.hpp"
using namespace math;

void prinf_mat(const Matrix& C);

int main() {
    Matrix A(2, 3);
    Matrix B(2, 3);

    Matrix sCalc(3, 4);

    std::fill(A.data(), A.data() + 6, 4.f);
    std::fill(B.data(), B.data() + 6, 3.f);

    std::fill(sCalc.data(), sCalc.data() + 12, 2.f);

    std::cout << "\nA:\n";
    prinf_mat(A);

    std::cout << "\nB:\n";
    prinf_mat(B);

    // -------- Add --------
    std::cout << "\n _Add_:\n";
    Matrix _Add_ = add(A, B);
    prinf_mat(_Add_);

    // -------- Sub --------
    std::cout << "\n _Sub_:\n";
    Matrix _Sub_ = sub(A, B);
    prinf_mat(_Sub_);

    // -------- Multi (element-wise) --------
    std::cout << "\n _Multi_:\n";
    Matrix _Multi_ = multi(A, B);
    prinf_mat(_Multi_);

    // -------- Div --------
    std::cout << "\n _Div_:\n";
    Matrix _Div_ = div(A, B);
    prinf_mat(_Div_);

    // -------- Scale --------
    std::cout << "\n _Scale_ (alpha = 0.5):\n";
    Matrix _Scale_ = scale(A, 0.5f);
    prinf_mat(_Scale_);

    // -------- Exp --------
    std::cout << "\n _Exp_:\n";
    Matrix _Exp_ = exp(A);
    prinf_mat(_Exp_);

    // -------- Log --------
    std::cout << "\n _Log_:\n";
    Matrix _Log_ = log(A);
    prinf_mat(_Log_);
    // log(1) = 0

    // -------- Sqrt --------
    std::cout << "\n _Sqrt_:\n";
    Matrix _Sqrt_ = sqrt(A);
    prinf_mat(_Sqrt_);

    // ===== MatMulti =====
    Matrix M1(2, 3);
    Matrix M2(3, 2);

    // std::fill or set value order
    // M1 = [1 2 3
    //       4 5 6]
    for (size_t i = 0; i < 6; ++i) M1.data()[i] = float(i + 1);

    // M2 = [1 2
    //       3 4
    //       5 6]
    for (size_t i = 0; i < 6; ++i) M2.data()[i] = float(i + 1);

    std::cout << "\nM1:\n";
    prinf_mat(M1);

    std::cout << "\nM2:\n";
    prinf_mat(M2);

    std::cout << "\n _MatMul_:\n";
    Matrix _MatMul_ = matmul(M1, M2);
    prinf_mat(_MatMul_);

    return 0;
}

//

void prinf_mat(const Matrix& C) {
    // test result
    for (size_t i = 0; i < C.rows(); i++) {
        for (size_t j = 0; j < C.cols(); j++) {
            std::cout << C.data()[i * C.cols() + j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}
// export CPP_LIST=$(find . -name "*.cpp") &&  g++ -std=c++17 $CPP_LIST -I"$(pwd)/math" -o test
// -O3 -march=native -ffast-math -fopenmp