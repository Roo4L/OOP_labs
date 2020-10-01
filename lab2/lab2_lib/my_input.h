//
// Created by CopiedWonder on 30.09.2020.
//

#ifndef LAB2_MY_INPUT_H
#define LAB2_MY_INPUT_H
#include <iostream>

namespace my_input {
    template<class T>
    int getNum(T &num) {
        std::cin >> num;
        if (!std::cin.good()) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max());
            return -1; // return error code
        }
        return 0; // return OK
    }
}
#endif //LAB2_MY_INPUT_H
