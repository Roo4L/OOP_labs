#include <iostream>
#include "math_plane.h"
#include "cardioid.h"
#include "my_input.h"

int main() {
    std::cout << "Welcome to cardioid class interface. The following commands are present:" << std::endl
              << "1. Create new Cardioid with specific radius and rotation angle." << std::endl
              << "2. Get cardioid radius." << std::endl
              << "3. Get rotation angle." << std::endl
              << "4. Get polar distance of cardioid point with specific angle." << std::endl
              << "5. Get the farthest from cardioid axis points." << std::endl
              << "6. Get radius of curvature." << std::endl
              << "7. Get area surrounded by cardioid." << std::endl
              << "8. Get curve length." << std::endl
              << "9. Set radius of cardioid." << std::endl
              << "10. Set rotation angle." << std::endl
              << "11. Rotate curve." << std::endl
              << "12. Exit." << std::endl;

    int menu_case;
    double rad, angle;
    bool end = false;
    cardioid::Cardioid* curve = nullptr;
    while (!end) {
        if (my_input::getNum(menu_case) == 0) {
            switch(menu_case)
            {
                case 1 :
                {
                    rad = 1;
                    angle = 0;
                    while (my_input::getNum(rad) != 0) {
                        std::cout << "Bad input. Retry." << std::endl;
                    }
                    while (my_input::getNum(angle) != 0) {
                        std::cout << "Bad input. Retry." << std::endl;
                    }
                    delete curve;
                    curve = new cardioid::Cardioid(rad, angle);
                    break;
                }
                case 2:
                    if (curve != nullptr) {
                        std::cout << curve->getRad() << std::endl;
                    }
                    else {
                        std::cout << "Curve not found." << std::endl;
                    }
                    break;
                case 3:
                    if (curve != nullptr) {
                        std::cout << curve->getAngle() << std::endl;
                    }
                    else {
                        std::cout << "Curve not found." << std::endl;
                    }
                    break;
                case 4:
                {
                    if (curve != nullptr) {
                        while (my_input::getNum(angle) != 0) {
                            std::cout << "Bad input. Retry." << std::endl;
                        }
                        std::cout << curve->getPolarDistance(angle) << std::endl;
                    }
                    else {
                        std::cout << "Curve not found." << std::endl;
                    }
                    break;
                }
                case 5:
                {
                    if (curve != nullptr) {
                        auto [p1, p2] = curve->getFarPoints();
                        std::cout << "r = " << p1.r << ", alpha = " << p1.angle << std::endl;
                        std::cout << "r = " << p2.r << ", aplha = " << p2.angle << std::endl;
                    }
                    else {
                        std::cout << "Curve not found." << std::endl;
                    }
                    break;
                }
                case 6:
                {
                    if (curve != nullptr) {
                        while (my_input::getNum(angle) != 0) {
                            std::cout << "Bad input. Retry." << std::endl;
                        }
                        std::cout << curve->getRadiusOfCurvature(angle) << std::endl;
                    }
                    else {
                        std::cout << "Curve not found." << std::endl;
                    }
                    break;
                }
                case 7:
                {
                    if (curve != nullptr) {
                        std::cout << curve->getArea() << std::endl;
                    }
                    else {
                        std::cout << "Curve not found." << std::endl;
                    }
                    break;
                }
                case 8:
                {
                    if (curve != nullptr) {
                        while (my_input::getNum(angle) != 0) {
                            std::cout << "Bad input. Retry." << std::endl;
                        }
                        std::cout << curve->getCurveLen(angle) << std::endl;
                    }
                    else {
                        std::cout << "Curve not found." << std::endl;
                    }
                    break;
                }
                case 9:
                {
                    if (curve != nullptr) {
                        while (my_input::getNum(rad) != 0) {
                            std::cout << "Bad input. Retry." << std::endl;
                        }
                        curve->setRad(rad);
                    }
                    else {
                        std::cout << "Curve not found." << std::endl;
                    }
                    break;
                }
                case 10:
                {
                    if (curve != nullptr) {
                        while (my_input::getNum(angle) != 0) {
                            std::cout << "Bad input. Retry" << std::endl;
                        }
                        curve->setAngle(angle);
                        break;
                    }
                    else {
                        std::cout << "Curve not found." << std::endl;
                    }
                    break;
                }
                case 11:
                {
                    if (curve != nullptr) {
                        while (my_input::getNum(angle) != 0) {
                            std::cout << "Bad input. Retry." << std::endl;
                        }
                        curve->rotate(angle);
                    }
                    else {
                        std::cout << "Curve not found." << std::endl;
                    }
                    break;
                }
                case 12:
                {
                    end = true;
                    break;
                }
                default:
                {
                    std::cout << "Bad operation code. Retry." << std::endl;
                    break;
                }
            }
        }
        else {
            std::cout << "Bad input. Retry." << std::endl;
        }
    }
    return 0;
}
