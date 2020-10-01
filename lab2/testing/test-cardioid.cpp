//
// Created by CopiedWonder on 01.10.2020.
//
#include "gtest/gtest.h"
#include "cardioid.h"
#include "math_plane.h"

namespace cardioid {
    class  CardioidTest: public ::testing::Test {
    };

    Cardioid c0_(1, 0);
    Cardioid c1_(3.5, 90);
    Cardioid c2_(2, -90);
    Cardioid c3_(2, 180);
    Cardioid c4_(10, 350);

    TEST_F(CardioidTest, IsTrueInitially) {
        EXPECT_EQ(c0_.getRad(), 1);
        EXPECT_EQ(c2_.getAngle(), 270);
    }

    TEST_F(CardioidTest, SettersWork) {
        EXPECT_EQ(c1_.setAngle(180).getAngle(), 180);
        EXPECT_EQ(c1_.setAngle(0).getAngle(), 0);
        EXPECT_EQ(c1_.setAngle(360).getAngle(), 0);
        EXPECT_EQ(c1_.setRad(20).getRad(), 20);
    }
    TEST_F(CardioidTest, PolarDistanceCheck) {
        EXPECT_EQ(c0_.getPolarDistance(0), 0);
        EXPECT_NEAR(c0_.getPolarDistance(180), 4, 0.0001);
        EXPECT_NEAR(c0_.getPolarDistance(360), 0, 0.0001);
        EXPECT_NEAR(c0_.getPolarDistance(270), 2, 0.0001);
    }

    TEST_F(CardioidTest, FarPointsCheck) {
        math_plane::Point* pair;
        pair = c2_.getFarPoints();
        EXPECT_EQ(pair[0].angle, 0);
        EXPECT_EQ(pair[1].angle, 180);
    }

    TEST_F(CardioidTest, RadiusOfCurvatureCheck) {
        EXPECT_NEAR(c3_.getRadiusOfCurvature(0), 5.33333, 0.0001);
    }
    TEST_F(CardioidTest, CurveLenCheck) {
        EXPRECT_NEAR(c4_.getCurveLen())
    }

}
