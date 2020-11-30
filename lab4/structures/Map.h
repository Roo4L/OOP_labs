//
// Created by copiedwonder on 23.11.2020.
//

#ifndef LAB4_MAP_H
#define LAB4_MAP_H
#include <memory>
#include <vector>
#include <utility>
#include "cocos2d.h"
#include "Units.h"
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
    typedef std::list<std::pair<std::shared_ptr<Monster>, time_t>> Wave;
    typedef const vector<Wave> WaveList;

    class Cell {
    public:
        Cell(CellType t = BASE_CELL): type_(t) {
            sprite = cocos2d::Sprite::create(cell_sprite[int(CellType)]);
        };
        Cell(const Cell& cp);
        Cell(Cell && cm);
        //TODO constructor from save
        CellType getType() const noexcept { return type_;};
        ~Cell() {
            sprite_->release();
        }
        cocos2d::Sprite* sprite_;
    private:
        CellType type_;
    };

    class Castle: public Cell {
    public:
        Castle(): Cell(CASTLE) {};
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
        Dangeon(std::string filename): Cell(DANGEON) {
            MakeWaveFromFile(waves, filename);
        };
        Dangeon(const Dangeon& cp);
        Dangeon(Dangeon&& cm);
        //TODO constructor from save
        int NextWave() const noexcept { cur_wave_it++; wave_start = time();};
        bool isActive() {return isActive_;};
        int getCurWaveNum() const noexcept { return cur_wave_num + 1;};
        std::shared_ptr<Monster> ReleaseMonster();
    private:
        WaveList waves;
        int cur_wave_it = -1;
        time_t wave_start = 0;
        std::shared_ptr<Road> next;
        bool isActive_ = true;
    };

    class Placable: public Cell {
    public:
        Placable(CellType t): Cell(t), unit_(nullptr) {};
        virtual std::shared_ptr<Unit> setUnit();
        bool isBusy() const noexcept {return unit_ != nullptr;};
        void removeUnit() {
            unit_.replace(nullptr);
        }
    protected:
        std::shared_ptr<Unit> unit_;
    };

    class Road: public Placable {
    public:
        Road(): Placable(ROAD) {};
        Road(const Road& cp);
        Road(Road&& cm);
        //TODO constructor from save
        cocos2d::Vec2 getDirection();
        std::shared_ptr<Road> getNext() const noexcept { return next;};
        std::shared_ptr<Unit> setUnit(EffectType type) override;
    private:
        std::shared_ptr<Road> next;
    };

    class Basement: public Placable {
    public:
        Basement(): Placable(BASEMENT) {};
        Basement(const Basement& cp);
        Basement(Basement&& cm);
        //TODO constructor from save
        shared_ptr<Unit> setUnit() override;
    };

    struct Map_ {
        std::vector<std::vector<std::shared_ptr<Cell>>> cell_arr;
        std::shared_ptr<Castle> castle;

        int save(std::string filename) const;
        int load(std::string filename);
        int resize(int new_size_x, int new_size_y);
    };

    struct UnitTable_ {
        std::list<std::shared_ptr<Tower>> towers;
        std::list<std::shared_ptr<MagicTrap>> traps;
    };

    UnitTable_ UnitTable;
    Map_ Map;
}
#endif //LAB4_MAP_H
