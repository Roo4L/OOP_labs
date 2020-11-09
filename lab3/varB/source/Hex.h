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
        int getDigits(char * buf, int buf_len) const{
            if (buf == nullptr) {
                throw std::invalid_argument("Buf is nullptr.");
            }
            int i = 0, j = 0;
            while (num_[i] == '\0' && i < MAX_NUM_LEN) i++;
            for (; i < MAX_NUM_LEN && j < (buf_len - 1); i++) {
                buf[j++] = num_[i];
            }
            buf[j] = '\0';
            return MAX_NUM_LEN - i;
        }
        char getSign() const noexcept { return sign_;}

        bool isEven() const noexcept {
            return !(int(num_[MAX_NUM_LEN - 1]) % 2);
        }

        Hex& operator<<=(unsigned int bias);
        Hex& operator>>=(unsigned int bias);

        friend Hex operator+(const Hex& x_, const Hex& y_);
        friend Hex operator-(const Hex& x_, const Hex& y_);
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
        return (x > 47 && x < 58 || x > 64 && x < 71);
    }
}

#endif //LAB3_HEX_H
