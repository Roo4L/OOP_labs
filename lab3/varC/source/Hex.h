//
// Created by CopiedWonder on 06.10.2020.
//

#ifndef LAB3_HEX_H
#define LAB3_HEX_H
#include <iostream>

namespace hexmath {
    static constexpr char Plus = 0;
    static constexpr char Minus = 1;

    class Hex {
    private:
        char sign_ = Plus;
        char* num_ = nullptr;
        int len_ = 0;
        Hex Complementary() const noexcept;
        Hex& toComplementary() noexcept;
        Hex& Truncate() noexcept;
    public:
        Hex() = default;
        explicit Hex(long long value) noexcept;
        Hex(const Hex& cp);
        Hex(Hex&& cp) noexcept;
        explicit Hex(const char *num);
        Hex& setNull() noexcept;
        void getDigits(char * buf, int buf_len) const{
            if (buf == nullptr) {
                throw std::invalid_argument("Buf is nullptr.");
            }
            int i = len_ - 1, j = 0;
            while (num_[i] == '\0' && i >= 0) i--;
            for (; i >= 0 && j < (buf_len - 1); i++) {
                buf[j++] = num_[i];
            }
            buf[j] = '\0';
        }
        char getSign() const noexcept { return sign_;}

        bool isEven() const noexcept {
            return !(int(num_[0]) % 2);
        }

        Hex& operator<<=(unsigned int bias) noexcept;
        Hex& operator>>=(unsigned int bias) noexcept;

        Hex& operator=(const Hex& y) noexcept;
        Hex& operator=(Hex&& y) noexcept;
        friend Hex operator""_H(const char* num);

        friend Hex operator+(const Hex& x_, const Hex& y_) noexcept;
        friend Hex operator-(const Hex& x_, const Hex& y_) noexcept;
        friend Hex& operator+=(Hex& x_, const Hex& y_) noexcept;
        friend Hex& operator-=(Hex& x_, const Hex& y_) noexcept;

        friend bool operator>(const Hex& x, const Hex& y) noexcept;
        friend bool operator<(const Hex& x, const Hex& y) noexcept;
        friend bool operator>=(const Hex& x, const Hex& y) noexcept;
        friend bool operator<=(const Hex& x, const Hex& y) noexcept;
        friend bool operator==(const Hex& x, const Hex& y) noexcept;
        friend bool operator!=(const Hex& x, const Hex& y) noexcept;

        friend std::istream& operator>>(std::istream& is, Hex& x) noexcept;
        friend std::ostream& operator<<(std::ostream& os, const Hex& x) noexcept;
    };

    inline bool isHexLetter(const char x) noexcept {
        return (x > 47 && x < 58 || x > 64 && x < 71);;
    }
}

#endif //LAB3_HEX_H
