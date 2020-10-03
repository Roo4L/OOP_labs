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
        Cardioid(): rad{1}, angle{0} {};
        Cardioid(double rad, double angle = 0);
        double getRad() const {return rad;};
        double getAngle() const { return angle;};
        double getPolarDistance(double ang) const;
        std::pair<math_plane::Point, math_plane::Point> getFarPoints() const;
        double getRadiusOfCurvature(double ang) const;
        double getArea() const { return 6 * math_plane::Pi * rad * rad;};
        double getCurveLen(double ang) const;
        Cardioid& setRad(double new_rad);
        Cardioid& setAngle(double new_angle);
        Cardioid& rotate(double rot_angle);
        ~Cardioid() = default;
    };
}
#endif //LAB2_CARDIOID_H
