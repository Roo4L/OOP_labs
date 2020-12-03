//
// Created by copiedwonder on 23.11.2020.
//

#ifndef LAB4_UNITS_H
#define LAB4_UNITS_H
#include <string>
#include <memory>
#include "cocos2d.h"
#include "Monsters.h"
#include "Map.h"
#include "level_descriptors.h"

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

    //attack styles

    float closestToCastle(const Tower& t, const Monster& m) {
        shared_ptr<Road> rel = static_pointer_cast<Road> m.getRelation();
        int len = 0;
        while (rel != nullptr) {
            len++;
            rel = rel->getNext();
        }
        return len;
    }
    float closestToTower(const Tower& t, const Monster& m) {
        return t.getDistance(m);
    }
    float weekest(const Tower& t, const Monster& m) {
        return m->getHP();
    }
    float strongest(const Tower& t, const Monster& m) {
        return -m.getHP();
    }
    float fastest(const Tower& t, const Monster& m) {
        return -m.getSpeed();
    }

    int (*ATTACK_CONDITIONS[])(const Tower&, const Monster&) {
        closestToCastle,
        closestToTower,
        weekest,
        strongest,
        fastest
    }
    enum AttackStyle {
        CLOSESTCASTLE,
        CLOSESTTOWER,
        WEEKEST,
        STRONGEST,
        FASTEST
    };

    enum EffectType {
        FROZEN,
        POISON,
        EXHAUST
    };
    class Unit{
    public:
        Unit(int x, int y, std::string sprite_f, int level = 0): x_(x), y_(y), level_ (level) {
            sprite_ = cocos2d::Sprite::create(sprite_f);
        };
        virtual bool isUpgradable();
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
        Tower(const Tower& cp);
        Tower(Tower&& cm);
        Tower(int x, int y, string sprite_f = unit_models[0], int level_ = 0): Unit(x, y, sprite_f, level_) {};
        shared_ptr<MagicTower> toMagic(EffectType type);
        shared_ptr<Monster> Attack();
        bool isUpgradable() override noexcept;
        int Upgrade() override noexcept;
        bool isReachable(const Monster& monster) const noexcept {
            return (monster.sprite_->getPosition().getDistance(sprite_->getPosition()) <= TOWER_DISCR[level_].rad);
        }
        float getDistance(const Monster& monster) const noexcept {
            return (monster.sprite_->getPosition().getDistance(sprite_->getPosition());
        }
        AttackStyle getStyle() const noexcept { return style_;};
        Tower& setStyle(AttackStyle style) noexcept {
            style_ = style;
            return *this;
        }
        ~Tower() {
            std::shared_ptr<Basement> cell = static_pointer_cast<Basement> Map.cell_arr[y][x];
            cell.removeUnit();
            for (auto& it : UnitTable.towers) {
                if ((*it).get() == this) {
                    UnitTable.towers.erase(it);
                }
            }
        }
    protected:
        AttackStyle style_ = CLOSESTCASTLE;
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
        MagicTower(const Tower& tower, EffectType type): Tower(tower), type_(type);
        shared_ptr<Monster> Attack()) override;
    };

    class MagicTrap: public Unit, public MagicSignature {
    public:
        MagicTrap(int x, int y, EffectType type, int effect_level = 0,  int tower_level = 0):
                    Unit(x, y, unit_models[4 + int(type)], tower_level), MagicSignature(type, effect_level) {};
        int Activate();
        bool isUpgradable() override noexcept;
        int Upgrade() override noexcept;
        bool isReachable(const Monster& monster) const noexcept {
            return (monster.sprite_->getPosition().getDistance(sprite_->getPosition()) <= TRAP_DESCR[level_].rad);
        }
        ~MagicTrap() {
            std::shared_ptr <Road> cell = static_pointer_cast<Road> Map.cell_arr[y][x];
            cell.removeUnit();
            for (auto &it : UnitTable.traps) {
                if ((*it).get() == this) {
                    UnitTable.traps.erase(it);
                }
            }
        }
    }
}
#endif //LAB4_UNITS_H
