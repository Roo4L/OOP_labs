//
// Created by CopiedWonder on 06.10.2020.
//

#include <stdexcept>
#include <cstring>
#include <iostream>
#include "Hex.h"
#include "my_input.h"

namespace hexmath {
    Hex Hex::Complementary() const {
        Hex res;
        res.num_ = (char *)malloc(sizeof(char) * len_);
        if (res.num_ == nullptr && len_ != 0) {
            throw std::bad_alloc();
        }
        res.sign_ = Hex::sign_;
        res.len_ = Hex::len_;
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
        num_ = (char *)malloc(sizeof(char) * cp.len_); //TODO out_of_memory
        if (num_ == nullptr && cp.len_ != 0) {
            throw std::bad_alloc();
        }
        sign_ = cp.sign_;
        len_ = cp.len_;
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
    Hex::Hex(long long value) {
        sign_ = (value < 0 ? Minus : Plus);
        if (value < 0) { value *= -1;}

        long long value_cp = value;
        while (value_cp > 0 ) {
            len_++;
            value_cp /= 16;
        }
        num_ = (char *)malloc(sizeof(char) * len_);
        if (num_ == nullptr && len_ != 0) {
            sign_ = Plus;
            len_ = 0;
            throw std::bad_alloc();
        }
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
        int len = strlen(snum);
        len_ = len;
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
        if (num_ == nullptr && len_ != 0) {
            sign_ = Plus;
            len_ = 0;
            throw std::bad_alloc();
        }
        int j = 1;
        for (; i < len; i++) {
            if (isHexLetter(snum[i])) {
                num_[len_ - j] = int(snum[i]) % 16 + int(snum[i]) / 65 * 9;
                j++;
            }
            else {
                throw std::invalid_argument("The string contains invalid symbols.");
            }
        }
        this->Truncate();
    }
    Hex& Hex::setNull() noexcept {
        free(num_);
        num_ = nullptr;
        sign_ = Plus;
        len_ = 0;
        return *this;
    }

    int Hex::getDigits(char * buf, int buf_len) const {
        if (buf == nullptr) {
            throw std::invalid_argument("Buf is nullptr.");
        }
        int i = len_ - 1, j = 0;
        for (; i >= 0 && j < (buf_len - 1); i--) {
            buf[j++] = num_[i];
        }
        buf[j] = '\0';
        return i + 1;
    }

    Hex operator""_H(const char* num) {
        return Hex(num);
    }

    Hex operator+(const Hex& x_, const Hex& y_) {
        Hex x = x_.Complementary();
        Hex y = y_.Complementary();
        Hex z;
        z.len_ = x.len_ > y.len_ ? x.len_ : y.len_;
        z.num_ = (char *)calloc(z.len_, sizeof(char));
        if (z.num_ == nullptr && z.len_ != 0) {
            z.len_ = 0;
            throw std::bad_alloc();
        }
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
            z.sign_ = x_.sign_;
            if (overdrive != z.sign_) {
                z.num_ = (char *)realloc(z.num_, z.len_ + 1);
                if (z.num_ == nullptr) {
                    z.len_ = 0;
                    z.sign_ = Plus;
                    throw std::bad_alloc();
                }
                z.num_[z.len_] = z.sign_ == Plus ? '\x1' : '\xE';
                z.len_++;
            }
        }
        return z.toComplementary().Truncate();
    }
    Hex operator-(const Hex &x_, const Hex &y_) {
        Hex y = y_;
        y.sign_ = (y.sign_ == Plus ? Minus : Plus);
        return x_ + y;
    }

    Hex& Hex::operator+=(const Hex& y_) {
        Hex x = this->Complementary();
        Hex y = y_.Complementary();
        this->len_ = x.len_ > y.len_ ? x.len_ : y.len_;
        char *tmp;
        tmp = (char *)calloc(this->len_, sizeof(char));
        if (tmp == nullptr && this->len_ != 0) {
            this->len_ = x.len_;
            throw std::bad_alloc();
        }
        free(this->num_);
        this->num_ = tmp;
        int num_buf, overdrive = 0;
        int add_len = x.len_ < y.len_ ? x.len_ : y.len_;
        int i = 0;
        for (; i < add_len; i++) {
            num_buf = (x.num_[i] + y.num_[i]) + overdrive;
            this->num_[i] = int(num_buf) % 16;
            overdrive = int(num_buf) / 16;
        }
        Hex longer = x.len_ > y.len_ ? x : y;
        Hex shorter = x.len_ < y.len_ ? x : y;
        while (i < this->len_) {
            num_buf = longer.num_[i] + shorter.sign_ * 15 + overdrive;
            this->num_[i] = int(num_buf) % 16;
            overdrive = int(num_buf) / 16;
            i++;
        }
        if (x.sign_ != y.sign_) {
            this->sign_ = (x.sign_ + y.sign_ + overdrive) % 2;
        }
        else {
            this->sign_ = x.sign_;
            if (overdrive != this->sign_) {
                this->num_ = (char *)realloc(this->num_, this->len_ + 1);
                this->num_[this->len_] = (this->sign_ == Plus ? '\x1' : '\xE');
                this->len_++;
            }
        }
        return this->toComplementary().Truncate();
    }
    Hex& Hex::operator-=(const Hex& y_) {
        Hex y = y_;
        y.sign_ = (y.sign_ == Plus ? Minus : Plus);
        return *this += y;
    }

    Hex& Hex::operator<<=(unsigned int bias) {
        if (!bias) return *this;
        char *tmp;
        tmp = (char *)realloc(num_, len_ + bias);
        if (tmp == nullptr) {
            throw std::bad_alloc();
        }
        len_ += bias;
        num_ = tmp;
        for (unsigned int i = len_ - 1; i > bias - 1; i--)
            Hex::num_[i] = Hex::num_[i - bias];
        for (unsigned int i = 0; i < bias; i++)
            Hex::num_[i] = 0;
        return *this;
    }
    Hex& Hex::operator>>=(unsigned int bias) noexcept{
        if (!bias) return *this;
        for (unsigned int i = bias; i < len_; i++)
            Hex::num_[i - bias] = Hex::num_[i];
        for (unsigned int i = (len_ > bias ? len_ - bias : 0) ; i < len_; i++)
            Hex::num_[i] = 0;
        Hex::Truncate();
        if (len_ <= bias) sign_ = Plus;
        return *this;
    }

    Hex& Hex::operator=(const Hex& y) {
        if (this == &y) {
            return *this;
        }
        char *tmp;
        tmp = (char *)malloc(sizeof(char) * y.len_); //TODO out_of_memory
        if (tmp == nullptr && y.len_ != 0) {
            throw std::bad_alloc();
        }
        free(Hex::num_);
        Hex::num_ = tmp;
        Hex::len_ = y.len_;
        Hex::sign_ = y.sign_;
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
            if (x.num_[i] > y.num_[i]) {
                return !x.sign_;
            }
            else if (x.num_[i] < y.num_[i]) {
                return x.sign_;
            }
        }
        return false;
    }
    bool operator<(const Hex& x, const Hex& y) noexcept {
        return y > x;
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
        try {
            buf = my_input::getStr(is);
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
    Hex::~Hex() {
        free(num_);
        num_ = nullptr;
        sign_ = Plus;
        len_ = 0;
    }
}