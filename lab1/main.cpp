#include <iostream>
#include "sparse_matrix.h"
#include "my_input.h"
#include <stdexcept>

int main() {
    std::cout << "Enter Matrix hight:" << std::endl;
    int n;
    while (my_input::getNum(n) != 0 || n < 0) {
        throw std::out_of_range("Bad matrix hight. Retry.");
    }
    std::cout << "Enter Matrix width:" << std::endl;
    int m;
    while (my_input::getNum(m) != 0 || m < 0) {
        throw std::out_of_range("Bad matrix width. Retry.");
    }
    sparse_matrix::Matrix M(n, m);
    std::cout << "Enter matrix fields in template {i j num}. Enter any string to end session." << std::endl;
    int i,j;
    float num;
    while (my_input::getNum(i) == 0) {
        if (my_input::getNum(j) !=0) {
            break;
        }
        if (my_input::getNum(num) != 0) {
            break;
        }
        M.add(i, j, num);
        std::cout << "Number entered successfully. Continue." << std::endl;
    }
    std::cout << "Original matrix:" << std::endl;
    M.output();
    int max_positive = M.max_signed(sparse_matrix::is_positive);
    M.swap(0, max_positive);
    int max_negative= M.max_signed(sparse_matrix::is_negative);
    M.swap(M.hight() - 1, max_negative);
    std::cout << "After operations: " << std::endl;
    M.output();
    return 0;
}
