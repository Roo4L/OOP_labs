//
// Created by copiedwonder on 23.11.2020.
//

#include <ctime>
#include <math>
#include "Map.h"
#include "cocos2d.h"
#include "GameScene.h"

namespace base_structures {
    Cell::Cell(const Cell &cp) {
        type_ = cp.type_;
        sprite_ = cocos2d::Sprite::createWithTexture(cp.sprite_->getTexture());
    }

    Castle::Castle(const Castle &cp) {
        type_ = cp.type_;
        sprite_ = cocos2d::Sprite::createWithTexture(cp.sprite_->getTexture());
        hp_ = cp.hp_;
        gold_ = cp.gold_;
    }

    void Castle::income(const Monster& frag) noexcept {
        gold_ += frag.getCost();
    }

    void Castle::doDamage(const Monster &frag) noexcept {
        hp_ -= frag.getHP();
    }

    shared_ptr<Monster> Dangeon::ReleaseMonster() {
        while (waves[cur_wave_it].front.second time_t <= time() - wave_start) {
            std::shared_ptr<Monster> m = move(waves[cur_wave_it].front.first);
            waves[cur_wave_it].pop_front();
            //add monster on map
            m->setRelation(next);
            m->sprite_->setPosition(sprite_->getPositionX(), sprite_->getPositioinY());
            game_scene->addChile(m->sprite());
            MonsterTable.push_back(m);
        }
    }

    cocos2d::Vec2 Road::getDirection() {
        float x = next->sprite_->getPositionX() - sprite_->getPositionX();
        float y = next->sprite_->getPositionY() - sprite_->getPositionY();
        return cocos2d::Vec2(x / abs(x) , y / abs(y));
    }

    shared_ptr<Unit> Road::setUnit(EffectType type) {
        if (unit_ != nullptr) throw std::logic_error("Can't set unit to non-empty cell.");
        unit_ = make_shared<MagicTrap>(sprite_->getPositionX() / sprite_->getContentSize().x,
                            sprite_->getPositionY() / sprite_->getContentSize().y,
                            type);
        UnitTable.traps.push_back(unit_);
        return unit_;
    }

    shared_ptr<Unit> Road::setUnit(shared_ptr<Unit> unit) {
        if (unit_ != nullptr) throw std::logic_error("Can't set unit to non-empty cell.");
        unit_ = unit;
        UnitTable.traps.push_back(unit_);
        return unit_;
    }

    shared_ptr<Unit> Basement::setUnit() {
        if (unit_ != nullptr) throw std::logic_error("Can't set unit to non-empty cell.");
        unit_ = make_shared<Tower>(sprite_->getPositionX() / sprite_->getContentSize().x,
                            sprite_->getPositionY() / sprite_->getContentSize().y,
                            unit_models[0]);
        UnitTable.towers.push_back(unit_);
        return unit_;
    }

    shared_ptr<Unit> Basement::setUnit(shared_ptr<Unit> unit) {
        if (unit_ != nullptr) throw std::logic_error("Can't set unit to non-empty cell.");
        unit_ = unit;
        UnitTable.towers.push_back(unit_);
        return unit_;
    }

}