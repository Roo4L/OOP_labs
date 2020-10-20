//
// Created by CopiedWonder on 06.10.2020.
//

#include <stdexcept>
#include <cstring>
#include <iostream>
#include <iomanip>
#include "Hex.h"

namespace hexmath {
    const char Aplhabit[17] = "0123456789ABCDEF";

    void Hex::Complimentary() {
        if (num[0] == 'F') {
            for (int i = 1; i < MAX_NUM_LEN; i++) {
                num[i] = Aplhabit[15 - (strchr(Aplhabit, num[i]) - Aplhabit)];
            }
            int buf, overdrive = 0, i = MAX_NUM_LEN - 1;
            do {
                buf = (strchr(Aplhabit, num[i]) - Aplhabit + 1
                num[i] = Aplhabit[buf % 16];
                overdrive = buf / 16;
            } while (overdrive != 0);
         }
    }

    Hex::Hex(const int decimal_num) {
        int value = decimal_num;
        num[MAX_NUM_LEN] = '\0';
        num[0] = (value < 0 ? 'F' : '0');
        if (value < 0) { value *= -1;}

        int digit;
        int it = MAX_NUM_LEN;
        while (value > 0 && it > 1) {
            it--;
            digit = value % 16;
            num[it] = '0' + digit % 10 + ('A' - '0') * digit / 10;
            value /= 16;
        }
        while (it > 1) {
            it --;
            num[it] = '0';
        }
        if (value > 0) {
            throw std::out_of_range("Constant too big for HEX class representation.");
        }
    }
    Hex::Hex(const char * const snum) {
        num[MAX_NUM_LEN] = '\0';
        size_t len = strlen(snum);
        int i = 0;
        if (len < 1) {
            throw std::out_of_range("Number have not been found.");
        }
        if (snum[0] == '-') {
            if (len > MAX_NUM_LEN - 1) {
                throw std::out_of_range("Number is longer then possible.");
            }
            num[0] = 'F';
            for (int j = 1; j < MAX_NUM_LEN - len + 1; j++) {
                num[j] = '0';
            }
            i++;
        }
        else {
            if (len > MAX_NUM_LEN) {
                throw std::out_of_range("Number is longer then possible.");
            }
            num[0] = '0';
            for (int j = 1; j < MAX_NUM_LEN - len; j++) {
                num[j] = '0';
            }
        }
        for (; i < len; i++) {
            if (isHexLetter(snum[i])) {
                num[MAX_NUM_LEN - len + i] = snum[i];
            }
            else {
                throw std::out_of_range("The string contains invalid symbols.");
            }
        }
    }
    Hex& Hex::setNull() {
        for (int i = 0; i < hexmath::MAX_NUM_LEN; i++) {
            num[i] = '0';
        }
        num[hexmath::MAX_NUM_LEN] = '\0';
        return *this;
    }

    bool isHexLetter(char x) {
        unsigned int pos1, pos2;
        pos1 = x - '0';
        pos2 = x - 'A';
        return (pos1 < 10 || pos2 < 6);
    }

    const Hex operator+(const Hex& x, const Hex& y) {
        x.Complimentary();
        y.Complimentary();
        Hex z();
        int num_buf overdrive = 0;
        for (int i = MAX_NUM_LEN - 1; i > 0; i--) {
            num_buf = (x.num[i] % 16 + y.num[i] % 16 + x.num[i] / 112 * 10 + y.num[i] / 112 * 10) + overdrive;
            z.num[i] = Aplhabit[num_buf % 16];
            overdrive = num_buf / div;
         }
        if (x.num[0] == y.num[0]) {
            if (overdrive) { // overflow
                z.num[0] = Aplhabit[15 - (strchr(Aplhabit, x.num[0]) - Aplhabit)]
            }
            else {
                z.num[0] = x[0];
            }
        }
        else {
            if (overdrive) { // overflow
                z.num[0] = '0';
            }
            else {
                z.num[0] = 'F';
            }
        }
        return z.Complimentary();
    }

    Hex operator-(const Hex &x, const Hex &y) {
        y.num[0] = Aplhabit[15 - (strchr(Aplhabit, y.num[i]) - Aplhabit)];
        return x + y;
    }

    const Hex& Hex::operator<<=(const int bias) {
        for (int i = 1; i < MAX_NUM_LEN - bias; i++) {
            this->num[i] = this->num[i + bias];
        }
        for (int i = MAX_NUM_LEN - bias; i < MAX_NUM_LEN; i++) {
            this->num[i] = '0';
        }
    }

    const Hex& Hex::operator>>=(const int bias) {
        for (int i = 1 + bias; i < MAX_NUM_LEN; i++) {
            this->num[i] = this->num[i - bias];
        }
        for (int i = 1; i < 1 + bias; i++) {
            this->num[i] = '0';
        }
    }

    const bool operator>(const Hex& x, const Hex& y) {
        for (int i = 0; i < MAX_NUM_LEN; i++) {
            if (strchr(Aplhabit, x.num[i]) - Aplhabit > strchr(Aplhabit, x.num[i]) - Aplhabit) {
                return true;
            }
        }
        return false;
    }

    const bool operator<(const Hex& x, const Hex& y) {
        return y > x;
    }

    const bool operator>=(const Hex& x, const Hex& y) {
        return !(x < y);
    }

    friend const bool operator<=(const Hex& x, const Hex& y) {
        return !(x > y);
    }

    friend const bool operator==(const Hex& x, const Hex& y) {
        return !(x > y || x < y);
    }

    friend const bool operator!=(const Hex& x, const Hex& y) {
        return x > y || x < y;
    }

    std::istream& operator>>(std::istream& is, Hex& num) {
        char buf[MAX_NUM_LEN + 1];
        is >> std::setw(MAX_NUM_LEN) >> buf;
        int len = strlen(buf);
        int start = 0;
        if (buf[0] =='-') {
            start++;
            num.num[0] = 'F';
        }
        else {
            num.num[0] = '0';
            if (buf[0] == '+') {
                start++;
            }
        }
        for (int i = start; i < len; i++) {
            if (isHexLetter(buf[i])) {
                num.num[i + (1 - start)] = buf[i];
            }
            else {
                throw std::out_of_range("Literal contains invalid symbols.");
            }
        }
        return is;
    }

    std::ostream& operator<<(std::ostream& os, const Hex& num) {
        os << (num.num[0] == '0' ? '+' : '-');
        os << (num.num + 1);
        return os;
    }

    bool Hex::isEven() {
        int last_ch_code = int(num[MAX_NUM_LEN - 2]);
        if (last_ch_code < 68) {
            return last_ch_code % 2;
        }
        else {
            return last_ch_code % 2 + 1;
        }
    }
}