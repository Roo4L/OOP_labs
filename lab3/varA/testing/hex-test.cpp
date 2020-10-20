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
    TEST(HexTest, HexConstructor) {
        EXPECT_THROW(Hex(getChar()), std::out_of_range);
        EXPECT_NO_THROW(Hex(hash(1)));
    }
    /*
    TEST(HexTest, HexBitShift) {
        Hex a("A0");
        EXPECT_EQ(a.BitShiftRight(2), Hex("A000"));
        Hex b("1245");
        EXPECT_EQ(b.BitShiftLeft(2),Hex("12"));
        Hex c("-2451");
        EXPECT_EQ(c.BitShiftRight(1), Hex("-24510"));
        Hex d("-8902");
        EXPECT_EQ(d.BitShiftLeft(1), Hex("-890"));
        EXPECT_THROW(d.BitShiftLeft(-23), std::invalid_argument);
        EXPECT_THROW(d.BitShiftRight(-100), std::invalid_argument);
    }
     */
 }

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}