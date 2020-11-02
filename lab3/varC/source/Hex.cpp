//
// Created by CopiedWonder on 06.10.2020.
//

#include <stdexcept>
#include <cstring>
#include <iostream>
#include "Hex.h"
#include "my_input.h"

namespace hexmath {
    Hex Hex::Complementary() const noexcept {
        Hex res;
        res.sign_ = Hex::sign_;
        res.len_ = Hex::len_;
        res.num_ = (char *)malloc(sizeof(char) * len_);
        if (sign_ == Minus) {
            for (int i = 0; i < len_; i++) {
                res.num_[i] = 15 - num_[i];
            }
            int overdrive = 0, i = 0;
            do {
                overdrive = int(res.num_[i] + 1) / 16;
                res.num_[i] = (res.num_[i] + 1) % 16;
                i++;
            } while (overdrive != 0 && i < len_);
        }
        else {
            for (int i = 0; i < len_; i++) {
                res.num_[i] = num_[i];
            }
        }
        return res;
    }
    Hex& Hex::toComplementary() noexcept{
        if (sign_ == Minus) {
            for (int i = 0; i < len_; i++) {
                num_[i] = 15 - num_[i];
            }
            int overdrive = 0, i = 0;
            do {
                overdrive = int(num_[i] + 1) / 16;
                num_[i] = (num_[i] + 1) % 16;
                i++;
            } while (overdrive != 0 && i < len_);
        }
        return *this;
    }
    Hex& Hex::Truncate() noexcept {
        int trunc_len = 0;
        for (int i = len_ - 1; i >= 0 && num_[i] == '\0'; i--) trunc_len++;
        if (trunc_len != 0) {
            num_ = (char *)realloc(num_, sizeof(char) * (len_ - trunc_len));
        }
        len_ -= trunc_len;
        return *this;
    }

    Hex::Hex(const Hex& cp) {
        sign_ = cp.sign_;
        len_ = cp.len_;
        num_ = (char *)malloc(sizeof(char) * len_);
        for (int i = 0; i < len_; i++) {
            num_[i] = cp.num_[i];
        }
    }
    Hex::Hex(Hex&& cp) noexcept {
        sign_ = cp.sign_;
        len_ = cp.len_;
        num_ = cp.num_;
        cp.num_ = nullptr;
        cp.len_ = 0;
        cp.sign_ = 0;
    }
    Hex::Hex(long long value) noexcept {
        sign_ = (value < 0 ? Minus : Plus);
        if (value < 0) { value *= -1;}

        long long value_cp = value;
        while (value_cp > 0 ) {
            len_++;
            value_cp /= 16;
        }
        num_ = (char *)malloc(sizeof(char) * len_);
        int it = 0;
        while (value > 0) {
            num_[it] = char(value % 16);
            value /= 16;
            it++;
        }
    }
    Hex::Hex(const char * snum) {
        if (snum == nullptr) {
            throw std::invalid_argument("Hex string doesn't exist. snum == nullptr.");
        }
        len_ = strlen(snum);
        int i = 0;
        if (len_ < 1) {
            throw std::out_of_range("Number have not been found.");
        }
        if (snum[0] == '-') {
            sign_ = len_ == 1 ? Plus : Minus;
            len_--;
            i++;
        }
        else {
            if (snum[0] == '+') {
                len_--;
                i++;
            }
            sign_ = Plus;
        }
        num_ = (char *)malloc(sizeof(char) * len_);
        int j = 1;
        for (; i < len_; i++) {
            if (isHexLetter(snum[i])) {
                num_[len_ - j] = int(snum[i]) % 16 + int(snum[i]) / 65 * 9;
                j++;
            }
            else {
                throw std::invalid_argument("The string contains invalid symbols.");
            }
        }
    }
    Hex& Hex::setNull() noexcept {
        free(num_);
        num_ = nullptr;
        sign_ = Plus;
        len_ = 0;
        return *this;
    }
    Hex operator""_H(const char* num) {
        return Hex(num);
    }

    Hex operator+(const Hex& x_, const Hex& y_) noexcept {
        Hex x = x_.Complementary();
        Hex y = y_.Complementary();
        Hex z;
        z.len_ = x.len_ > y.len_ ? x.len_ : y.len_;
        z.num_ = (char *)calloc(z.len_, sizeof(char));
        int num_buf, overdrive = 0;
        int add_len = x.len_ < y.len_ ? x.len_ : y.len_;
        int i = 0;
        for (; i < add_len; i++) {
            num_buf = (x.num_[i] + y.num_[i]) + overdrive;
            z.num_[i] = int(num_buf) % 16;
            overdrive = int(num_buf) / 16;
        }
        Hex longer = x.len_ > y.len_ ? x : y;
        Hex shorter = x.len_ < y.len_ ? x : y;
        while (i < z.len_) {
            num_buf = longer.num_[i] + shorter.sign_ * 15 + overdrive;
            z.num_[i] = int(num_buf) % 16;
            overdrive = int(num_buf) / 16;
            i++;
        }
        if (x_.sign_ != y_.sign_) {
            z.sign_ = (x_.sign_ + y_.sign_ + overdrive) % 2;
        }
        else {
            if (!overdrive) {
                z.sign_ = x_.sign_;
            } else {
                z.num_ = (char *)realloc(z.num_, z.len_ + 1);
                z.num_[z.len_] = z.sign_ == Plus ? '\x1' : '\xF';
                z.len_++;
            }
        }
        return z.toComplementary().Truncate();
    }
    Hex operator-(const Hex &x_, const Hex &y_) noexcept {
        Hex y = y_;
        y.sign_ = (y.sign_ == Plus ? Minus : Plus);
        return x_ + y;
    }

