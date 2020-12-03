//
// Created by copiedwonder on 23.11.2020.
//

#include <chrono>
#include <cmath>
#include <list>
#include <fstream>
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

    int Map_::load(std::string filename) {
        std::ifstream save_(filename, std::ios::binary);
        if (!save_.is_open())
            return -1;
        CellType celltype;
        int size_x, size_y;
        std::vector <std::pair<int, int>> way_trace;
        if (!save_.eof()) {
            save_ >> size_x >> size_y;
            cell_arr.resize(size_x);
            for (auto &it : cell_arr) {
                it->resize(size_y);
            }
        }
        for (int i = 0; i < size_x; i++) {
            for (int j = 0; j < size_y; j++) {
                if (!save_.is_open())
                    return -1;
                save_ >> celltype;
                switch (celltype) {
                    case DANGEON:
                        cell_arr[i][j] = std::make_shared<Dangeon>(save_)
                        break;
                    case BASEMENT:
                        cell_arr[i][j] = std::make_shared<Basement>();
                        break;
                    case ROAD:
                        int x, y;
                        save_ >> x >> y;
                        way_trac.push_back({x, y});
                        cell_arr[i][j] = std::make_shared<Road>();
                        break;
                    case CASTLE:
                        int hp, gold;
                        save_ >> hp >> gold;
                        cell_arr[i][j] = std::make_shared<Castle>(hp, gold);
                        break;
                    default:
                        cell_arr[i][j] = std::make_shared<Cell>();
                        break;
                }
            }
        }

    }
}