//
// Created by CopiedWonder on 01.10.2020.
//
#include <cmath>
#include "gtest/gtest.h"
#include "cardioid.h"
#include "math_plane.h"

const int TEST_F_CLASSES = 360;
const int TEST_CLASSES = 720;
const int ANG_STEP = 90;

namespace cardioid {
    class  CardioidTest: public ::testing::Test
    {
    protected:
        Cardioid* cf[TEST_F_CLASSES];
        void SetUp() override{
            for (int i = 0; i < TEST_F_CLASSES; i++) {
                cf[i] = new Cardioid(i, i);
            }
        }
        void TearDown() override{
            for (auto & p : cf) {
                delete p;
            }
        }
    };

    Cardioid* c[TEST_CLASSES];

    TEST_F (CardioidTest, ContructorCheck) {
        for (int i = 0; i < TEST_CLASSES; i++) {
            EXPECT_ANY_THROW(new Cardioid(-(i+1), i));
        }
        for (int i = 0; i < TEST_CLASSES; i++) {
            EXPECT_NO_THROW(c[i] = new Cardioid(i, i));
        }
    }

    TEST_F(CardioidTest, IsTrueInitially) {
        double ang;
        for (int i = 0; i < TEST_CLASSES; i++) {
            ang = i;
            math_plane::normalise_angle(ang);
            EXPECT_DOUBLE_EQ(c[i]->getRad(), i);
            EXPECT_DOUBLE_EQ(c[i]->getAngle(), ang);
            EXPECT_NE(c[i]->getRad(), i + 2);
            EXPECT_NE(c[i]->getAngle(), i + 2);
        }
        for (int i = 0; i < TEST_F_CLASSES; i++) {
            ang = i;
            math_plane::normalise_angle(ang);
            EXPECT_DOUBLE_EQ(cf[i]->getRad(), i);
            EXPECT_DOUBLE_EQ(cf[i]->getAngle(), ang);
            EXPECT_NE(cf[i]->getRad(), i + 2);
            EXPECT_NE(cf[i]->getAngle(), i + 2);
        }
    }

    TEST_F(CardioidTest, SettersWork) {
        double ang;
        for (int i = 0; i< TEST_CLASSES; i++) {
            ang = 1000 + i;
            math_plane::normalise_angle(ang);
            EXPECT_DOUBLE_EQ(c[i]->setRad(1000+i).getRad(), 1000+i);
            EXPECT_DOUBLE_EQ(c[i]->setAngle(1000+i).getAngle(), ang);
            EXPECT_NE(c[i]->setRad(1000+i).getRad(), i);
            EXPECT_NE(c[i]->setAngle(1000+i).getAngle(), i);
        }
        for (int i = 0; i< TEST_F_CLASSES; i++) {
            ang = 1000 + i;
            math_plane::normalise_angle(ang);
            EXPECT_DOUBLE_EQ(cf[i]->setRad(1000+i).getRad(), 1000+i);
            EXPECT_DOUBLE_EQ(cf[i]->setAngle(1000+i).getAngle(), ang);
            EXPECT_NE(cf[i]->setRad(1000+i).getRad(), i);
            EXPECT_NE(cf[i]->setAngle(1000+i).getAngle(), i);
        }
    }
    TEST_F(CardioidTest, PolarDistanceCheck) {
        double rad, angle;
        for (auto & p : c) {
            rad = p->getRad();
            angle = p->getAngle();
            for (int ang = 0; ang < 360; ang += ANG_STEP) {
                EXPECT_DOUBLE_EQ(p->getPolarDistance(ang), 2 * rad * (1 - cos(cardioid::Rad(ang - angle))));
                EXPECT_NE(p->getPolarDistance(ang), -1);
            }
        }
        for (auto & p : cf) {
            rad = p->getRad();
            angle = p->getAngle();
            for (int ang = 0; ang < 360; ang += ANG_STEP) {
                EXPECT_DOUBLE_EQ(p->getPolarDistance(ang), 2 * rad * (1 - cos(cardioid::Rad(ang - angle))));
                EXPECT_NE(p->getPolarDistance(ang), -1);
            }
        }
    }

