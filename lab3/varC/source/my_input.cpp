//
// Created by copiedwonder on 01.11.2020.
//

#include "my_input.h"

namespace my_input {
    char* getStr(std::istream& istream) {
        char* str = nullptr, *tmp;
        char c;
        int i = 0;
        do
        {
            i++;
            tmp = (char *)realloc(str, i);
            if (tmp == nullptr) {
                throw std::bad_alloc();
            }
            str = tmp;
            istream.get(c);
            str[i - 1] = c;
        } while (c !=  '\n');
        str[i - 1] = '\0';
        return str;
    }
}
