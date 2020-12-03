//
// Created by copiedwonder on 23.11.2020.
//

#include <chrono>
#include <cmath>
#include <list>
#include "Map.h"
#include "cocos2d.h"

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

    std::shared_ptr<Monster> Dangeon::ReleaseMonster() {
        std::shared_ptr<Monster> m = move(waves[cur_wave_it].front().first);
        waves[cur_wave_it].pop_front();
        //add monster on map
        m->setRelation(next);
        m->sprite_->setPosition(sprite_->getPositionX(), sprite_->getPositionY());
        MonsterTable.push_back(m);
        return m;
    }

    cocos2d::Vec2 Road::getDirection() {
        float x = next->sprite_->getPositionX() - sprite_->getPositionX();
        float y = next->sprite_->getPositionY() - sprite_->getPositionY();
        return cocos2d::Vec2(x / abs(x) , y / abs(y));
    }

    std::shared_ptr<Unit> Road::setUnit(EffectType type) {
        if (unit_ != nullptr) throw std::logic_error("Can't set unit to non-empty cell.");
        unit_ = std::make_shared<MagicTrap>(sprite_->getPositionX() / sprite_->getContentSize().width,
                            sprite_->getPositionY() / sprite_->getContentSize().height,
                            type);
        return unit_;
    }

    std::shared_ptr<Unit> Road::setUnit(std::shared_ptr<Unit> unit) {
        if (unit_ != nullptr) throw std::logic_error("Can't set unit to non-empty cell.");
        unit_ = unit;
        return unit_;
    }

    std::shared_ptr<Unit> Basement::setUnit() {
        if (unit_ != nullptr) throw std::logic_error("Can't set unit to non-empty cell.");
        unit_ = make_shared<Tower>(sprite_->getPositionX() / sprite_->getContentSize().width,
                            sprite_->getPositionY() / sprite_->getContentSize().height,
                            unit_models[0]);
        return unit_;
    }

    std::shared_ptr<Unit> Basement::setUnit(std::shared_ptr<Unit> unit) {
        if (unit_ != nullptr) throw std::logic_error("Can't set unit to non-empty cell.");
        unit_ = unit;
        return unit_;
    }

}