    TEST_F(CardioidTest, FarPointsCheck) {
        for (auto & i : cf) {
            auto [p1, p2] = i->getFarPoints();
            EXPECT_DOUBLE_EQ(p1.r, i->getPolarDistance(90 + i->getAngle()));
            EXPECT_DOUBLE_EQ(p1.angle, (i->getAngle() + 90) >= 360 ? i->getAngle() - 270 : i->getAngle() + 90);
            EXPECT_DOUBLE_EQ(p2.r, i->getPolarDistance(-90 + i->getAngle()));
            EXPECT_DOUBLE_EQ(p2.angle, (i->getAngle() - 90) >= 0 ? i->getAngle() - 90 : i->getAngle() + 270);
        }
        for (auto & i : c) {
            auto[p1, p2] = i->getFarPoints();
            EXPECT_DOUBLE_EQ(p1.r, i->getPolarDistance(90 + i->getAngle()));
            EXPECT_DOUBLE_EQ(p1.angle, (i->getAngle() + 90) >= 360 ? i->getAngle() - 270 : i->getAngle() + 90);
            EXPECT_DOUBLE_EQ(p2.r, i->getPolarDistance(-90 + i->getAngle()));
            EXPECT_DOUBLE_EQ(p2.angle, (i->getAngle() - 90) >= 0 ? i->getAngle() - 90 : i->getAngle() + 270);
        }
    }

    TEST_F(CardioidTest, RadiusOfCurvatureCheck) {
        double rad, angle;
        for (auto & i : c) {
            rad = i->getRad();
            angle = i->getAngle();
            for (int ang = 0; ang < 360; ang += ANG_STEP) {
                EXPECT_DOUBLE_EQ(i->getRadiusOfCurvature(ang), 4 * rad * sqrt(2) * sqrt(1 - cos(Rad(ang - angle)))/ 3);
            }
        }
        for (auto & i : cf) {
            rad = i->getRad();
            angle = i->getAngle();
            for (int ang = 0; ang < 360; ang += ANG_STEP) {
                EXPECT_DOUBLE_EQ(i->getRadiusOfCurvature(ang), 4 * rad * sqrt(2) * sqrt(1 - cos(Rad(ang - angle)))/ 3);
            }
        }
    }
    TEST_F(CardioidTest, CurveLenCheck) {
        double rad, angle;
        for (auto & i : c) {
            rad = i->getRad();
            angle = i->getAngle();
            for (int ang = 0; ang < 360; ang += ANG_STEP) {
                EXPECT_DOUBLE_EQ(i->getRadiusOfCurvature(ang), 4 * rad * sqrt(2) * sqrt(1 - cos(Rad(ang - angle)))/ 3);
            }
        }
        for (auto & i : cf) {
            rad = i->getRad();
            angle = i->getAngle();
            for (int ang = 0; ang < 360; ang += ANG_STEP) {
                EXPECT_DOUBLE_EQ(i->getRadiusOfCurvature(ang), 4 * rad * sqrt(2) * sqrt(1 - cos(Rad(ang - angle)))/ 3);
            }
        }
    }
    TEST_F(CardioidTest, RotateCheck) {
        double angle;
        for (auto & i : cf) {
            angle = i->getAngle();
            for (int ang = 0; ang < 360; ang += ANG_STEP) {
                angle += ang;
                math_plane::normalise_angle(angle);
                i->rotate(ang);
                EXPECT_DOUBLE_EQ(i->getAngle(), angle);
            }
        }
        for (auto & i : c) {
            angle = i->getAngle();
            for (int ang = 0; ang < 360; ang += ANG_STEP) {
                angle += ang;
                math_plane::normalise_angle(angle);
                i->rotate(ang);
                EXPECT_DOUBLE_EQ(i->getAngle(), angle);
            }
        }
    }

    TEST_F(CardioidTest, DistructorCheck) {
        for (auto & i : c) {
            EXPECT_NO_THROW(delete i);
        }
    }
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}