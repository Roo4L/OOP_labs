//
// Created by CopiedWonder on 29.11.2020.
//

#ifndef LAB4_LEVEL_DISCRIPTORS_H
#define LAB4_LEVEL_DISCRIPTORS_H
#include <vector>
#include <string>
#include <fstream>

namespace base_structures {
    struct UnitLevel {
        float rad;
        float damage;
        int cost;
    };

    struct EffectLevel {
        double effect_time;
        float effect_strength;
    };

    static std::vector<UnitLevel> TOWER_DESCR;
    static std::vector<UnitLevel> TRAP_DESCR;
    static std::vector<EffectLevel> MAGICTOWER_DESCR;
    static std::vector<EffectLevel> MAGICTRAP_DESCR;

    int LoadUnitDescr(std::vector<UnitLevel> &descr, std::string filename) {
        std::ifstream config(filename, std::ios::binary);
        if (!config.is_open()) {
            return -1;
        }
        char *buf = new char[sizeof(UnitLevel)];
        UnitLevel *level = new UnitLevel();
        while (!config.eof()) {
            config.read((char *) level, sizeof(UnitLevel));
            descr.push_back(*level);
        }
        config.close();
        return 0;
    }
}
#endif //LAB4_LEVEL_DISCRIPTORS_H
