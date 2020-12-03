//
// Created by copiedwonder on 23.11.2020.
//
#include <limits>
#include <memory>
#include "model_types.h"
#include "Units.h"
#include "level_descriptors.h"

namespace base_structures {
    std::shared_ptr<MagicTower> Tower::toMagic(EffectType type) {
        std::shared_ptr <MagicTower> res = std::make_shared<MagicTower>(*this, type);
        this->~Tower();
        return res;
    }

    std::shared_ptr<Monster> Tower::Attack(MonsterTable_& MonsterTable) {
        std::shared_ptr<Monster> m = nullptr;
        int regression = std::numeric_limits<int>::max();
        for (auto it : MonsterTable) {
            int res;
            if (isReachable(*it) && (res = ATTACK_CONDITIONS[int(style_)](*this, *it) < regression) {
                regression = res;
                m = *it;
            }
        }
        m->getDamage(TOWER_DESCR[level_].damage);
        if (!m->isAlive()) {
            ~m;
        }
        return m;
    }

    bool Tower::isUpgradable() noexcept {
        return (TOWER_DESCR.size() != level_ + 1);
    }

    int Tower::Upgrade() {
        if (isUpgradable()) {
            level_++;
            return 0;
        } else return -1;
    }

    std::shared_ptr <Monster> MagicTower::Attack()) {
        shared_ptr<Monster> m = nullptr;
        int regression = std::numeric_limits<int>::max();
        for (auto it : MonsterTable) {
            if (isReachable(*it) && (int res = ATTACK_CONDITIONS[int(style_)](*this, *(*it)) < regression)) {
                regression = res;
                m = *it;
            }
        }
        m->getDamage(TOWER_DESCR[level_].damage);
        if (!m->isAlive) {
            ~m;
        }
        else {
            m->applyDebuf({MAGICTOWER_DESCR[effect_level_].effect_time,
                           MAGICTOWER_DESCR[effect_level_].effect_strength,
                           type_});
        }
        return m;
    }

    bool MagicTrap::isUpgradable() noexcept {
        return (TRAP_DESCR.size() != level_ + 1);
    }

    int MagicTrap::Upgrade() {
        if (isUpgradable()) {
            level_++;
            return 0;
        } else return -1;
    }

    int MagicTrap::Activate() { //TODO rethink return type
        for (auto it : MonsterTable) {
            if (isReachable(*it)) {
                (*it)->getDamage(TRAP_DESCR[level_].damage);
                if (!(*it)->isAlive()) {
                    ~(*it);
                }
                else {
                    (*it)->applyDebuf({MAGICTRAP_DESCR[effect_level_].effect_time, MAGICTRAP_DESCR[effect_level_].effect_strength, type_});
                }
            }
        }
        ~(*this);
        return 0;
    }

}