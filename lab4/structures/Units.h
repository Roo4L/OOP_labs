//
// Created by copiedwonder on 23.11.2020.
//

#ifndef LAB4_UNITS_H
#define LAB4_UNITS_H
#include <string>
#include "cocos2d.h"
#include "Monsters.h"
#include "Map.h"

namespace base_structures {
    static const std::string unit_models[] = {
            "res/units/tower.png",
            "res/units/frozen_tower.png",
            "res/units/poison_tower.png",
            "res/units/exhaust_tower.png",
            "res/units/frozen_trap.png",
            "res/units/poison_trap.png",
            "res/untis/exhaust_trap.png"
    };

    enum EffectType {
        FROZEN,
        POISON,
        EXHAUST
    };
    class Unit{
    public:
        Unit(int x, int y, string sprite_f, int level_ = 0): x_(x), y_(y), level_(level) {
            sprite_ = cocos2d::Sprite::create(sprite_f);
        };
        virtual int Upgrade();
        cocos2d::Sprite* sprite_;
        ~Unit() {
            sprite_->release();
        }
    protected:
        int x_;
        int y_;
        int level_;
    };

    class Tower: public Unit {
    public:
        Tower(int x, int y, string sprite_f = unit_models[0], int level_ = 0): Unit(x, y, sprite_f, level_) {};
        shared_ptr<MagicTower> toMagic();
        shared_ptr<Monster> Attack();
        int Upgrade() override;
    };

    class MagicSignature {
    public:
        MagicSignature(EffectType type, int effect_level = 0): type_(type), effect_level_(effect_level) {};
        virtual int UpgradeEffect();
    protected:
        EffectType type_;
        int effect_level_;
    };

    class MagicTower: public Tower, public MagicSignature {
    public:
        MagicTower(Tower tower, EffectType type): Tower(tower), ;
        shared_ptr<Monster> Attack() override;
    };

    class MagicTrap: public Unit, public MagicSignature {
    public:
        shared_ptr<Monster> Activate();
    };
}

#endif //LAB4_UNITS_H
