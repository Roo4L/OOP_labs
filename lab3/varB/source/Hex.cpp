//
// Created by CopiedWonder on 06.10.2020.
//

#include <stdexcept>
#include <cstring>
#include <iostream>
#include <iomanip>
#include "Hex.h"

namespace hexmath {
    Hex Hex::Complementary() const noexcept{
        Hex res;
        res.sign_ = Hex::sign_;
        if (sign_ == Minus) {
            for (int i = 0; i < MAX_NUM_LEN; i++) {
                res.num_[i] = 15 - num_[i];
            }
            int overdrive = 0, i = MAX_NUM_LEN - 1;
            do {
                overdrive = int(res.num_[i] + 1) / 16;
                res.num_[i] = (res.num_[i] + 1) % 16;
                i--;
            } while (overdrive != 0 && i >= 0);
        }
        else {
            for (int i = 0; i < MAX_NUM_LEN; i++) {
                res.num_[i] = num_[i];
            }
        }
        return res;
    }

    Hex::Hex(const Hex& cp) {
        sign_ = cp.sign_;
        for (int i = 0; i < MAX_NUM_LEN; i++) {
            num_[i] = cp.num_[i];
        }
    }
    Hex::Hex(long long value) {
        sign_ = (value < 0 ? Minus : Plus);
        if (value < 0) { value *= -1;}

        int it = MAX_NUM_LEN;
        while (value > 0 && it > 0) {
            it--;
            num_[it] = char(value % 16);
            value /= 16;
        }
        while (it > 0) {
            it --;
            num_[it] = '\0';
        }
        if (value > 0) {
            throw std::out_of_range("Constant too big for HEX class representation.");
        }
    }
    Hex::Hex(const char * const snum) {
        size_t len = strlen(snum);
        int i = 0;
        if (len < 1) {
            throw std::out_of_range("Number have not been found.");
        }
        if (snum[0] == '-') {
            if (len > MAX_NUM_LEN + 1) {
                throw std::out_of_range("Number is longer then possible.");
            }
            sign_ = Minus;
            i++;
        }
        else {
            if (snum[0] == '+') {
                i++;
                if (len > MAX_NUM_LEN + 1) {
                    throw std::out_of_range("Number is longer then possible.");
                }
            }
            else if  (len > MAX_NUM_LEN) {
                throw std::out_of_range("Number is longer then possible.");
            }
            sign_ = Plus;

        }
        for (int j = 0; j < MAX_NUM_LEN - len + i; j++) {
            num_[j] = '\0';
        }
        for (; i < len; i++) {
            if (isHexLetter(snum[i])) {
                num_[MAX_NUM_LEN - len + i] = int(snum[i]) % 16 + int(snum[i]) / 65 * 9;
            }
            else {
                throw std::invalid_argument("The string contains invalid symbols.");
            }
        }
    }
    Hex& Hex::setNull() noexcept {
        for (int i = 0; i < hexmath::MAX_NUM_LEN; i++) {
            num_[i] = 0;
        }
        sign_ = Plus;
        return *this;
    }

    Hex operator+(const Hex& x_, const Hex& y_) noexcept {
        Hex x = x_.Complementary();
        Hex y = y_.Complementary();
        Hex z;
        int num_buf, overdrive = 0;
        for (int i = MAX_NUM_LEN - 1; i >= 0; i--) {
            num_buf = (x.num_[i] + y.num_[i]) + overdrive;
            z.num_[i] = int(num_buf) % 16;
            overdrive = int(num_buf) / 16;
        }
        z.sign_ = (x_.sign_ + y_.sign_ + overdrive) % 2;
        return z.Complementary();
    }

    Hex operator-(const Hex &x_, const Hex &y_) noexcept {
        Hex y = y_;
        y.sign_ = (y.sign_ == Plus ? Minus : Plus);
        return x_ + y;
    }

    const Hex& Hex::operator<<=(const int bias) {
        if (bias > 0) {
            for (int i = MAX_NUM_LEN - 1; i > bias - 1; i--)
                Hex::num_[i] = Hex::num_[i - bias];
            for (int i = 0; i < bias && i < MAX_NUM_LEN; i++)
                Hex::num_[i] = 0;
        }
        else if (bias < 0) throw std::invalid_argument("Bit shift operation can't take a negative shift.");
        return *this;
    }

    const Hex& Hex::operator>>=(const int bias) {
        if (bias > 0) {
            for (int i = 0; i < MAX_NUM_LEN - bias; i++)
                Hex::num_[i] = Hex::num_[i + bias];
            for (int i = (MAX_NUM_LEN - bias > 0 ? MAX_NUM_LEN - bias : 0); i < MAX_NUM_LEN; i++)
                Hex::num_[i] = 0;
        }
        else if (bias < 0) throw std::invalid_argument("Bit shift operation can't take a negative shift.");
        return *this;
    }

    bool operator>(const Hex& x, const Hex& y) noexcept {
        if (x.sign_ != y.sign_) {
            return x.sign_ < y.sign_;
        }
        for (int i = 0; i < MAX_NUM_LEN; i++) {
            if (int(x.num_[i]) > y.num_[i]) {
                return true;
            }
            else if (int(x.num_[i] < y.num_[i])) {
                return false;
            }
        }
        return false;
    }

    bool operator<(const Hex& x, const Hex& y) noexcept {
        return y < x;
    }

    bool operator>=(const Hex& x, const Hex& y) noexcept {
        return !(x < y);
    }

    bool operator<=(const Hex& x, const Hex& y) noexcept {
        return !(x > y);
    }

    bool operator==(const Hex& x, const Hex& y) noexcept {
        return !(x > y || x < y);
    }

    bool operator!=(const Hex& x, const Hex& y) noexcept {
        return x > y || x < y;
    }

    std::istream& operator>>(std::istream& is, Hex& x) {
        char buf[MAX_NUM_LEN + 2];
        std::cin >> buf;
        x = Hex(buf);
        return is;
    }

    std::ostream& operator<<(std::ostream& os, const Hex& x) noexcept{
        os << (x.sign_ == 0 ? '+' : '-');
        int i = 0;
        while (x.num_[i] == 0) {
            i++;
        }
        for (; i < MAX_NUM_LEN; i++) {
            if (x.num_[i] > 9) {
                os << char(55 + x.num_[i]);
            }
            else {
                os << char(48 + x.num_[i]);
            }
        }
        return os;
    }
}