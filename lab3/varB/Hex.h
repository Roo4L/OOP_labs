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
        void Complimentary();
    public:
        Hex() { this->setNull();}
        explicit Hex(int decimal_num);
        Hex(const char *num);
        Hex& setNull();

        bool isEven();

        const Hex& operator<<=(const int bias);
        const Hex& operator>>=(const int bias);

        friend const Hex operator+(const Hex& x, const Hex& y);
        friend const Hex operator-(const Hex& x, const Hex& y);
        friend const bool operator>(const Hex& x, const Hex& y);
        friend const bool operator<(const Hex& x, const Hex& y);
        friend const bool operator>=(const Hex& x, const Hex& y);
        friend const bool operator<=(const Hex& x, const Hex& y);
        friend const bool operator==(const Hex& x, const Hex& y);
        friend const bool operator!=(const Hex& x, const Hex& y);
        friend std::istream& operator>>(std::istream& is, Hex& num);
        friend std::ostream& operator<<(std::ostream& os, const Hex& num);
    };

    bool isHexLetter(char x);

}

#endif //LAB3_HEX_H
