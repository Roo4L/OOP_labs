//
// Created by copiedwonder on 23.11.2020.
//

#include "Units.h"

std::shared_ptr<MagicTower> Tower::toMagic(EffectType type) {
    std::shared_ptr<MagicTower> res = std::make_shared<MagicTower>(*this, type);
    this->~Tower();
    Map.cell_arr[y][x].setUnit(res);
    return res;
}

std::shared_ptr<Tower> Tower::Attack() {
    ///
}

bool Tower::isUpgradable() {
    return (TOWER_DISCR.size() != level_ + 1);
}

int Tower::Upgrade() {
    if (isUpgradable()) {
        level_++;
        return 0;
    }
    else return -1;
}
