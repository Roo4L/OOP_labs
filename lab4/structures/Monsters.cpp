//
// Created by copiedwonder on 28.11.2020.
//

#include "Monsters.h"
#include <stdexcept>
Monster& Monster::setRelation(shared_ptr<Cell> cell) {
    if (cell == nullptr) {
        throw std::invalid_argument("Monster can't relate to null cell");
    }

    relation = cell;
    return *this;
}

Monster& Monster::applyDebuf(Effect debuf) {
    // check wheither the same debuf is already applied
    for (auto& it : debufs) {
        if (it->first.type == debuf.type) {
            if (it->first.debuf_time - time() + it->second) < debuf.debuf_time)
                debufs.erase(it);
            else
                return *this;
        }
    }
    // apply debuf
    debufs.push_back({debuf, time()});
    return *this;
}

Monster& Monster::getDamage(int damage) {
    hp_ -= damage;
    return *this;
}

Monster& Monster::Move() {
    //TBD
}

Monster&
