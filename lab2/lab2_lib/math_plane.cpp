//
// Created by CopiedWonder on 29.09.2020.
//
#include <stdexcept>
#include "math_plane.h"

math_plane::Point::Point(double r, double angle) {
    if (r < 0) { throw std::out_of_range("Polar distance is negative.");}
    this->r = r;
    math_plane::normalise_angle(angle);
    this->angle = angle;
}
