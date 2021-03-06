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
#include <cstdlib>
#include "cocos2d.h"
#include "list.h"


namespace base_structures {

    static const float TILE_SIZE = 50.0;
    static const std::string SAVE_PATH = "/home/copiedwonder/OOP_Labs/lab4/saves/";
    /**
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
    /**
     * @defgroup decsriptos Level Descriptors
     * @{
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

        bool operator==(const Effect& other) const;
    };
    /** @} */
    /**
     * Unit Descriptors. Used to define unit stats.
     */
    static std::vector<UnitLevel> TOWER_DESCR = {
            {
                100, 10, 50
            },
            {
                120, 15, 50
            }
    };
    static std::vector<UnitLevel> TRAP_DESCR {
            {
                50, 20, 50
            },
            {
                80, 30, 50
            }
    };
    static std::vector<EffectLevel> MAGICTOWER_DESCR {
            {
                std::chrono::duration<double>(1), 0.25
            },
            {
                std::chrono::duration<double>(1.5), 0.30
            }
    };
    static std::vector<EffectLevel> MAGICTRAP_DESCR {
            {
                std::chrono::duration<double>(2), 0.3
            },
            {
                std::chrono::duration<double>(2.5), 0.35
            }
    };

    int LoadUnitDescr(std::vector<UnitLevel> &descr, std::string filename);
    /**
     * @defgroup map_interface Map interface
     * @{
     */
    /// Types of map tiles
    enum CellType {
        BASE_CELL, ///< Generic type
        CASTLE,
        DANGEON,
        ROAD,
        BASEMENT
    };
    /**
     * Paths to sprite textures. Order related to enum CellType
     */
    static const std::string cell_sprite[] = {
            "/home/copiedwonder/OOP_Labs/lab4/TDS/Resources/res/cells/base_cell.png",
            "/home/copiedwonder/OOP_Labs/lab4/TDS/Resources/res/cells/castle.png",
            "/home/copiedwonder/OOP_Labs/lab4/TDS/Resources/res/cells/dangeon.png",
            "/home/copiedwonder/OOP_Labs/lab4/TDS/Resources/res/cells/road.png",
            "/home/copiedwonder/OOP_Labs/lab4/TDS/Resources/res/cells/basement.png"
    };

    using Wave = std::list<std::pair<std::shared_ptr<base_structures::Monster>, double>>;
    using WaveList = std::vector<Wave>;

    /**
     * @bried Base tile class
     */
    class Cell {
    public:
        Cell(): Cell(0, 0, BASE_CELL) {};
        Cell(int x, int y, CellType t = BASE_CELL): type_(t) {
            if (type_ != BASE_CELL)
                sprite_ = cocos2d::Sprite::create(cell_sprite[int(type_)]);
            else {
                int cell_var = rand() % 4;
                sprite_ = cocos2d::Sprite::create("/home/copiedwonder/OOP_Labs/lab4/TDS/Resources/res/cells/base_cell"
                                                                                + std::to_string(cell_var) + ".png");
            }
            sprite_->setPosition(x * TILE_SIZE, y * TILE_SIZE);
        };
        Cell(const Cell& cp);
        Cell& operator=(const Cell& cp);
        Cell& operator=(Cell&& cm);
        virtual CellType getType() const noexcept { return type_;};
        ~Cell() {}
        cocos2d::Sprite* sprite_;
    protected:
        CellType type_;
    };
    /**
     * @bried Class of castle tile
     */
    class Castle: public Cell {
    public:
        Castle(int x, int y): Castle(x, y, 200, 100) {};
        Castle(int x, int y, int hp, int gold); ///< @throws std::invalid_argument If HP or Gold values are negative
        Castle(const Castle& cp);

