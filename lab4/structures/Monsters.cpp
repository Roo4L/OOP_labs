//
// Created by copiedwonder on 28.11.2020.
//

#include "Monsters.h"
#include <stdexcept>

namespace base_structures {
    std::istream& operator>>(std::istream& is, MonsterDescriptor& desc) noexcept {
        try {
            int model;
            int hp;
            int speed;
            int cost;
            is >> model >> hp >> speed >> cost;
            desc.model = MonsterModel(model);
            desc.hp = hp;
            desc.speed = speed;
            desc.cost = cost;
        }
        catch (...) {
            is.setstate(std::ios::failbit);
        }
        return is;
    }

    Monster& Monster::setRelation(std::shared_ptr<Cell> cell) {
        if (cell == nullptr) {
            throw std::invalid_argument("Monster can't relate to null cell");
        }

        relation = cell;
        return *this;
    }

    Monster& Monster::applyDebuf(Effect debuf) {
        // check wheither the same debuf is already applied
        for (auto it = debufs.begin(); it != debufs.end(); ++it) {
            if (it->first.type == debuf.type) {
                if (it->first.effect_time > debuf.effect_time || it->first.effect_time +
                                                (std::chrono::steady_clock::now() - it->second) < debuf.effect_time) {
                    debufs.erase(it);
                }
                else
                    return *this;
            }
        }
        // apply debuf
        debufs.push_back({debuf, std::chrono::steady_clock::now()});
        return *this;
    }

    Monster& Monster::getDamage(int damage) {
        hp_ -= damage;
        return *this;
    }

    Monster& Monster::Move() {

    }
}