//
// Created by CopiedWonder on 29.09.2020.
//
#include <stdexcept>
#include <cmath>
#include "cardioid.h"
#include "math_plane.h"

inline double Rad(double angle) {
    return angle * math_plane::Pi / 180;
}

cardioid::Cardioid::Cardioid(double rad, double angle) {
    if (rad < 0) { throw std::out_of_range("Cardioid radius is negative.");}
    this->rad = rad;
    math_plane::normalise_angle(angle);
    this->angle = angle;
}
double cardioid::Cardioid::getAngle() const {
    return angle;
}

double cardioid::Cardioid::getRad() const {
    return rad;
}

double cardioid::Cardioid::getPolarDistance(double ang) const {
    return 2*rad*(1 - cos(Rad(ang - angle)));
}

math_plane::Point* cardioid::Cardioid::getFarPoints() const {
    math_plane::Point* out_pair;
    out_pair = new math_plane::Point[2];
    out_pair[0] = {this->getPolarDistance(90 + angle), 90. + angle};
    out_pair[1] = {this->getPolarDistance(- 90 + angle), -90. + angle};
    return out_pair;
}

double cardioid::Cardioid::getRadiusOfCurvature(double ang) const {
    return (4 * rad * sqrt(2) * sqrt(1 - cos(Rad(ang - angle)))/ 3);
}

double cardioid::Cardioid::getArea() const {
    return 6 * math_plane::Pi * rad * rad;
}


double cardioid::Cardioid::getCurveLen(double ang) const {
    double l1 = 8 * rad * (1 - cos(Rad(angle / 2)));
    double l2 = 8 * rad * (1 - cos(Rad((ang - angle) / 2)));
    return ang < angle ? (l1 - l2) : (l1 + l2);
}

cardioid::Cardioid& cardioid::Cardioid::setAngle(double new_angle) {
    math_plane::normalise_angle(new_angle);
    angle = new_angle;
    return *this;
}

cardioid::Cardioid& cardioid::Cardioid::setRad(double new_rad) {
    rad = new_rad;
    return *this;
}

cardioid::Cardioid &cardioid::Cardioid::rotate(double rot_angle) {
    angle += rot_angle;
    math_plane::normalise_angle(angle);
    return *this;
}




