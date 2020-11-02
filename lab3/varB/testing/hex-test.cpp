//
// Created by CopiedWonder on 20.10.2020.
//
#include <cmath>
#include "gtest/gtest.h"
#include "Hex.h"

constexpr int TEST_F_CLASSES = 10;

inline int hash(int i) {
    return  (TEST_F_CLASSES * 193 + i * 37) %
            int(fmin(std::numeric_limits<int>::max(), pow(2, hexmath::MAX_NUM_LEN)));
}

char* genStr(char t, int len, bool negative = false) {
    char *res;
    int start = 0;
    if (negative) start++;
    res = new char[len + 1 + start];
    if (negative) res[0] = '-';
    for (int i = start; i < len + start; i++) {
        res[i] = t;
    }
    res[len + start] = '\0';
    return res;
}


bool cmp(const char *x, const char *y) {
    int i = 0;
    while (x[i] != 0 || y[i] != 0) {
        if (x[i] != y[i]) return false;
        i++;
    }
    return x[i] == y[i];
}
namespace hexmath {
class HexTest: public ::testing::Test
    {
    protected:
        Hex* hf[TEST_F_CLASSES];
        void SetUp() override {
            for (int i = 0; i < TEST_F_CLASSES; i++)
                hf[i] = new Hex(hash(i));
        }
        void TearDown() override {
            for (auto & h : hf) {
                delete h;
            }
            for (int i = 0; i < TEST_F_CLASSES; i++) {
                hf[i] = new Hex();
            }
        }
    };

    TEST(HexTesting, ConstructorFromChar) {
        // Check for normal numbers
        Hex a("A34");
        EXPECT_EQ(a.getSign(), hexmath::Plus);
        char tmp[MAX_NUM_LEN + 1];
        a.getDigits(tmp, MAX_NUM_LEN + 1);
        EXPECT_TRUE(cmp(tmp, "\x0A\x03\x04"));
        Hex b("-534");
        EXPECT_EQ(b.getSign(), hexmath::Minus);
        b.getDigits(tmp, MAX_NUM_LEN + 1);
        EXPECT_TRUE(cmp(tmp, "\x05\x03\x04"));
        Hex c("+432F");
        EXPECT_EQ(c.getSign(), hexmath::Plus);
        c.getDigits(tmp, MAX_NUM_LEN + 1);
        EXPECT_TRUE(cmp(tmp, "\x04\x03\x02\x0F"));
        // Check numbers with leading zeroes
        Hex d("00234");
        EXPECT_EQ(d.getSign(), hexmath::Plus);
        d.getDigits(tmp, MAX_NUM_LEN + 1);
        EXPECT_TRUE(cmp(tmp, "\x02\x03\x04"));
        Hex f("-00043");
        EXPECT_EQ(f.getSign(), hexmath::Minus);
        f.getDigits(tmp, MAX_NUM_LEN + 1);
        EXPECT_TRUE(cmp(tmp, "\x04\x03"));
        Hex e("+00AB7");
        EXPECT_EQ(e.getSign(), hexmath::Plus);
        e.getDigits(tmp, MAX_NUM_LEN + 1);
        EXPECT_TRUE(cmp(tmp, "\x0A\x0B\x07"));
        Hex z("-");
        EXPECT_EQ(z.getSign(), hexmath::Plus);
        z.getDigits(tmp, MAX_NUM_LEN + 1);
        EXPECT_TRUE(cmp(tmp, ""));
        EXPECT_THROW(Hex zz(""), std::out_of_range);
        Hex zzz("+");
        EXPECT_EQ(zzz.getSign(), hexmath::Plus);
        zzz.getDigits(tmp, MAX_NUM_LEN + 1);
        EXPECT_TRUE(cmp(tmp, ""));

        EXPECT_THROW(Hex("UDKF"), std::invalid_argument);
        EXPECT_THROW(Hex(genStr('A', 17)), std::out_of_range);
    }

