//
// Created by copiedwonder on 05.12.2020.
//

#ifndef LAB4_BASE_STRUCTURES_H
#define LAB4_BASE_STRUCTURES_H
#include <vector>
#include <string>
#include <list>
#include <fstream>
#include <chrono>

namespace base_structures {
    /*
     * Type predefinition
     */
    class Unit;
    class Tower;
    class MagicTower;
    class MagicTrap;
    class Monster;
    struct Map_;
    struct UnitTable_;
    class Cell;
    class Castle;
    class Dangeon;
    class Placable;
    class Road;
    class Basement;
    /*
     * Level Descriptors
     */
    struct UnitLevel {
        int rad;
        int damage;
        int cost;
    };
    struct EffectLevel {
        std::chrono::duration<double> effect_time;
        double effect_strength;
    };
    enum EffectType {
        FROZEN,
        POISON,
        EXHAUST
    };
    struct Effect {
        std::chrono::duration<double> effect_time;
        double effect_strength;
        EffectType type;
    };
    static std::vector<UnitLevel> TOWER_DESCR;
    static std::vector<UnitLevel> TRAP_DESCR;
    static std::vector<EffectLevel> MAGICTOWER_DESCR;
    static std::vector<EffectLevel> MAGICTRAP_DESCR;

    int LoadUnitDescr(std::vector<UnitLevel> &descr, std::string filename);
    /*
     * Map interface
     */
    enum CellType {
        BASE_CELL,
        CASTLE,
        DANGEON,
        ROAD,
        BASEMENT
    };
    using Wave = std::list<std::pair<std::shared_ptr<base_structures::Monster>, double>>;
    using WaveList = std::vector<Wave>;

    class Cell {
    public:
        Cell(): Cell(0, 0, BASE_CELL) {};
        Cell(int x, int y, CellType t = BASE_CELL): type_(t) {};
        Cell(const Cell& cp);
        Cell(Cell && cm);
        virtual CellType getType() const noexcept { return type_;};
        ~Cell() {}
    protected:
        CellType type_;
    };
    class Castle: public Cell {
    public:
        Castle(int x, int y): Cell(x, y, CASTLE) {};
        Castle(int x, int y, int hp, int gold): hp_(hp), gold_(gold), Cell(x, y, CASTLE) {
            if (hp <= 0 || gold <= 0) {
                throw std::invalid_argument("Castle params are negative.");
            }
        };
        Castle(const Castle& cp);
        Castle(Castle&& cm);

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
        Dangeon(int x, int y): Cell(x, y, DANGEON) {};
        Dangeon(int x, int y, std::istream& is);
        Dangeon(const Dangeon& cp);
        Dangeon(Dangeon&& cm);

        void NextWave() noexcept { cur_wave_it++;};
        bool isActive() {return isActive_;};
        int getCurWaveNum() const noexcept { return cur_wave_it;};
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
        std::shared_ptr<Road> next = nullptr;
        bool isActive_ = true;
    };
    class Placable: public Cell {
    public:
        Placable(int x, int y, CellType t): Cell(x, y, t), unit_(nullptr) {};
        virtual std::shared_ptr<Unit> setUnit() = 0;
        std::shared_ptr<Unit> getUnit() { return unit_;};
        bool isBusy() const noexcept {return unit_ != nullptr;};
        void removeUnit() {
            unit_ = nullptr;
        }
    protected:
        std::shared_ptr<Unit> unit_;
    };
    class Road: public Placable {
    public:
        Road(int x, int y, std::shared_ptr<Road> n = nullptr): Placable(x, y, ROAD), next(n) {};
        Road(const Road& cp);
        Road(Road&& cm);
        std::shared_ptr<Road> getNext() const noexcept { return next;};
        Road& setNext(std::shared_ptr<Road> n) {
            n.get() != this ? next = n : throw std::invalid_argument("Way loop detected.");
            return *this;
        };
        std::shared_ptr<Unit> setUnit() override {return setUnit(FROZEN);};
        std::shared_ptr<Unit> setUnit(EffectType type);
        std::shared_ptr<Unit> setUnit(std::shared_ptr<Unit> unit);
    private:
        std::shared_ptr<Road> next;
    };
    class Basement: public Placable {
    public:
        Basement(int x, int y): Placable(x, y, BASEMENT) {};
        Basement(const Basement& cp);
        Basement(Basement&& cm);
        std::shared_ptr<Unit> setUnit() override;
        std::shared_ptr<Unit> setUnit(std::shared_ptr<Unit> unit);
    };

    struct Map_ {
        std::vector<std::vector<std::shared_ptr<Cell>>> cell_arr;
        std::shared_ptr<Castle> castle;

        int save(std::string filename) const;
        int load(std::string filename);
        // int resize(int new_size_x, int new_size_y);
    };
    /*
     * Monster interface
     */
    enum MonsterModel {
        WEEK,
        STRONG
    };


    struct MonsterDescriptor {
        int hp;
        int speed;
        int cost;
        MonsterModel model;

