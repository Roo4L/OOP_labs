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

char* genStr(char t = '\1', int len = hexmath::MAX_NUM_LEN + 1) {
    char *res = new char[len];
    for (int i = 0; i < len; i++) {
        res[i] = t;
    }
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

    TEST(HexTest, ConstructorFromChar) {
        // Check for normal numbers
        Hex a("A34");
        EXPECT_EQ(a.getSign(), hexmath::Plus);
        char tmp[MAX_NUM_LEN];
        a.getDigits(tmp);
        EXPECT_TRUE(cmp(tmp, "\x0A\x03\x04"));
        Hex b("-534");
        EXPECT_EQ(b.getSign(), hexmath::Minus);
        b.getDigits(tmp);
        EXPECT_TRUE(cmp(tmp, "\x05\x03\x04"));
        Hex c("+432F");
        EXPECT_EQ(c.getSign(), hexmath::Plus);
        c.getDigits(tmp);
        EXPECT_TRUE(cmp(tmp, "\x04\x03\x02\x0F"));
        // Check numbers with leading zeroes
        Hex d("00234");
        EXPECT_EQ(d.getSign(), hexmath::Plus);
        d.getDigits(tmp);
        EXPECT_TRUE(cmp(tmp, "\x02\x03\x04"));
        Hex f("-00043");
        EXPECT_EQ(f.getSign(), hexmath::Minus);
        f.getDigits(tmp);
        EXPECT_TRUE(cmp(tmp, "\x04\x03"));
        Hex e("+00AB7");
        EXPECT_EQ(e.getSign(), hexmath::Plus);
        e.getDigits(tmp);
        EXPECT_TRUE(cmp(tmp, "\x0A\x0B\x07"));

        EXPECT_THROW(Hex("UDKF"), std::invalid_argument);
        EXPECT_THROW(Hex(genStr()), std::out_of_range);
    }

    TEST(HexTest, ConstructorFromInt) {
        Hex a(1345);
        EXPECT_EQ(a.getSign(), hexmath::Plus);
        char tmp[MAX_NUM_LEN];
        a.getDigits(tmp);
        EXPECT_TRUE(cmp(tmp, "541"));
        Hex b(-2000);
        EXPECT_EQ(b.getSign(), hexmath::Minus);
        b.getDigits(tmp);
        EXPECT_TRUE(cmp(tmp, "7DO"));
        Hex c(+0);
        EXPECT_EQ(c.getSign(), hexmath::Plus);
        c.getDigits(tmp);
        EXPECT_TRUE(cmp(tmp, ""));
    }

    TEST(HexTest, DefaultConstructor) {
        Hex a;
        EXPECT_EQ(a.getSign(), hexmath::Plus);
        char tmp[MAX_NUM_LEN];
        a.getDigits(tmp);
        EXPECT_TRUE(cmp(tmp, ""));
    }
    TEST_F(HexTest, HexCompare) {
        for (int i = 0; i < TEST_F_CLASSES; i += 2) {
            EXPECT_EQ(hash(i) > hash(i + 1), hexmath::isGreater(*hf[i], *hf[i+1]));
            EXPECT_EQ(hash(i) < hash(i + 1), hexmath::isLess(*hf[i], *hf[i+1]));
            EXPECT_EQ(hash(i) >= hash(i + 1), hexmath::isGreaterOrEquals(*hf[i], *hf[i+1]));
            EXPECT_EQ(hash(i) <= hash(i + 1), hexmath::isLessOrEquals(*hf[i], *hf[i+1]));
            EXPECT_EQ(hash(i) == hash(i + 1), hexmath::Equals(*hf[i], *hf[i+1]));
            EXPECT_EQ(hash(i) != hash(i + 1), hexmath::notEquals(*hf[i], *hf[i+1]));
        }
    }
    TEST_F(HexTest, HexIsEven) {
        for (int i = 0; i < TEST_F_CLASSES; i++) {
            EXPECT_EQ(bool((hash(i) + 1) % 2), hf[i]->isEven());
        }
    }

    TEST(HexTest, HexAdd) {
        Hex a("A10");
        Hex b("904");
        Hex c = hexmath::Add(a, b);
        Hex d = hexmath::Add(b, a);
        EXPECT_TRUE(hexmath::Equals(c, Hex("1314")));
        EXPECT_TRUE(hexmath::Equals(d, Hex("1314")));
        Hex x("BF0");
        Hex y("-FF20");
        Hex z = hexmath::Add(x, y);
        Hex u = hexmath::Add(y, x);
        EXPECT_TRUE(hexmath::Equals(z, Hex("-F330")));
        EXPECT_TRUE(hexmath::Equals(u, Hex("-F330")));
        Hex t("-132");
        Hex v("-F20");
        Hex k = hexmath::Add(x, y);
        Hex l = hexmath::Add(y, x);
        EXPECT_TRUE(hexmath::Equals(k, Hex("-141")));
        EXPECT_TRUE(hexmath::Equals(l, Hex("-141")));
        // overflow check
        Hex i(genStr('F', MAX_NUM_LEN));
        Hex j("2");
        Hex m = hexmath::Add(i, j);
    }

    TEST(HexTest, HexSub) {
        Hex a("A10");
        Hex b("904");
        Hex c = hexmath::Sub(a, b);
        Hex d = hexmath::Sub(b, a);
        EXPECT_TRUE(hexmath::Equals(c, Hex("10C")));
        EXPECT_TRUE(hexmath::Equals(d, Hex("-10C")));
        Hex x("BF0");
        Hex y("-FF20");
        Hex z = hexmath::Sub(x, y);
        Hex u = hexmath::Sub(y, x);
        EXPECT_TRUE(hexmath::Equals(z, Hex("10B10")));
        EXPECT_TRUE(hexmath::Equals(u, Hex("-10B10")));
        Hex t("-132");
        Hex v("-F20");
        Hex k = hexmath::Add(x, y);
        Hex l = hexmath::Add(y, x);
        EXPECT_TRUE(hexmath::Equals(k, Hex("DEE")));
        EXPECT_TRUE(hexmath::Equals(l, Hex("-DEE")));
    }

    TEST(HexTest, HexBitShift) {
        Hex a("A0");
        EXPECT_TRUE(hexmath::Equals(a.BitShiftRight(2), Hex("A000")));
        Hex b("1245");
        EXPECT_TRUE(hexmath::Equals(b.BitShiftLeft(2),Hex("12")));
        Hex c("-2451");
        EXPECT_TRUE(hexmath::Equals(c.BitShiftRight(1), Hex("-24510")));
        Hex d("-8902");
        EXPECT_TRUE(hexmath::Equals(d.BitShiftLeft(1), Hex("-890")));
        EXPECT_THROW(d.BitShiftLeft(-23), std::invalid_argument);
        EXPECT_THROW(d.BitShiftRight(-100), std::invalid_argument);
    }
 }

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}