        int getHp() const noexcept { return hp_;};
        int getGold() const noexcept { return gold_;};
        void income(const Monster& frag) noexcept; ///< Increase money by the cost of monster. Do this when monster dies.
        bool spend(int cost = 50) noexcept;///< Spend money for new unit.
        void doDamage(const Monster& frag) noexcept; ///< Decrease castle hp by the current monster hp.
    private:
        int hp_ = 200;
        int gold_ = 300;
    };
    /**
     * @brief Class of dangeon tile
     */
    class Dangeon: public Cell {
    public:
        Dangeon(int x, int y): Cell(x, y, DANGEON) {
            this->sprite_->setAnchorPoint(cocos2d::Vec2(0.5, 0.4));
        };
        Dangeon(int x, int y, std::ifstream& is); ///< Used to read dangeon params from binary save file
        Dangeon(const Dangeon& cp);
        /**
         * @warning This method does not check whether next wave is available. cur_wave_it will be incremented anyway,
         *          but dangeon will be disabled (isActive == false) if cur_wave_it is invalid.
         */
        void NextWave() noexcept {
            cur_wave_it++;
            isActive_ = cur_wave_it < waves.size() ? true : false;
        };
        bool isActive() const {return isActive_;};
        int getCurWaveNum() const noexcept { return cur_wave_it;};
        int getWavesCount() const noexcept {return waves.size();};
        std::shared_ptr<Monster> ReleaseMonster(std::chrono::time_point<std::chrono::steady_clock> wave_start);
        void AddWave(int count = 1);
        void AddMonster(std::pair<Monster, double> monster);
        Dangeon& setNext(std::shared_ptr<Road> n) {
            next = n;
            return *this;
        }
        double NextMonsterTime() const noexcept {
            if (!isActive_)
                return std::numeric_limits<double>::max();
            else if (waves[cur_wave_it].begin() != waves[cur_wave_it].end())
                return waves[cur_wave_it].front().second;
            else
                return std::numeric_limits<double>::max();
        }
        int saveToFile(std::ofstream& os) const; ///< Save dangeon waves to save file
    private:
        WaveList waves; ///< Container for all dangeon monsters
        int cur_wave_it = -1;
        std::shared_ptr<Road> next = nullptr; ///< Ref to cell where monsters will be placed after Release.
        bool isActive_ = true; ///< Describes whether dangeon will release monster in future.
    };
    /**
     * @brief Abstract class for tiles any units (not monsters!) can be placed
     */
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
        std::shared_ptr<Unit> unit_ = nullptr; ///< Ref to emplaced unit
    };
    /**
     * @brief Class of road tile
     */
    class Road: public Placable {
    public:
        Road(int x, int y, std::shared_ptr<Road> n = nullptr): Placable(x, y, ROAD), next(n) {};
        Road(const Road& cp);
        cocos2d::Vec2 getDirection(); ///< Get unit vector in cartesian system for monster movement
        std::shared_ptr<Road> getNext() const noexcept { return next;};
        Road& setNext(std::shared_ptr<Road> n) {
            n.get() != this ? next = n : throw std::invalid_argument("Way loop detected.");
            return *this;
        };
        std::shared_ptr<Unit> setUnit() override {return setUnit(FROZEN);};
        std::shared_ptr<Unit> setUnit(EffectType type); ///< Place magic trap with specified type
        std::shared_ptr<Unit> setUnit(std::shared_ptr<Unit> unit);
    private:
        std::shared_ptr<Road> next; ///< Ref to tile where all monsters on this tile should go
    };
    /**
     * @brief Class of basement tile
     */
    class Basement: public Placable {
    public:
        Basement(int x, int y): Placable(x, y, BASEMENT) {};
        Basement(const Basement& cp);
        std::shared_ptr<Unit> setUnit() override;
        std::shared_ptr<Unit> setUnit(std::shared_ptr<Unit> unit);
    };

    /**
     * @brief Class of Tiles Map
     */
    struct Map_ {
        std::vector<std::vector<std::shared_ptr<Cell>>> cell_arr; ///< Tiles container
        std::shared_ptr<Castle> castle; ///< Quick ref to castle on the map. Only one castle is permitted.
        Map_() = default;
        Map_(int width, int height); ///< Init map of specified size with base cells

        int save(std::string filename) const; ///< Save map to binary file
        int load(std::string filename); ///< Load map from binary save file
    };
    /** @} */
    /**
     * @defgroup monster_interface Monster interface
     * @{
     */

    /// Monster model types. Used for reference to textures files
    enum MonsterModel {
        WEEK,
        STRONG
    };

    static const std::string monster_models[] = {
            "res/monsters/week_monster.png",
            "res/monsters/strong_monster.png"
    };

    /**
     * @brief Generic monster stats descriptor
     */
    struct MonsterDescriptor {
        int hp;
        int speed;
        int cost;
        MonsterModel model;
    };
    /**
     * @brief Base monster class
     */
    class Monster {
    public:
        Monster(): Monster(100, 40, 50, WEEK) {};
        Monster(int hp, int speed, int cost, MonsterModel model): hp_(hp), speed_(speed), cost_(cost), model_(model) {
            sprite_ = cocos2d::Sprite::create(monster_models[int(model)]);
        };
        Monster(MonsterDescriptor& disc): Monster(disc.hp, disc.speed, disc.cost, disc.model) {};
        Monster(const Monster& cp);
        int getCost() const noexcept { return cost_;};
        int getHP() const noexcept { return hp_;};
        int getSpeed() const noexcept { return speed_;};
        MonsterModel getModel() const noexcept { return model_;};
        Monster& setRelation(std::shared_ptr<Road> cell);
        std::shared_ptr<Road> getRelation() const noexcept { return relation;};
        Monster& applyDebuf(Effect debuf);
        void UpdateDebufs(); ///< Check whether debufs expired. Used by scheduler on each game frame
        double slowEffectStrength() const; ///< @return coef of monster speed
        Monster& getDamage(int damage);
        bool isAlive() const noexcept { return hp_ > 0; };
    private:
        int hp_ = 100;
        int speed_ = 40;
        int cost_ = 50;
        MonsterModel model_ = MonsterModel::WEEK;
        std::list<std::pair<Effect, std::chrono::time_point<std::chrono::steady_clock>>> debufs; ///< Effects on monster
        std::shared_ptr<Road> relation = nullptr; ///< Tile which center monster currently passed. Used for proper monster movement
    public:
        cocos2d::Sprite* sprite_ = nullptr;
    };

    using MonsterTable_ = std::list<std::shared_ptr<Monster>>;
    /** @} */
    /**
     * @defgroup units_interface Units interface
     * @{
     */
    /// Paths to units' textures
    static const std::string unit_models[] = {
            "res/units/tower.png",
            "res/units/frozen_tower.png",
            "res/units/poison_tower.png",
            "res/units/exhaust_tower.png",
            "res/units/frozen_trap.png",
            "res/units/poison_trap.png",
            "res/units/exhaust_trap.png"
    };


    //attack styles
    enum AttackStyle {
        CLOSESTCASTLE,
        CLOSESTTOWER,
        WEEKEST,
        STRONGEST,
        FASTEST
    };
    /**
     * @defgroup attack_styles Attack style regression functions
     * @ingroup units_interface
     * @{
     */
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
    /** @} */

    /**
     * @brief Abstarct unit class
     */
    class Unit{
    public:
        Unit(int x, int y, std::string model, int level = 0): x_(x), y_(y), level_ (level) {
            sprite_ = cocos2d::Sprite::create(model);
            sprite_->setPosition({x * TILE_SIZE, y * TILE_SIZE});
        };
        virtual bool isUpgradable() noexcept = 0;
        virtual int Upgrade() noexcept = 0;
        int getLevel() const noexcept { return level_;};
        cocos2d::Sprite* sprite_;
    protected:
        int x_;
        int y_;
        int level_;
    };
    /**
     * @brief Tower unit class
     */
    class Tower: public Unit {
    public:
        Tower(const Tower& cp);
        Tower(const Tower& cp, std::string model);
        Tower(int x, int y, std::string model = unit_models[0], int level = 0): Unit(x, y, model, level) {
            sprite_->setAnchorPoint(cocos2d::Vec2(0.5, 0.2));
        };
        /**
         * Choose and attack monster from Monster Table basing on Tower attack style. Autoremove monster from Monster
         * Table if it is dead
         * @param MonsterTable
         * @return Attacked monster
         */
        virtual std::shared_ptr<Monster> Attack(MonsterTable_& MonsterTable);
        bool isUpgradable() noexcept override;
        int Upgrade() noexcept override;
        bool isReachable(const Monster& monster) const noexcept;
        float getDistance(const Monster& monster) const noexcept;
        AttackStyle getStyle() const noexcept { return style_;};
        Tower& setStyle(AttackStyle style) noexcept {
            style_ = style;
            return *this;
        }
    protected:
        AttackStyle style_ = CLOSESTCASTLE;
    };
    /**
     * @brief Abstract class for magic units
     */
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
    /**
     * @brief Magic tower unit class
     */
    class MagicTower: public Tower, public MagicSignature {
    public:
        MagicTower(const Tower& tower, EffectType type, int effect_level = 0): Tower(tower, unit_models[int(type) + 1]), MagicSignature(type, effect_level){};
        std::shared_ptr<Monster> Attack(MonsterTable_& MonsterTable) override; ///< Attack monster and apply effect on it
        bool isEffectUpgradable() noexcept override;
        int UpgradeEffect() noexcept override;
    };
    /**
     * @brief Magic trap unit class
     */
    class MagicTrap: public Unit, public MagicSignature {
    public:
        MagicTrap(int x, int y, EffectType type, int effect_level = 0,  int trap_level = 0):
                Unit(x, y, unit_models[4 + int(type)], trap_level), MagicSignature(type, effect_level) {};
        /**
         * Attack all reachable monsters from Monster Table and apply effects on them. Autoremove monsters from
         * Monster Table if they are dead.
         * @param MonsterTable
         * @return Attacked mosnters
         */
        std::vector<std::shared_ptr<Monster>> Activate(MonsterTable_& MonsterTable);
        bool isUpgradable() noexcept override;
        int Upgrade() noexcept override;
        bool isEffectUpgradable() noexcept override;
        int UpgradeEffect() noexcept override;
        bool isReachable(const Monster& monster) const noexcept {
            return (monster.sprite_->getPosition().getDistance(sprite_->getPosition()) <= TRAP_DESCR[level_].rad);
        }
    };
    /**
     * @brief Container class for all units on the map
     */
    struct UnitTable_ {
        std::list<std::shared_ptr<Tower>> towers;
        std::list<std::shared_ptr<MagicTrap>> traps;
    };
    /** @} */
    template <typename T> int sgn(T val) {
        return (T(0) < val) - (val < T(0));
    }
}

#endif //LAB4_BASE_STRUCTURES_H
