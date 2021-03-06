#include "base_structures.h"
#include <algorithm>
#include <memory>
namespace base_structures {
    int LoadUnitDescr(std::vector<UnitLevel> &descr, std::string filename) {
        std::ifstream config(filename, std::ios::binary);
        if (!config.is_open()) {
            return -1;
        }
        char *buf = new char[sizeof(UnitLevel)];
        UnitLevel *level = new UnitLevel();
        while (!config.eof()) {
            config.read((char *) level, sizeof(UnitLevel));
            descr.push_back(*level);
        }
        config.close();
        return 0;
    }

    bool Effect::operator==(const Effect &other) const {
        return (type == other.type) && (effect_time == other.effect_time) && (effect_strength == other.effect_strength);
    }

    /*
     * Map definition
     */
    Cell::Cell(const Cell &cp) {
        type_ = cp.type_;
        sprite_ = cocos2d::Sprite::createWithTexture(cp.sprite_->getTexture());
    }

    Cell& Cell::operator=(const Cell& cp) {
        type_ = cp.type_;
        sprite_ = cocos2d::Sprite::createWithTexture(cp.sprite_->getTexture());
        sprite_->setContentSize(cp.sprite_->getContentSize());
        sprite_->setPosition(cp.sprite_->getPosition());
        return *this;
    }

    Cell& Cell::operator=(Cell&& cm) {
        type_ = cm.type_;
        sprite_ = cm.sprite_;
        cm.type_ = BASE_CELL;
        cm.sprite_ = nullptr;
        return *this;
    }

    Castle::Castle(int x, int y, int hp, int gold): hp_(hp), gold_(gold), Cell(x, y, CASTLE) {
        if (hp <= 0 || gold <= 0) {
            throw std::invalid_argument("Castle params are negative.");
        }
        this->sprite_->setAnchorPoint(cocos2d::Vec2(0.5, 0.4));
    };

    Castle::Castle(const Castle &cp) {
        type_ = cp.type_;
        hp_ = cp.hp_;
        gold_ = cp.gold_;
        sprite_ = cocos2d::Sprite::createWithTexture(cp.sprite_->getTexture());
    }
    void Castle::income(const Monster& frag) noexcept {
        gold_ += frag.getCost();
    }
    bool Castle::spend(int cost) noexcept {
        if (gold_ >= cost) {
            gold_ -= cost;
            return true;
        }
        else
            return false;
    }
    void Castle::doDamage(const Monster &frag) noexcept {
        hp_ -= frag.getHP();
    }

