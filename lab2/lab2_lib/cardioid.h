//
// Created by CopiedWonder on 29.09.2020.
//

#ifndef LAB2_CARDIOID_H
#define LAB2_CARDIOID_H
#include "math_plane.h"
namespace cardioid
{
    class Cardioid
    {
    private:
        double rad;
        double angle;
    public:
        explicit Cardioid(double rad = 1, double angle = 0);
        double getRad() const;
        double getAngle() const;
        double getPolarDistance(double ang) const;
        math_plane::Point* getFarPoints() const;
        double getRadiusOfCurvature(double ang) const;
        double getArea() const;
        double getCurveLen(double ang) const;
        Cardioid& setRad(double new_rad);
        Cardioid& setAngle(double new_angle);
        Cardioid& rotate(double rot_angle);
        ~Cardioid() = default;
    };
}
#endif //LAB2_CARDIOID_H