    TEST(HexTesting, ConstructorFromInt) {
        Hex a(1345);
        EXPECT_EQ(a.getSign(), hexmath::Plus);
        char tmp[MAX_NUM_LEN + 1];
        a.getDigits(tmp, MAX_NUM_LEN + 1);
        EXPECT_TRUE(cmp(tmp, "\x5\x4\x1"));
        Hex b(-2000);
        EXPECT_EQ(b.getSign(), hexmath::Minus);
        b.getDigits(tmp, MAX_NUM_LEN + 1);
        EXPECT_TRUE(cmp(tmp, "\x7\xD\x0O"));
        Hex c(+0);
        EXPECT_EQ(c.getSign(), hexmath::Plus);
        c.getDigits(tmp, MAX_NUM_LEN + 1);
        EXPECT_TRUE(cmp(tmp, ""));
    }

    TEST(HexTesting, DefaultConstructor) {
        Hex a;
        EXPECT_EQ(a.getSign(), hexmath::Plus);
        char tmp[MAX_NUM_LEN + 1];
        a.getDigits(tmp, MAX_NUM_LEN + 1);
        EXPECT_TRUE(cmp(tmp, ""));
    }
    TEST_F(HexTest, HexCompare) {
        for (int i = 0; i < TEST_F_CLASSES; i += 2) {
            EXPECT_EQ(hash(i) > hash(i + 1), *hf[i] > *hf[i+1]);
            EXPECT_EQ(hash(i) < hash(i + 1), *hf[i] < *hf[i+1]);
            EXPECT_EQ(hash(i) >= hash(i + 1), *hf[i] >= *hf[i+1]);
            EXPECT_EQ(hash(i) <= hash(i + 1), *hf[i] <= *hf[i+1]);
            EXPECT_EQ(hash(i) == hash(i + 1), *hf[i] == *hf[i+1]);
            EXPECT_EQ(hash(i) != hash(i + 1), *hf[i] != *hf[i+1]);
        }
    }
    TEST_F(HexTest, HexIsEven) {
        for (int i = 0; i < TEST_F_CLASSES; i++) {
            EXPECT_EQ(bool((hash(i) + 1) % 2), hf[i]->isEven());
        }
    }

    TEST(HexTesting, HexAdd) {
        Hex a("A10");
        Hex b("904");
        Hex c = a + b;
        Hex d = b + a;
        EXPECT_TRUE(c == Hex("1314"));
        EXPECT_TRUE(d == Hex("1314"));
        Hex x("BF0");
        Hex y("-FF20");
        Hex z = x + y;
        Hex u = y + x;
        EXPECT_TRUE(z == Hex("-F330"));
        EXPECT_TRUE(u == Hex("-F330"));
        Hex t("-132");
        Hex v("-F20");
        Hex k = t + v;
        Hex l = v + t;
        EXPECT_TRUE(k == Hex("-1052"));
        EXPECT_TRUE(l == Hex("-1052"));
        // overflow check
        Hex i(genStr('F', MAX_NUM_LEN));
        Hex j("2");
        EXPECT_THROW(Hex m = i + j, std::overflow_error);
    }

    TEST(HexTesting, HexSub) {
        Hex a("A10");
        Hex b("904");
        Hex c = a - b;
        Hex d = b - a;
        EXPECT_TRUE(c == Hex("10C"));
        EXPECT_TRUE(d == Hex("-10C"));
        Hex x("BF0");
        Hex y("-FF20");
        Hex z = x - y;
        Hex u = y - x;
        EXPECT_TRUE(z == Hex("10B10"));
        EXPECT_TRUE(u == Hex("-10B10"));
        Hex t("-132");
        Hex v("-F20");
        Hex k = t - v;
        Hex l = v - t;
        EXPECT_TRUE(k == Hex("DEE"));
        EXPECT_TRUE(l == Hex("-DEE"));
    }

    TEST(HexTesting, HexBitShift) {
        Hex a("A0");
        EXPECT_TRUE((a <<= 2) == Hex("A000"));
        Hex b("1245");
        EXPECT_TRUE((b >>= 2) == Hex("12"));
        Hex c("-2451");
        EXPECT_TRUE((c <<= 1) == Hex("-24510"));
        Hex d("-8902");
        EXPECT_TRUE((d >>= 1) == Hex("-890"));
        EXPECT_NO_THROW(d <<= -23);
        EXPECT_NO_THROW(d >>= -100);
    }
 }

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}