        friend std::istream& operator>>(std::istream& is, MonsterDescriptor& desc) noexcept;
    };
    class Monster {
    public:
        Monster() {};
        Monster(int hp, int speed, int cost, MonsterModel model): hp_(hp), speed_(speed), cost_(cost), model_(model) {};
        Monster(MonsterDescriptor& disc): Monster(disc.hp, disc.speed, disc.cost, disc.model) {};
        Monster(const Monster& cp);
        Monster(Monster&& cm);
        int getCost() const noexcept { return cost_;};
        int getHP() const noexcept { return hp_;};
        int getSpeed() const noexcept { return speed_;};
        MonsterModel getModel() const noexcept { return model_;};
        Monster& setRelation(std::shared_ptr<Road> cell);
        std::shared_ptr<Road> getRelation() const noexcept { return relation;};
        Monster& applyDebuf(Effect debuf);
        Monster& getDamage(int damage);
        bool isAlive() const noexcept { return hp_ > 0; };
    private:
        int hp_ = 100;
        int speed_ = 40;
        int cost_ = 50;
        MonsterModel model_ = MonsterModel::WEEK;
        std::list<std::pair<Effect, std::chrono::time_point<std::chrono::steady_clock>>> debufs;
        std::shared_ptr<Road> relation = nullptr;
    };

    using MonsterTable_ = std::list<std::shared_ptr<Monster>>;

    /*
     * Units interface
     */
    //attack styles
    enum AttackStyle {
        CLOSESTCASTLE,
        CLOSESTTOWER,
        WEEKEST,
        STRONGEST,
        FASTEST
    };
    float closestToCastle(const Tower& t, const Monster& m);
    float closestToTower(const Tower& t, const Monster& m);
    float weekest(const Tower& t, const Monster& m);
    float strongest(const Tower& t, const Monster& m);
    float fastest(const Tower& t, const Monster& m);
    static float (*ATTACK_CONDITIONS[])(const Tower&, const Monster&) {
            closestToCastle,
            closestToTower,
            weekest,
            strongest,
            fastest
    };

    class Unit{
    public:
        Unit(int x, int y, int level = 0): x_(x), y_(y), level_ (level) {};
        virtual bool isUpgradable() noexcept = 0;
        virtual int Upgrade() noexcept = 0;
        int getLevel() const noexcept { return level_;};
    protected:
        int x_;
        int y_;
        int level_;
    };
    class Tower: public Unit {
    public:
        Tower(const Tower& cp);
        Tower(Tower&& cm);
        Tower(int x, int y, int level_ = 0): Unit(x, y, level_) {};
         // std::shared_ptr<MagicTower> toMagic(EffectType type);
        virtual std::shared_ptr<Monster> Attack(MonsterTable_& MonsterTable);
        bool isUpgradable() noexcept override;
        int Upgrade() noexcept override;
        // bool isReachable(const Monster& monster) const noexcept;
        // float getDistance(const Monster& monster) const noexcept;
        AttackStyle getStyle() const noexcept { return style_;};
        Tower& setStyle(AttackStyle style) noexcept {
            style_ = style;
            return *this;
        }
    protected:
        AttackStyle style_ = CLOSESTCASTLE;
    };
    class MagicSignature {
    public:
        MagicSignature(EffectType type, int effect_level = 0): type_(type), effect_level_(effect_level) {};
        virtual bool isEffectUpgradable() noexcept = 0;
        virtual int UpgradeEffect() noexcept = 0;
        EffectType getType() const noexcept { return type_;};
        int getEffectLevel() const noexcept { return effect_level_;};
    protected:
        EffectType type_;
        int effect_level_;
    };
    class MagicTower: public Tower, public MagicSignature {
    public:
        MagicTower(const Tower& tower, EffectType type): Tower(tower), MagicSignature(type){};
        std::shared_ptr<Monster> Attack(MonsterTable_& MonsterTable) override;
        bool isEffectUpgradable() noexcept override;
        int UpgradeEffect() noexcept override;
    };
    class MagicTrap: public Unit, public MagicSignature {
    public:
        MagicTrap(int x, int y, EffectType type, int effect_level = 0,  int trap_level = 0):
                Unit(x, y, trap_level), MagicSignature(type, effect_level) {};
        int Activate(MonsterTable_& MonsterTable);
        bool isUpgradable() noexcept override;
        int Upgrade() noexcept override;
        bool isEffectUpgradable() noexcept override;
        int UpgradeEffect() noexcept override;
        //bool isReachable(const Monster& monster) const noexcept {
        //    return (monster.sprite_->getPosition().getDistance(sprite_->getPosition()) <= TRAP_DESCR[level_].rad);
        //}
        ~MagicTrap();
    };

    struct UnitTable_ {
        std::list<std::shared_ptr<Tower>> towers;
        std::list<std::shared_ptr<MagicTrap>> traps;
    };
}

#endif //LAB4_BASE_STRUCTURES_H
