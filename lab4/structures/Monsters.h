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
        time_t effect_time;
        float effect_strength;
        EffectType type;
    };

    enum MonsterModel {
        WEEK,
        STRONG
    };

    struct MonsterDiscriptor {
        int hp;
        int speed;
        int cost;
        MonsterModel model;
    };

    class Monster {
    public:
        Monster(int hp = 100, int speed = 40, int cost = 50, std::string sprite_f = monster_models[0]): hp_(hp), speed_(speed), cost_(cost) {
            sprite_ = cocos2d::Sprite::create(sprite_f);
        }
        Monster(MonsterDiscriptor& disc): Monster(disc.hp, disc.speed, disc.cost, monster_models[int(disc.model)]) {};
        Monster(const Monster& cp);
        Mnster(Monster&& cm);
        int getCost() const noexcept { return cost_;};
        int getHP() const noexcept { return hp_;};
        int getSpeed() const noexcept { return speed_;};
        Monster& setRelation(shared_ptr<Cell> cell);
        shared_ptr<Cell> getRelation() const noexcept { return relation;};
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
        std::list<std::pair<Effect, time_t>> debufs;
        shared_ptr<Cell> relation = nullptr;
    };

    typedef std::list<std::shared_ptr<Monster>> MonsterTable_;

    static MonsterTable_ MonsterTable;
}


#endif //LAB4_MONSTERS_H
