//
// Created by CopiedWonder on 06.10.2020.
//

#ifndef LAB3_HEX_H
#define LAB3_HEX_H
#include <iostream>

namespace hexmath {
    static const unsigned int MAX_NUM_LEN = 16;

    class Hex {
    private:
        char num[MAX_NUM_LEN + 1];
    public:
        Hex() { this->setNull();}
        Hex(int dnum);
        Hex(char *num);
        Hex& setNull();

        bool isEven();

        friend std::istream& operator>>(std::istream& is, Hex& num);
        friend std::ostream& operator<<(std::ostream& os, Hex& num);
    };

    bool isHexLetter(char x);

}

#endif //LAB3_HEX_H
