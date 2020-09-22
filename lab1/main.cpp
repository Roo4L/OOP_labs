#include <iostream>
#include <stdexcept>
#include "sparse_matrix.h"
#include "my_input.h"

int main() {
    std::cout << "Enter Matrix hight:" << std::endl;
    int n;
    (my_input::getNum(n) != 0 || n < 0) ? throw std::out_of_range("Bad matrix hight. Retry."): true;

    std::cout << "Enter Matrix width:" << std::endl;
    int m;
    (my_input::getNum(m) != 0 || m < 0) ? throw std::out_of_range("Bad matrix width. Retry.") : true;

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
    if (max_positive >= 0) {M.swap(0, max_positive);}
    int max_negative= M.max_signed(sparse_matrix::is_negative);
    if (max_negative >= 0) {M.swap(M.hight() - 1, max_negative);}
    std::cout << "After operations: " << std::endl;
    M.output();
    return 0;
}
