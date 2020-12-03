//
// Created by copiedwonder on 28.11.2020.
//

#ifndef LAB4_MONSTERS_H
#define LAB4_MONSTERS_H
#include <memory>
#include <list>
#include <string>
#include <utility>
#include <iostream>
#include "cocos2d.h"
#include "Map.h"
#include "Units.h"

namespace base_structures {
    static const std::string monster_models[] = {
            "res/monsters/week_monster.png",
            "res/monsters/strong_monster.png"
    };

    class Monster {
    public:
        Monster(std::string sprite_f = monster_models[0]) {
            sprite_ = cocos2d::Sprite::create(sprite_f);
        }
        Monster(int hp, int speed, int cost, std::string sprite_f = monster_models[0]): hp_(hp), speed_(speed), cost_(cost) {
            sprite_ = cocos2d::Sprite::create(sprite_f);
        }
        Monster(MonsterDescriptor& disc): Monster(disc.hp, disc.speed, disc.cost, monster_models[int(disc.model)]), model_(disc.model) {};
        Monster(const Monster& cp);
        Monster(Monster&& cm);
        int getCost() const noexcept { return cost_;};
        int getHP() const noexcept { return hp_;};
        int getSpeed() const noexcept { return speed_;};
        MonsterModel getModel() const noexcept { return model_;};
        Monster& setRelation(std::shared_ptr<Cell> cell);
        std::shared_ptr<Cell> getRelation() const noexcept { return relation;};
        Monster& applyDebuf(Effect debuf);
        Monster& getDamage(int damage);
        Monster& Move();
        bool isAlive() const noexcept { return hp_ > 0; };
        ~Monster() {
            sprite_->release();
        }
        cocos2d::Sprite* sprite_;
    private:
        int hp_ = 100;
        int speed_ = 40;
        int cost_ = 50;
        MonsterModel model_ = MonsterModel::WEEK;
        std::list<std::pair<Effect, std::chrono::time_point<std::chrono::steady_clock>>> debufs;
        std::shared_ptr<Cell> relation = nullptr;
    };
}


#endif //LAB4_MONSTERS_H
