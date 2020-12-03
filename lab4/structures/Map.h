//
// Created by copiedwonder on 23.11.2020.
//

#ifndef LAB4_MAP_H
#define LAB4_MAP_H
#include <memory>
#include <vector>
#include <utility>
#include "model_types.h"
#include "cocos2d.h"
#include "Units.h"
#include "Monsters.h"

namespace base_structures {
    static const std::string cell_sprite[] = {
            "res/cells/base_cell.png",
            "res/cells/castle.png",
            "res/cells/dangeon.png",
            "res/cells/road.png",
            "res/cells/basement.png"
    };
    class Cell {
    public:
        Cell(CellType t = BASE_CELL): type_(t) {
            sprite_ = cocos2d::Sprite::create(cell_sprite[int(type_)]);
        };
        Cell(const Cell& cp);
        Cell(Cell && cm);
        //TODO constructor from save
        CellType getType() const noexcept { return type_;};
        ~Cell() {
            sprite_->release();
        }
        cocos2d::Sprite* sprite_;
    protected:
        CellType type_;
    };

    class Castle: public Cell {
    public:
        Castle(): Cell(CASTLE) {};
        Castle(int hp, int gold): hp_(hp), gold_(gold), Cell(CASTLE) {};
        Castle(const Castle& cp);
        Castle(Castle&& cm);
        //TODO constructor from save
        int getHp() const noexcept { return hp_;};
        int getGold() const noexcept { return gold_;};
        void income(const Monster& frag) noexcept;
        void doDamage(const Monster& frag) noexcept;
    private:
        int hp_ = 200;
        int gold_ = 100;
    };

    class Dangeon: public Cell {
    public:
        Dangeon(): Cell(DANGEON) {};
        Dangeon(std::istream& is): Cell(DANGEON), waves(100) {
            int k, last_wave_num = -1;
            is >> k;
            while (k != 100) { // End reading when wave num is 100 (delimiter)
                if (k != last_wave_num) last_wave_num = k;
                double spawn_time;
                MonsterDescriptor desc;
                is >> desc >> spawn_time;
                waves[k].push_back({std::make_shared<Monster>(desc), spawn_time});
            }
            waves.resize(last_wave_num + 1);
        };
        Dangeon(const Dangeon& cp);
        Dangeon(Dangeon&& cm);
        //TODO constructor from save
        int NextWave() noexcept { cur_wave_it++;};
        bool isActive() {return isActive_;};
        int getCurWaveNum() const noexcept { return cur_wave_it + 1;};
        std::shared_ptr<Monster> ReleaseMonster();
        double NextMonsterTime() const noexcept {
            if (waves[cur_wave_it].begin() != waves[cur_wave_it].end())
                return waves[cur_wave_it].front().second;
            else
                return std::numeric_limits<double>::max();
        }
        int saveToFile(std::ofstream& os) const;
    private:
        WaveList waves;
        int cur_wave_it = -1;
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
        Road(std::shared_ptr<Road> n = nullptr): Placable(ROAD), next(n) {};
        Road(const Road& cp);
        Road(Road&& cm);
        //TODO constructor from save
        cocos2d::Vec2 getDirection();
        std::shared_ptr<Road> getNext() const noexcept { return next;};
        std::shared_ptr<Road> setNext(std::shared_ptr<Road> n) {
            if (n.get() != this)
                next = n;
            else
                throw std::invalid_argument("Way loop detected.");
        };
        std::shared_ptr<Unit> setUnit() override {return setUnit(FROZEN);};
        std::shared_ptr<Unit> setUnit(EffectType type);
        std::shared_ptr<Unit> setUnit(std::shared_ptr<Unit> unit);
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
        std::shared_ptr<Unit> setUnit(std::shared_ptr<Unit> unit);
    };

    struct Map_ {
        std::vector<std::vector<std::shared_ptr<Cell>>> cell_arr;
        std::shared_ptr<Castle> castle;

        int save(std::string filename) const;
        int load(std::string filename);
        // int resize(int new_size_x, int new_size_y);
    };

    struct UnitTable_ {
        std::list<std::shared_ptr<Tower>> towers;
        std::list<std::shared_ptr<MagicTrap>> traps;
    };

    // UnitTable_ UnitTable;
    // Map_ Map;
}
#endif //LAB4_MAP_H
