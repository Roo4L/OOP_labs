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
        return m;
    }

    int Dangeon::saveToFile(std::ofstream &os) const {
        os << int(DANGEON);
        for (int i = 0; i < waves.size(); ++i) {
            for (auto& release : waves[i]) {
                os << i;
                os << release.first->getHP() << release.first->getSpeed() << release.first->getCost()
                                                                                    << int(release.first->getModel());
            }
        }
        os << 100;
        return 0;
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
        int celltype;
        int size_x, size_y;
        std::vector <std::pair<int, int>> way_trace;
        if (!save_.eof()) {
            save_ >> size_x >> size_y;
            cell_arr.resize(size_x);
            for (auto &it : cell_arr) {
                it.resize(size_y);
            }
        }
        for (int i = 0; i < size_x; i++) {
            for (int j = 0; j < size_y; j++) {
                if (!save_.eof()) {
                    save_.close();
                    return -1;
                }
                save_ >> celltype;
                switch (celltype) {
                    case DANGEON:
                        cell_arr[i][j] = std::dynamic_pointer_cast<Cell>(std::make_shared<Dangeon>(save_));
                        break;
                    case BASEMENT:
                        cell_arr[i][j] = std::dynamic_pointer_cast<Cell>(std::make_shared<Basement>());
                        break;
                    case ROAD:
                        int x, y;
                        save_ >> x >> y;
                        way_trace.push_back(std::pair<int,int>(i * size_y + j, x * size_y + y));
                        cell_arr[i][j] = std::dynamic_pointer_cast<Cell>(std::make_shared<Road>());
                        break;
                    case CASTLE:
                        int hp, gold;
                        save_ >> hp >> gold;
                        cell_arr[i][j] = std::dynamic_pointer_cast<Cell>(std::make_shared<Castle>(hp, gold));
                        break;
                    default:
                        cell_arr[i][j] = std::dynamic_pointer_cast<Cell>(std::make_shared<Cell>());
                        break;
                }
            }
        }
        for (auto& path : way_trace) {
            std::shared_ptr<Road> source = std::dynamic_pointer_cast<Road>(
                    cell_arr[path.first / size_y][path.first % size_y]);
            source->setNext(std::dynamic_pointer_cast<Road>(cell_arr[path.second / size_y][path.second % size_y]));
        }
        save_.close();
        return 0;
    }

    int Map_::save(std::string filename) const {
        std::ofstream os(filename, std::ios::binary);
        if (!os.is_open())
            return -1;
        os << cell_arr.size();
        os << cell_arr[0].size();
        for (int i = 0; i < cell_arr.size(); i++) {
            for (int j = 0; j < cell_arr[i].size(); j++) {
                os << int(cell_arr[i][j]->getType());
                switch (cell_arr[i][j]->getType()) {
                    case DANGEON:
                        std::dynamic_pointer_cast<Dangeon>(cell_arr[i][j])->saveToFile(os);
                        break;
                    case ROAD: {
                        std::shared_ptr<Road> cell = std::dynamic_pointer_cast<Road>(cell_arr[i][j]);
                        int x, y;
                        if (i > 0 && cell_arr[i - 1][j] == cell->getNext()) {
                            x = i - 1;
                            y = j;
                        } else if (i < cell_arr.size() - 1 && cell_arr[i + 1][j] == cell->getNext()) {
                            x = i + 1;
                            y = j;
                        }
                        if (j > 0 && cell_arr[i][j - 1] == cell->getNext()) {
                            x = i;
                            y = j - 1;
                        } else if (j < cell_arr[0].size() - 1 && cell_arr[i][j + 1] == cell->getNext()) {
                            x = i;
                            y = j + 1;
                        }
                        os << x << y;
                        break;
                    }
                    case CASTLE: {
                        std::shared_ptr<Castle> cell = std::dynamic_pointer_cast<Castle>(cell_arr[i][j]);
                        os << cell->getHp() << cell->getGold();
                        break;
                    }
                    default:
                        break;
                }
            }
        }
        os.close();
        return 0;
    }
}