//
// Created by CopiedWonder on 06.10.2020.
//

#ifndef LAB3_HEX_H
#define LAB3_HEX_H
#include <iostream>

namespace hexmath {
    static constexpr int MAX_NUM_LEN = 16;
    static constexpr char Plus = 0;
    static constexpr char Minus = 1;

    class Hex;

    Hex Add(const Hex& x_, const Hex& y_);
    Hex Sub(const Hex& x_, const Hex& y_);
    bool isGreater(const Hex& x, const Hex& y) noexcept;
    bool isLess(const Hex& x, const Hex& y) noexcept;
    bool isGreaterOrEquals(const Hex& x, const Hex& y) noexcept;
    bool isLessOrEquals(const Hex& x, const Hex& y) noexcept;
    bool Equals(const Hex& x, const Hex& y) noexcept;
    bool notEquals(const Hex& x, const Hex& y) noexcept;
    std::istream& input(std::istream& is, Hex& x);
    std::ostream& print(std::ostream& os, const Hex& x) noexcept;

    class Hex {
    private:
        char sign_;
        char num_[MAX_NUM_LEN];
        Hex Complementary() const noexcept;
    public:
        Hex() noexcept { this->setNull();};
        explicit Hex(long long value);
        Hex(const Hex& cp);
        Hex(const char *num);
        Hex& setNull() noexcept;
        void getDigits(char * buf, int buf_len) const {
            if (buf == nullptr) {
                throw std::invalid_argument("Buf is a nullptr.");
            }
            int i = 0, j = 0;
            while (num_[i] == '\0' && i < MAX_NUM_LEN) i++;
            for (; i < MAX_NUM_LEN && j < (buf_len - 1); i++) {
                buf[j++] = num_[i];
            }
            buf[j] = '\0';
        }
        char getSign() const noexcept { return sign_;}

        bool isEven() const noexcept {
            return !(int(num_[MAX_NUM_LEN - 1]) % 2);
        }

        Hex& BitShiftLeft(unsigned int bias);
        Hex& BitShiftRight(unsigned int bias);

        friend Hex hexmath::Add(const Hex& x_, const Hex& y_);
        friend Hex hexmath::Sub(const Hex& x_, const Hex& y_);
        friend bool hexmath::isGreater(const Hex& x, const Hex& y) noexcept;
        friend bool hexmath::isLess(const Hex& x, const Hex& y) noexcept;
        friend bool hexmath::isGreaterOrEquals(const Hex& x, const Hex& y) noexcept;
        friend bool hexmath::isLessOrEquals(const Hex& x, const Hex& y) noexcept;
        friend bool hexmath::Equals(const Hex& x, const Hex& y) noexcept;
        friend bool hexmath::notEquals(const Hex& x, const Hex& y) noexcept;
        friend std::istream& hexmath::input(std::istream& is, Hex& x);
        friend std::ostream& hexmath::print(std::ostream& os, const Hex& x) noexcept;
    };

    inline bool isHexLetter(const char x) noexcept {
        return (abs(x - '0') < 10 || abs(x - 'A') < 6);
    }
}

#endif //LAB3_HEX_H
