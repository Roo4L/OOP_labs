//
// Created by copiedwonder on 23.11.2020.
//

#ifndef LAB4_MAP_H
#define LAB4_MAP_H
#include <memory>
#include <vector>
#include <pair>
#inlcude "cocos2d.h"
#include "Monsters.h"
#include "graph_params.h"

namespace base_structures {
    enum CellType {
        BASE_CELL,
        CASTLE,
        DANGEON,
        ROAD,
        BASEMENT
    };
    typedef std::vector<std::pair<std::shared_ptr<Monster>, int>> Wave;

    class Cell {
    public:
        Cell(): type_(BASE_CELL) {
            sprite = cocos2d::Sprite::create(cell_sprite[int(CellType)]);
        };
        Cell(const Cell& cp);
        Cell(Cell && cm);
        //TODO constructor from save
        CellType getType() const noexcept { return type_;};
        ~Cell() {
            sprite::release();
        }
    private:
        CellType type_;
        cocos2d::Sprite* sprite;
    };

    class Castle: public Cell {
    public:
        Castle(): type_(CASTLE) {};
        Castle(const Castle& cp);
        Castle(Castle&& cm);
        //TODO constructor from save
        int getHp() const noexcept { return hp_;};
        int getGold() const noexcept { return gold_;};
        void income(const Monster& frag) noexcept;
        void doDamage(const Monster& frag) noexcept;
    private:
        int hp_;
        int gold_;
    };

    class Dangeon: public Cell {
    public:
        Dangeon(): type_(DANGEON) {};
        Dangeon(const Dangeon& cp);
        Dangeon(Dangeon&& cm);
        //TODO constructor from save
        int NextWave();
        int getCurWaveNum() const noexcept { return cur_wave_num + 1;};
        std::list<Monster>::iterator ReleaseMonster();
    private:
        const std::array<Wave, 15> WaveList;
        int cur_wave_it = -1;
        time_t wave_start_num;
        std::shared_ptr<Road> next;
    };

    class Road: public Cell {
    public:
        Road(): type_(ROAD), trap_(false) {};
        Road(const Road& cp);
        Road(Road&& cm);
        //TODO constructor from save
        cocos2d::Vec2 getDirection();
        std::shared_ptr<MagicTrap> setTrap();
        int removeTrap();
        bool isTrapped() const noexcept {return trap_;};
    private:
        bool trap_;
        std::shared_ptr<Road> next;
    };

    class Basement: public Cell {
    public:
        Basement(): type_(BASEMENT), tower(false) {};
        Basement(const Basement& cp);
        Basement(Basement&& cm);
        //TODO constructor from save
        std::shared_ptr<Tower> setTower();
        int removeTower();
        bool isBuildUp() const noexcept {return tower_;};
    private:
        bool tower_;
    };

    struct Map {
        std::vector<std::vector<std::shared_ptr<Cell>>> cell_arr;
        std::vector<std::vector<std::shared_ptr<Cell>>>::iterator castle;

        int save(std::string filename) const;
        int load(std::string filename);
        int resize(int new_size_x, int new_size_y);
    };
}
#endif //LAB4_MAP_H
