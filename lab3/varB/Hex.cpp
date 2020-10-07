//
// Created by CopiedWonder on 06.10.2020.
//

#include <stdexcept>
#include <cstring>
#include "Hex.h"

namespace hexmath {

    Hex::Hex(int dnum) {
        num[MAX_NUM_LEN] = '\0';
        num[0] = (dnum < 0 ? 'F' : '0');
        if (dnum < 0) { dnum *= -1;}

        int digit;
        int it = MAX_NUM_LEN;
        while (dnum > 0 && it > 1) {
            it--;
            digit = dnum % 16;
            num[it] = '0' + digit % 10 + ('A' - '0') * digit / 10;
            dnum /= 16;
        }
        while (it > 1) {
            it --;
            num[it] = '0';
        }
        if (dnum > 0) {
            throw std::out_of_range("Constant too big for HEX class representation.");
        }
    }
    Hex::Hex(char *snum) {
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

    std::istream& operator>>(std::istream& is, Hex& num) {
        char buf[MAX_NUM_LEN + 1];
        is >> std::setw(MAX_NUM_LEN + 1) >> buf;
        int len = strlen(buf);
        int start = 0;
        if (buf[0] =='-') {
            start++;
            num[0] == 'F';
        }
        else {
            num[0] == '0';
            if (buf[0] == '+') {
                start++;
            }
        }
        for (int i = start; i < len; i++) {
            if (isHexLetter(buf[i])) {
                num[i + (1 - start)] = buf[i];
            }
            else {
                throw std::out_of_range("Literal contains invalid symbols.");
            }
        }
        return is;
    }

    std::ostream& operator<<(std::ostream& os, Hex& num) {
        os << (num->num[0] == '0' ? '+' : '-');
        os << (num->num + 1);
        return os;
    }
}