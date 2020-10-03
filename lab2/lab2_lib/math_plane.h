//
// Created by CopiedWonder on 29.09.2020.
//

#ifndef LAB2_MATH_PLANE_H
#define LAB2_MATH_PLANE_H

namespace math_plane
{
    struct Point
    {
        double r;
        double angle;
        Point(): r{0}, angle{0} {};
        Point(double r, double angle);
        Point& operator=(const Point& p) = default;
        ~Point() = default;
    };

    const double Pi = 3.14159;

    inline void normalise_angle(double& angle) {
        while (angle > 0) {
            angle -= 360;
        }
        while (angle < 0) {
            angle += 360;
        }
    }
}
#endif //LAB2_MATH_PLANE_H
