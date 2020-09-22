//
// Created by CopiedWonder on 20.09.2020.
//

#ifndef LAB1_2_0_MY_INPUT_H
#define LAB1_2_0_MY_INPUT_H
#include <iostream>

namespace my_input {
    template<class T>
    int getNum(T &num) {
        std::cin >> num;
        if (!std::cin.good()) {
            return -1; // return error code
        }
        return 0; // return OK
    }
}
#endif //LAB1_2_0_MY_INPUT_H