    Dangeon::Dangeon(int x, int y, std::ifstream& is): Cell(x, y, DANGEON), waves(100) {
        int k, last_wave_num = -1;
        is.read((char *) &k, sizeof(int));
        if (!is.good())
            throw std::invalid_argument("Dangeone Constructor - input file can't be read.");
        while (k < 100) { // End reading when wave num is 100 (delimiter)
            if (k != last_wave_num) last_wave_num = k;
            double spawn_time;
            MonsterDescriptor desc;
            is.read((char *) &desc, sizeof(MonsterDescriptor));
            if (!is.read((char *) &spawn_time, sizeof(double)))
                throw std::invalid_argument("Dangeon Constructor - input file is currupted.");
            waves[k].push_back({std::make_shared<base_structures::Monster>(desc), spawn_time});
            if (!is.read((char *) &k, sizeof(int)))
                throw std::invalid_argument("Dangeon Constructor - input file is currupted.");
        }
        waves.resize(last_wave_num + 1);
        this->sprite_->setAnchorPoint(cocos2d::Vec2(0.5, 0.4));
    }
    std::shared_ptr<Monster> Dangeon::ReleaseMonster(std::chrono::time_point<std::chrono::steady_clock> wave_start) {
        if (!isActive() || waves[cur_wave_it].begin() == waves[cur_wave_it].end()) {
            return nullptr;
        }
        if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - wave_start).count() > waves[cur_wave_it].front().second) {
            std::shared_ptr<base_structures::Monster> m = waves[cur_wave_it].front().first;
            waves[cur_wave_it].pop_front();
            //add monster on map
            m->sprite_ = cocos2d::Sprite::create(monster_models[m->getModel()]);
            m->setRelation(next);
            m->sprite_->setPosition(this->next->sprite_->getPosition());
            return m;
        }
        else
            return nullptr;
    }

    void Dangeon::AddWave(int count) {
        for (int i = 0; i < count; i++) {
            waves.push_back(Wave());
        }
    }

    void Dangeon::AddMonster(std::pair<Monster, double> monster) {
        waves.back().push_back({std::make_shared<Monster>(monster.first), monster.second});
    }

    int Dangeon::saveToFile(std::ofstream &os) const {
        for (int i = 0; i < waves.size(); ++i) {
            for (auto& release : waves[i]) {
                os.write((char *) &i, sizeof(int));
                int hp = release.first->getHP();
                int speed = release.first->getSpeed();
                int cost = release.first->getCost();
                MonsterModel model = release.first->getModel();
                os.write((char *) &hp, sizeof(int));
                os.write((char *) &speed, sizeof(int));
                os.write((char *) &cost, sizeof(int));
                os.write((char *) &model, sizeof(int));
                double spawn_time = release.second;
                os.write((char *) &spawn_time, sizeof(double));
            }
        }
        int delim = 100;
        os.write((char *) &delim, sizeof(int));
        int x = next->sprite_->getPositionX() / TILE_SIZE;
        int y = next->sprite_->getPositionY() / TILE_SIZE;
        os.write((char *) &x, sizeof(int));
        os.write((char *) &y, sizeof(int));
        return 0;
    }

    cocos2d::Vec2 Road::getDirection() {
        if (next == nullptr) {
            return {0, 0};
        }
        float x = next->sprite_->getPositionX() - sprite_->getPositionX();
        float y = next->sprite_->getPositionY() - sprite_->getPositionY();
        return cocos2d::Vec2(sgn(x) , sgn(y));
    }

    std::shared_ptr<Unit> Road::setUnit(EffectType type) {
        if (unit_ != nullptr) throw std::logic_error("Can't set unit to non-empty cell.");
        unit_ = std::static_pointer_cast<Unit>(
                std::make_shared<MagicTrap>(sprite_->getPositionX() / sprite_->getContentSize().width,
                                            sprite_->getPositionY() / sprite_->getContentSize().height,
                                            type));
        return unit_;
    }
    std::shared_ptr<Unit> Road::setUnit(std::shared_ptr<Unit> unit) {
        if (unit_ != nullptr) throw std::logic_error("Can't set unit to non-empty cell.");
        unit_ = unit;
        return unit_;
    }

    std::shared_ptr<Unit> Basement::setUnit() {
        if (unit_ != nullptr) throw std::logic_error("Can't set unit to non-empty cell.");
        unit_ = std::static_pointer_cast<Unit>(
                        std::make_shared<Tower>(sprite_->getPositionX() / sprite_->getContentSize().width,
                                        sprite_->getPositionY() / sprite_->getContentSize().height));
        return unit_;
    }
    std::shared_ptr<Unit> Basement::setUnit(std::shared_ptr<Unit> unit) {
        if (unit_ != nullptr) throw std::logic_error("Can't set unit to non-empty cell.");
        unit_ = unit;
        return unit_;
    }

    Map_::Map_(int width, int height): cell_arr(width, std::vector<std::shared_ptr<base_structures::Cell>>(height)) {
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                cell_arr[i][j] = move(std::make_shared<base_structures::Cell>(i, j));
            }
        }
    }

    int Map_::load(std::string filename) {
        std::ifstream save_(SAVE_PATH + filename, std::ios::binary);
        if (!save_.is_open())
            return -1;
        int celltype;
        int size_x, size_y;
        std::vector<std::pair<int, int>> dangeon_way_trace;
        std::vector<std::pair<int, int>> way_trace;
        if (!save_.eof()) {
            save_.read((char *) &size_x, sizeof(int));
            save_.read((char *) &size_y, sizeof(int));
            cell_arr.resize(size_x);
            for (auto &it : cell_arr) {
                it.resize(size_y);
            }
        }
        for (int i = 0; i < size_x; i++) {
            for (int j = 0; j < size_y; j++) {
                if (save_.eof() || !save_.good()) {
                    save_.close();
                    return -1;
                }
                save_.read((char *) &celltype, sizeof(int));
                switch (celltype) {
                    case DANGEON: {
                        cell_arr[i][j] = std::static_pointer_cast<Cell>(std::make_shared<Dangeon>(i, j, save_));
                        int x, y;
                        save_.read((char *) &x, sizeof(int));
                        save_.read((char *) &y, sizeof(int));
                        dangeon_way_trace.push_back({i * size_y + j, x * size_y + y});
                        break;
                    }
                    case BASEMENT:
                        cell_arr[i][j] = std::static_pointer_cast<Cell>(std::make_shared<Basement>(i, j));
                        break;
                    case ROAD:
                        int x, y;
                        save_.read((char *) &x, sizeof(int));
                        save_.read((char *) &y, sizeof(int));
                        way_trace.emplace_back(std::pair<int,int>(i * size_y + j, x * size_y + y));
                        cell_arr[i][j] = std::static_pointer_cast<Cell>(std::make_shared<Road>(i, j));
                        break;
                    case CASTLE:
                        int hp, gold;
                        save_.read((char *) &hp, sizeof(int));
                        save_.read((char *) &gold, sizeof(int));
                        castle = std::make_shared<Castle>(i, j, hp, gold);
                        cell_arr[i][j] = std::static_pointer_cast<Cell>(castle);
                        break;
                    default:
                        cell_arr[i][j] = std::make_shared<Cell>(i, j);
                        break;
                }
            }
        }
        for (auto& path : way_trace) {
            std::shared_ptr<Road> source = std::dynamic_pointer_cast<Road>(
                    cell_arr[path.first / size_y][path.first % size_y]);
            if (path.second != size_x * size_y) {
                source->setNext(std::dynamic_pointer_cast<Road>(cell_arr[path.second / size_y][path.second % size_y]));
            }
            else {
                source->setNext(nullptr);
            }
        }
        for (auto& path : dangeon_way_trace) {
            std::shared_ptr<Dangeon> dang = std::dynamic_pointer_cast<Dangeon>(
                    cell_arr[path.first / size_y][path.first % size_y]);
            dang->setNext(std::dynamic_pointer_cast<Road>(cell_arr[path.second / size_y][path.second % size_y]));
        }
        save_.close();
        return 0;
    }
    int Map_::save(std::string filename) const {
        std::ofstream os(SAVE_PATH + filename, std::ios::binary);
        if (!os.is_open())
            return -1;
        int size_x = cell_arr.size();
        os.write((char *) &size_x, sizeof(int));
        int size_y =  cell_arr[0].size();
        os.write((char *) &size_y, sizeof(int));
        for (int i = 0; i < size_x; i++) {
            for (int j = 0; j < cell_arr[i].size(); j++) {
                CellType type = cell_arr[i][j]->getType();
                os.write((char *) &type, sizeof(int));
                switch (type) {
                    case DANGEON:
                        std::dynamic_pointer_cast<Dangeon>(cell_arr[i][j])->saveToFile(os);
                        break;
                    case ROAD: {
                        std::shared_ptr<Road> cell = std::dynamic_pointer_cast<Road>(cell_arr[i][j]);
                        int x, y;
                        if (cell->getNext() == nullptr) {
                            x = size_x;
                            y = 0;
                        }
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
                        os.write((char *) &x, sizeof(int));
                        os.write((char *) &y, sizeof(int));
                        break;
                    }
                    case CASTLE: {
                        std::shared_ptr<Castle> cell = std::dynamic_pointer_cast<Castle>(cell_arr[i][j]);
                        int hp = cell->getHp();
                        int gold = cell->getGold();
                        os.write((char *) &hp, sizeof(int));
                        os.write((char *) &gold, sizeof(int));
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
    /*
     * Units definition
     */
    float closestToCastle(const Tower& t, const Monster& m) {
        std::shared_ptr<Road> rel = m.getRelation();
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
        return m.getHP();
    }
    float strongest(const Tower& t, const Monster& m) {
        return -m.getHP();
    }
    float fastest(const Tower& t, const Monster& m) {
        return -m.getSpeed();
    }

    Tower::Tower(const Tower& cp): Tower(cp.x_, cp.y_) {
        sprite_ = cocos2d::Sprite::createWithTexture(cp.sprite_->getTexture());
        level_ = cp.level_;
    }

    Tower::Tower(const Tower& cp, std::string model): Tower(cp.x_, cp.y_, model) {
        level_ = cp.level_;
    }

    std::shared_ptr<Monster> Tower::Attack(MonsterTable_& MonsterTable) {
        std::shared_ptr<Monster> m = nullptr;
        float regression = std::numeric_limits<float>::max();
        for (auto it : MonsterTable) {
            float res;
            if (isReachable(*it) && ((res = ATTACK_CONDITIONS[int(style_)](*this, *it)) < regression)) {
                regression = res;
                m = it;
            }
        }
        if (m != nullptr) {
            m->getDamage(TOWER_DESCR[level_].damage);
            if (!m->isAlive()) {
                MonsterTable.erase(std::find(MonsterTable.begin(), MonsterTable.end(), m));
            }
        }
        return m;
    }
    bool Tower::isUpgradable() noexcept {
        return (TOWER_DESCR.size() != (level_ + 1));
    }
    int Tower::Upgrade() noexcept {
        if (isUpgradable()) {
            level_++;
            return 0;
        } else return -1;
    }
    bool Tower::isReachable(const Monster& monster) const noexcept {
        return (monster.sprite_->getPosition().getDistance(sprite_->getPosition()) <= TOWER_DESCR[level_].rad);
    }
    float Tower::getDistance(const Monster& monster) const noexcept {
        return (monster.sprite_->getPosition().getDistance(sprite_->getPosition()));
    }
    bool MagicTower::isEffectUpgradable() noexcept {
        return (MAGICTOWER_DESCR.size() != effect_level_ + 1);
    }
    int MagicTower::UpgradeEffect() noexcept {
        if (isEffectUpgradable()) {
            effect_level_++;
            return 0;
        } else return -1;
    }
    std::shared_ptr <Monster> MagicTower::Attack(MonsterTable_& MonsterTable) {
        std::shared_ptr<Monster> m = nullptr;
        int regression = std::numeric_limits<int>::max();
        int res;
        for (auto it : MonsterTable) {
            if (isReachable(*it) && (res = ATTACK_CONDITIONS[int(style_)](*this, *it)) < regression) {
                regression = res;
                m = it;
            }
        }
        if (m != nullptr) {
            m->getDamage(TOWER_DESCR[level_].damage);
            if (!m->isAlive()) {
                MonsterTable.erase(std::find(MonsterTable.begin(), MonsterTable.end(), m));
            }
            else {
                m->applyDebuf({MAGICTOWER_DESCR[effect_level_].effect_time,
                               MAGICTOWER_DESCR[effect_level_].effect_strength,
                               type_});
            }
        }
        return m;
    }
    bool MagicTrap::isUpgradable() noexcept {
        return (TRAP_DESCR.size() != (level_ + 1));
    }
    int MagicTrap::Upgrade() noexcept {
        if (isUpgradable()) {
            level_++;
            return 0;
        } else return -1;
    }
    bool MagicTrap::isEffectUpgradable() noexcept {
        return (MAGICTRAP_DESCR.size() != (effect_level_ + 1));
    }
    int MagicTrap::UpgradeEffect() noexcept {
        if (isEffectUpgradable()) {
            effect_level_++;
            return 0;
        } else return -1;
    }
    std::vector<std::shared_ptr<Monster>> MagicTrap::Activate(MonsterTable_& MonsterTable) { //TODO rethink return type
        std::vector<std::shared_ptr<Monster>> ret;
        for (auto it : MonsterTable) {
            if (isReachable(*it)) {
                it->getDamage(TRAP_DESCR[level_].damage);
                ret.push_back(it);
                if (it->isAlive()) {
                    it->applyDebuf({MAGICTRAP_DESCR[effect_level_].effect_time,
                                    MAGICTRAP_DESCR[effect_level_].effect_strength,
                                    type_});
                }
            }
        }
        for (auto it : ret) {
            if (!it->isAlive())
                MonsterTable.erase(std::find(MonsterTable.begin(), MonsterTable.end(), it));
        }
        return ret;
    }
    /*
     * Monsters definition
     */

    Monster& Monster::setRelation(std::shared_ptr<Road> cell) {
        if (cell == nullptr) {
            throw std::invalid_argument("Monster can't relate to null cell");
        }

        relation = cell;
        return *this;
    }
    Monster& Monster::applyDebuf(Effect debuf) {
        // check wheither the same debuf is already applied
        std::list<std::pair<base_structures::Effect, std::chrono::time_point<std::chrono::steady_clock>>> deleted;
        for (auto it = debufs.begin(); it != debufs.end(); ++it) {
            if (it->first.type == debuf.type) {
                if (it->first.effect_time < debuf.effect_time ||
                        it->first.effect_time + it->second - std::chrono::steady_clock::now() < debuf.effect_time) {
                    deleted.push_back(*it);
                }
                else
                    return *this;
            }
        }
        for (auto debuf : deleted) {
            debufs.erase(std::find(debufs.begin(), debufs.end(), debuf));
        }
        // apply debuf
        debufs.push_back({debuf, std::chrono::steady_clock::now()});
        return *this;
    }

    double Monster::slowEffectStrength() const {
        for (const auto& debuf : debufs) {
            if (debuf.first.type == FROZEN) {
                return (1 - debuf.first.effect_strength);
            }
        }
        return 1;
    }

    void Monster::UpdateDebufs() {
        std::vector<std::pair<Effect, std::chrono::time_point<std::chrono::steady_clock>>> deleted;
        for (auto& debuf : debufs) {
            if (std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - debuf.second).count() > debuf.first.effect_time.count()) {
                deleted.push_back(debuf);
            }
        }

        for (auto& debuf : deleted) {
            debufs.erase(std::find(debufs.begin(), debufs.end(), debuf));
        }
    }

    Monster& Monster::getDamage(int damage) {
        hp_ -= damage;
        return *this;
    }

    Monster::Monster(const Monster &cp) {
        hp_ = cp.hp_;
        speed_ = cp.speed_;
        cost_ = cp.cost_;
        model_ = cp.model_;
        sprite_ = cocos2d::Sprite::createWithTexture(cp.sprite_->getTexture());
    }
}
