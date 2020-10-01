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
        Point(double r = 0, double angle = 0);
        Point& operator=(const Point& p);
        ~Point() =default;
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
