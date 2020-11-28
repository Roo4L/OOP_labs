//
// Created by copiedwonder on 28.11.2020.
//

#ifndef LAB4_MONSTERS_H
#define LAB4_MONSTERS_H
#include <memory>
#include <list>
#include <string>
#include <utility>
#include "cocos2d.h"
#include "Map.h"

namespace base_structures {
    static const std::string monster_models[] = {
            "res/monsters/week_monster.png",
            "res/monsters/strong_monster.png"
    };

    struct Effect {
        time_t debuf_time;
        float debuf_strength;
    };

    class Monster {
    public:
        Monster(int hp = 100, int speed = 40, std::string sprite_f = monster_models[0]): hp_(hp), speed_(speed) {
            sprite_ = cocos2d::Sprite::create(sprite_f);
        }
        Monster(const Monster& cp);
        MOnster(Monster&& cm);
        Monster& applyDebuf(Effect debuf);
        Monster& getDamage(int damage);
        Monster& Move();
        bool isAlive() const noexcept { return hp_ > 0; };
        ~Monster() {
            sprite_->release();
        }
    private:
        int hp_ = 100;
        int speed_ = 40;
        std::list<std::pair<Effect, time_t>> debufs;
        cocos2d::Sprite* sprite_;
        shared_ptr<Cell> relation;
    };

    typedef std::list<std::shared_ptr<Monster>> MonsterTable;
}


#endif //LAB4_MONSTERS_H
