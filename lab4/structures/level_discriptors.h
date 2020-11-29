//
// Created by CopiedWonder on 29.11.2020.
//

#ifndef LAB4_LEVEL_DISCRIPTORS_H
#define LAB4_LEVEL_DISCRIPTORS_H
#include <vector>

struct UnitLevel {
    float rad;
    float damage;
    int cost;
};

std::vector<UnitLevel> TOWER_DISCR;
std::vector<UnitLevel> TRAP_DISCR;

int LoadUnitDiscr(std::vector<UnitLevel>& discr, std::string filename);

#endif //LAB4_LEVEL_DISCRIPTORS_H
