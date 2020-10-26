//
// Created by CopiedWonder on 30.09.2020.
//

#ifndef LAB2_MY_INPUT_H
#define LAB2_MY_INPUT_H
#include <iostream>
#include <limits>

namespace my_input {
    template<class T>
    void getNum(T &num) {
        while (true) {
            std::cin >> num;
            if (!std::cin.good()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Bad input. Retry." << std::endl;
            }
            else {
                break;
            }
        }
    }
}
#endif //LAB2_MY_INPUT_H