    Hex& operator+=(Hex& x_, const Hex& y_) noexcept {
        Hex x = x_.Complementary();
        Hex y = y_.Complementary();
        x_.len_ = x.len_ > y.len_ ? x.len_ : y.len_;
        free(x.num_);
        x_.num_ = (char *)calloc(x_.len_, sizeof(char));
        int num_buf, overdrive = 0;
        int add_len = x.len_ < y.len_ ? x.len_ : y.len_;
        int i = 0;
        for (; i < add_len; i++) {
            num_buf = (x.num_[i] + y.num_[i]) + overdrive;
            x_.num_[i] = int(num_buf) % 16;
            overdrive = int(num_buf) / 16;
        }
        Hex longer = x.len_ > y.len_ ? x : y;
        Hex shorter = x.len_ < y.len_ ? x : y;
        while (i < x_.len_) {
            num_buf = longer.num_[i] + shorter.sign_ * 15 + overdrive;
            x_.num_[i] = int(num_buf) % 16;
            overdrive = int(num_buf) / 16;
            i++;
        }
        if (x.sign_ != y.sign_) {
            x_.sign_ = (x.sign_ + y.sign_ + overdrive) % 2;
        }
        else {
            if (!overdrive) {
                x_.sign_ = x.sign_;
            } else {
                x_.num_ = (char *)realloc(x_.num_, x_.len_ + 1);
                x_.num_[x_.len_] = x_.sign_ == Plus ? '\x1' : '\xF';
                x_.len_++;
            }
        }
        return x_.toComplementary().Truncate();
    }
    Hex& operator-=(Hex& x_, const Hex& y_) noexcept {
        Hex y = y_;
        y.sign_ = (y.sign_ == Plus ? Minus : Plus);
        return x_ += y;
    }

    Hex& Hex::operator<<=(unsigned int bias) noexcept {
        if (!bias) return *this;
        len_ += bias;
        num_ = (char *)realloc(num_, len_);
        for (int i = len_ - 1; i > bias - 1; i--)
            Hex::num_[i] = Hex::num_[i - bias];
        for (int i = 0; i < bias; i++)
            Hex::num_[i] = 0;
        return *this;
    }
    Hex& Hex::operator>>=(unsigned int bias) noexcept {
        if (!bias) return *this;
        for (int i = 0; i < len_ - bias; i++)
            Hex::num_[i] = Hex::num_[i + bias];
        for (int i = len_ - bias; i < len_; i++)
            Hex::num_[i] = 0;
        Hex::Truncate();
        return *this;
    }

    Hex& Hex::operator=(const Hex& y) noexcept {
        if (this == &y) {
            return *this;
        }
        free(Hex::num_);
        Hex::len_ = y.len_;
        Hex::sign_ = y.sign_;
        Hex::num_ = (char *)malloc(sizeof(char) * Hex::len_);
        for (int i = 0; i < Hex::len_; i++) Hex::num_ = y.num_;
        return *this;
    }
    Hex& Hex::operator=(Hex&& y) noexcept {
        free(num_);
        len_ = y.len_;
        sign_ = y.sign_;
        num_ = y.num_;
        y.num_ = nullptr;
        y.len_ = 0;
        y.sign_ = 0;
        return *this;
    }

    bool operator>(const Hex& x, const Hex& y) noexcept {
        if (x.sign_ != y.sign_) {
            return x.sign_ < y.sign_;
        }
        if (x.len_ != y.len_) {
            if (x.sign_ == Plus) {
                return x.len_ > y.len_;
            }
            else {
                return x.len_ < y.len_;
            }
        }
        for (int i = x.len_ - 1; i >= 0; i--) {
            if (int(x.num_[i]) > y.num_[i]) {
                return x.sign_;
            }
            else if (int(x.num_[i] < y.num_[i])) {
                return !x.sign_;
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

    std::istream& operator>>(std::istream& is, Hex& x) noexcept {
        char *buf;
        buf = my_input::getStr(is);
        try {
            x = Hex(buf);
        }
        catch (...) {
            is.setstate(std::ios::failbit);
        }
        return is;
    }
    std::ostream& operator<<(std::ostream& os, const Hex& x) noexcept{
        os << (x.sign_ == 0 ? '+' : '-');
        for (int i = x.len_ - 1; i >= 0; i--) {
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