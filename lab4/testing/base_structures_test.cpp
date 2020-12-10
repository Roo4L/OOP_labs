//
// Created by copiedwonder on 05.12.2020.
//

#include "gtest/gtest.h"
#include "base_structures.h"
#include <fstream>
#include <string>

namespace base_structures {
    TEST(MapTest, CellConstructor) {
        Cell a(0, 0);
        EXPECT_EQ(BASE_CELL, a.getType());
        Cell b(0, 0);
        EXPECT_EQ(CASTLE, b.getType());
    }

    TEST(MapTest, CastleConsturctor) {
        Castle a(0, 0);
        EXPECT_EQ(CASTLE, a.getType());
        EXPECT_EQ(200, a.getHp());
        EXPECT_EQ(100, a.getGold());
        Castle b(0, 0, 1304, 1'000);
        EXPECT_EQ(CASTLE, b.getType());
        EXPECT_EQ(1304, a.getHp());
        EXPECT_EQ(1000, a.getGold());
        EXPECT_THROW(Castle c(-4, 1'000), std::invalid_argument);
    }

    TEST(MapTest, CastleIncome) {
        Castle a(0, 0);
        Monster m;
        a.income(m);
        EXPECT_EQ(CASTLE, a.getType());
        EXPECT_EQ(150, a.getGold());
        EXPECT_EQ(200, a.getHp());
        Castle b(0, 0, 1304, 1'000);
        Monster mm(100, 100, 120, WEEK);
        b.income(mm);
        EXPECT_EQ(CASTLE, b.getType());
        EXPECT_EQ(1304, a.getHp());
        EXPECT_EQ(1120, a.getGold());
    }

    TEST(MapTest, CastleDoDamage) {
        Castle a(0, 0);
        Monster m;
        a.doDamage(m);
        EXPECT_EQ(CASTLE, a.getType());
        EXPECT_EQ(150, a.getGold());
        EXPECT_EQ(100, a.getHp());
        Castle b(0, 0, 1304, 1'000);
        Monster mm(1100, 100, 120, WEEK);
        b.doDamage(mm);
        EXPECT_EQ(CASTLE, b.getType());
        EXPECT_EQ(204, a.getHp());
        EXPECT_EQ(1000, a.getGold());
        Castle c(0, 0, 50, 50);
        Monster mmm(100, 100, 100, WEEK);
        c.doDamage(mmm);
        EXPECT_EQ(CASTLE, b.getType());
        EXPECT_EQ(-50, a.getHp());
        EXPECT_EQ(50, a.getGold());
    }

    TEST(MapTest, DangeonConstructor) {
        Dangeon a(0, 0);
        EXPECT_EQ(DANGEON, a.getType());
        EXPECT_EQ(-1, a.getCurWaveNum());
        EXPECT_TRUE(a.isActive());
        std::fstream buf("DangeonConstructorTest.txt", std::ios::binary);
        buf << 100 << 100 << 100 << WEEK;
        Dangeon b(0, 0, buf);
        EXPECT_EQ(DANGEON, b.getType());
        EXPECT_EQ(-1, b.getCurWaveNum());
        EXPECT_TRUE(b.isActive());
        //TODO Check monster in waves
    }

    TEST(MapTest, DangeonWaves) {

    }

    TEST(MapTest, RoadConstructor) {
        Road a(0, 0);
        EXPECT_EQ(ROAD, a.getType());
        EXPECT_EQ(nullptr, a.getNext());
        EXPECT_EQ(cocos2d::Vec2(0, 0), a.getDirection());
        std::shared_ptr<Road> n = std::make_shared<Road>(1, 0);
        Road b(0, 0, n);
        EXPECT_EQ(DANGEON, b.getType());
        EXPECT_EQ(n, a.getNext());
        EXPECT_EQ(cocos2d::Vec2(1, 0), a.getDirection());
    }

    TEST(MapTest, RoadSetUnit) {
        Road a(0, 0);
        std::shared_ptr<MagicTrap> u = std::dynamic_pointer_cast<MagicTrap>(a.setUnit());
        EXPECT_EQ(ROAD, a.getType());
        EXPECT_EQ(nullptr, a.getNext());
        EXPECT_EQ(cocos2d::Vec2(0, 0), a.getDirection());
        EXPECT_TRUE(a.isBusy());
        EXPECT_EQ(u->getType(), FROZEN);
        EXPECT_EQ(u->getEffectLevel(), 0);
        EXPECT_EQ(u->getLevel(), 0);
    }

    TEST(MapTest, BasementConstructor) {
        Basement a(0, 0);
        EXPECT_EQ(BASEMENT, a.getType());
    }
    TEST(MapTest, BasementSetUnit) {
        Basement a(0, 0);
        std::shared_ptr<Tower> u = std::dynamic_pointer_cast<Tower>(a.setUnit());
        EXPECT_EQ(BASEMENT, a.getType());
        EXPECT_TRUE(a.isBusy());
        EXPECT_EQ(u->getLevel(), 0);
    }

    TEST(MapTest, MapLoad) {
        Map_ Map;
        Map.load("test_map_1.bin");
        EXPECT_EQ(Map.cell_arr[0][0]->getType(), BASE_CELL);
        EXPECT_EQ(Map.cell_arr[1][0]->getType(), CASTLE);
        EXPECT_EQ(Map.castle->getHp(), 0x100);
        EXPECT_EQ(Map.castle->getGold(), 0x10);
    }
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}