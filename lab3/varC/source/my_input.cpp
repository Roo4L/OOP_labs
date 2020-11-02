//
// Created by copiedwonder on 01.11.2020.
//

#include "my_input.h"

namespace my_input {
    char* getStr(std::istream& istream) {
        char* str = nullptr;
        char c;
        int i = 0;
        do
        {
            i++;
            str = (char *)realloc(str, i);
            istream.get(c);
            str[i - 1] = c;
        } while (c !=  '\n');
        str[i - 1] = '\0';
        return str;
    }
}
