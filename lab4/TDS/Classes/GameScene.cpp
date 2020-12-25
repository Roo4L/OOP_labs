//
// Created by copiedwonder on 03.12.2020.
//

#include "GameScene.h"
#include <memory>
#include <future>
#include "Popup.h"

#define EXEC_SEP(mutex, code) \
mutex.lock();                 \
code;                         \
mutex.unlock();

static int TILE_WIDTH = 50;

USING_NS_CC;

Scene* Game::createScene(std::string level_name)
{
    level_name_ = level_name;
    return Game::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Game::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    // init global variables
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    if (Map.load(level_name_))
        throw std::invalid_argument("Can't read Map save.");

    // display all sprites
    for (auto& row : Map.cell_arr) {
        for (auto& cell : row) {
            switch (cell->getType()) {
                case base_structures::DANGEON:
                case base_structures::CASTLE:
                    this->addChild(cell->sprite_, 1);
                    break;
                default:
                    this->addChild(cell->sprite_);
            }
        }
    }

    auto _mouseListener = EventListenerMouse::create();
    _mouseListener->onMouseDown = CC_CALLBACK_1(Game::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener->clone(), this);

    auto key_listener = EventListenerKeyboard::create();
    key_listener->onKeyReleased = CC_CALLBACK_2(Game::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(key_listener, this);

    wave_start = std::chrono::steady_clock::now();
    CellSelected = cocos2d::Sprite::create("CellSelected.png");
    CellSelected->setContentSize(Size(TILE_WIDTH, TILE_WIDTH));
    this->CellSelected->setPosition(cocos2d::Vec2(0, 0));
    this->addChild(this->CellSelected);

    this->HPTextField = cocos2d::Label::createWithTTF("HP: " + std::to_string(Map.castle->getHp()), "fonts/Marker Felt.ttf", 30);
    this->HPTextField->setPosition(cocos2d::Vec2(visibleSize.width - HPTextField->getContentSize().width,
                                                 visibleSize.height - HPTextField->getContentSize().height));
    this->addChild(HPTextField, 6);

    this->GoldTextField = cocos2d::Label::createWithTTF("Gold: " + std::to_string(Map.castle->getGold()), "fonts/Marker Felt.ttf", 30);
    this->GoldTextField->setPosition(cocos2d::Vec2(visibleSize.width - HPTextField->getContentSize().width,
                                                 visibleSize.height - HPTextField->getContentSize().height * 2));
    this->addChild(GoldTextField, 6);

    this->WaveNumTextField = cocos2d::Label::createWithTTF("Wave: -1", "fonts/Marker Felt.ttf", 30);
    this->WaveNumTextField->setPosition(cocos2d::Vec2(visibleSize.width - HPTextField->getContentSize().width,
                                                 visibleSize.height - HPTextField->getContentSize().height * 3));
    this->addChild(WaveNumTextField, 6);

    this->scheduleUpdate();

    return true;
}

void Game::update(float delta) {
    static float last_attack_time = 0;
    last_attack_time += delta;
    GoldTextField->setString("Gold: " + std::to_string(Map.castle->getGold()));
    if (!lose) {
        if (isWaveEnded()) {
             if (isGameEnded()) {
                 auto dialog = UICustom::WinPopup::create(level_name_);
                 this->pause();
                 _eventDispatcher->pauseEventListenersForTarget(this);
                 this->addChild(dialog, 5);
             }
            wave_num++;
            WaveNumTextField->setString("Wave: " + std::to_string(wave_num));
            wave_start = std::chrono::steady_clock::now();
            for (auto& row : Map.cell_arr) {
                for (auto& cell : row) {
                    if (cell->getType() == base_structures::DANGEON) {
                        std::dynamic_pointer_cast<base_structures::Dangeon>(cell)->NextWave();
                    }
                }
            }
        }
        std::future<void> releaseMonstersFuture= std::async(std::launch::async, &::Game::releaseMonsters, this);
        std::future<void> updateDebufsFuture = std::async(std::launch::async, &::Game::updateDebufs, this);
        std::future<void> activateTrapsFuture = std::async(std::launch::async, &::Game::activateTraps, this);
        if (last_attack_time > 0.7) {
            std::future<void> attackTowersFuture = std::async(std::launch::async, &::Game::attackTowers, this);
            last_attack_time = 0;
        }
        std::future<void> moveMonstersFuture = std::async(std::launch::async, &::Game::moveMonsters, this, delta);
    }
    else {
        scene_mutex.lock();
        auto dialog = UICustom::LosePopup::create(level_name_);
        this->pause();
        _eventDispatcher->pauseEventListenersForTarget(this);
        this->addChild(dialog, 5);
        scene_mutex.unlock();
    }
}


void Game::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

bool Game::isWaveEnded() {
    if (wave_num < 0) {
        return true;
    }
    monster_table_mutex.lock();
    bool is_empty = MonsterTable.empty();
    monster_table_mutex.unlock();
    if (is_empty) {
        for (auto& row : Map.cell_arr) {
            for (auto& cell : row) {
                if (cell->getType() == base_structures::DANGEON &&
                    std::dynamic_pointer_cast<base_structures::Dangeon>(cell)->NextMonsterTime() != std::numeric_limits<double>::max()) {
                    return false;
                }
            }
        }
        return true;
    }
    else {
        return false;
    }
}

bool Game::isGameEnded() {
    monster_table_mutex.lock();
    bool is_empty = MonsterTable.empty();
    monster_table_mutex.unlock();
    if (is_empty) {
        for (auto& row : Map.cell_arr) {
            for (auto& cell : row) {
                if (cell->getType() == base_structures::DANGEON &&
                    std::dynamic_pointer_cast<base_structures::Dangeon>(cell)->isActive()) {
                    return false;
                }
            }
        }
        return true;
    }
    else {
        return false;
    }
}

void Game::updateDebufs() {
    monster_table_mutex.lock();
    for (auto& monster : MonsterTable) {
        monster->UpdateDebufs();
    }
    monster_table_mutex.unlock();
}

void Game::releaseMonsters() {
    for (auto& row : Map.cell_arr) {
        for (auto& cell : row) {
            if (cell->getType() == base_structures::DANGEON) {
                auto dang = std::dynamic_pointer_cast<base_structures::Dangeon>(cell);
                std::shared_ptr<base_structures::Monster> monster = dang->ReleaseMonster(wave_start);
                while (monster != nullptr) {
                    scene_mutex.lock();
                    this->addChild(monster->sprite_, 2);
                    scene_mutex.unlock();
                    monster_table_mutex.lock();
                    MonsterTable.push_back(monster);
                    monster_table_mutex.unlock();
                    monster = dang->ReleaseMonster(wave_start);
                }
            }
        }
    }
}

void Game::activateTraps() {
    traps_mutex.lock();
    std::list<std::shared_ptr<base_structures::MagicTrap>> activated;
    for (const auto& trap : UnitTable.traps) {
        bool is_activate = false;
        monster_table_mutex.lock();
        for (auto& monster : MonsterTable) {
            if (trap->isReachable(*monster)) is_activate = true;
        }
        monster_table_mutex.unlock();
        if (is_activate) {
            monster_table_mutex.lock();
            std::vector<std::shared_ptr<base_structures::Monster>> victims = trap->Activate(MonsterTable);
            monster_table_mutex.unlock();
            for (auto victim : victims) {
                if (!victim->isAlive()) {
                    castle_mutex.lock();
                    Map.castle->income(*victim);
                    castle_mutex.unlock();
                    EXEC_SEP(scene_mutex,victim->sprite_->removeFromParentAndCleanup(true));
                } else {
                    auto tint1 = cocos2d::TintBy::create(0.1, 0.f, 125.f, 125.f);
                    auto tint2 = cocos2d::TintBy::create(0.1, 0.f, -125.f, -125.f);
                    auto tint_act = cocos2d::Sequence::create(tint1, tint2, nullptr);
                    victim->sprite_->runAction(tint_act);
                }
            }
            activated.push_back(trap);
        }
    }
    for (auto trap : activated) {
        UnitTable.traps.erase(std::find(UnitTable.traps.begin(), UnitTable.traps.end(), trap));
        EXEC_SEP(scene_mutex, trap->sprite_->removeFromParentAndCleanup(true));
    }
    traps_mutex.unlock();
}

void Game::attackTowers() {
    towers_mutex.lock();
    for (const auto& tower : UnitTable.towers) {
        std::shared_ptr<base_structures::Monster> victim = tower->Attack(MonsterTable);
        if (victim != nullptr && !victim->isAlive()) {
            EXEC_SEP(castle_mutex, Map.castle->income(*victim));
            EXEC_SEP(scene_mutex, victim->sprite_->removeFromParentAndCleanup(true));
        }
        else if (victim != nullptr) {
            auto tint1 = cocos2d::TintBy::create(0.1, 0.f, 125.f, 125.f);
            auto tint2 = cocos2d::TintBy::create(0.1, 0.f, -125.f, -125.f);
            auto tint_act = cocos2d::Sequence::create(tint1, tint2, nullptr);
            victim->sprite_->runAction(tint_act);
        }
    }
    towers_mutex.unlock();
}

void Game::moveMonsters(float dt) {
    monster_table_mutex.lock();
    std::list<std::shared_ptr<base_structures::Monster>> deleted;
    for (auto& monster : MonsterTable) {
        cocos2d::Vec2 pos = monster->sprite_->getPosition();
        double speed = monster->getSpeed() * monster->slowEffectStrength();
        auto direction = monster->getRelation()->getDirection();
        if (direction != Vec2(0, 0)) {
            pos += direction * speed * dt;
            monster->sprite_->setPosition(pos);
            if (monster->sprite_->getPosition().getDistance(monster->getRelation()->sprite_->getPosition()) > TILE_WIDTH) {
                monster->setRelation(monster->getRelation()->getNext());
            }
        }
        else {
            EXEC_SEP(castle_mutex, Map.castle->doDamage(*monster));
            this->HPTextField->setString("HP: " + std::to_string(Map.castle->getHp()));
            deleted.push_back(monster);
        }
    }
    for (auto& monster : deleted) {
        MonsterTable.erase(std::find(MonsterTable.begin(), MonsterTable.end(), monster));
        EXEC_SEP(scene_mutex, monster->sprite_->removeFromParentAndCleanup(true));
    }
    monster_table_mutex.unlock();
    castle_mutex.lock();
    if (Map.castle->getHp() <= 0) {
        lose = true;
    }
    castle_mutex.unlock();
}

void Game::onMouseDown(cocos2d::Event* event) {
    EventMouse* e = (EventMouse *)event;
    log("Click! X: %f. Y: %f.", e->getCursorX(), e->getCursorY());
    int x = static_cast<int>(round(e->getCursorX()/TILE_WIDTH));
    int y = static_cast<int>(round(e->getCursorY()/TILE_WIDTH));
    log("CellSelected: (%d,%d)", x, y);
    this->CellSelected->setPosition(this->Map.cell_arr[x][y]->sprite_->getPosition());
}

void Game::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, Event* event) {

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    int x = this->CellSelected->getPositionX() / TILE_WIDTH;
    int y = this->CellSelected->getPositionY() / TILE_WIDTH;
    switch(keyCode) {
        case cocos2d::EventKeyboard::KeyCode::KEY_1: {
            castle_mutex.lock();
            bool is_enough_money = Map.castle->spend();
            castle_mutex.unlock();
            if (is_enough_money) {
                switch(Map.cell_arr[x][y]->getType()) {
                    case base_structures::BASEMENT: {
                        auto cell = std::dynamic_pointer_cast<base_structures::Basement>(Map.cell_arr[x][y]);
                        if (cell->isBusy()) {
                            auto unit = std::static_pointer_cast<base_structures::Unit>(
                                    std::make_shared<base_structures::MagicTower>(
                                            *(std::dynamic_pointer_cast<base_structures::Tower>(cell->getUnit())),
                                            base_structures::FROZEN));
                            towers_mutex.lock();
                            UnitTable.towers.erase(std::find(UnitTable.towers.begin(), UnitTable.towers.end(),
                                                             std::dynamic_pointer_cast<base_structures::Tower>(
                                                                     cell->getUnit())));
                            towers_mutex.unlock();
                            EXEC_SEP(scene_mutex, cell->getUnit()->sprite_->removeFromParentAndCleanup(true));
                            cell->removeUnit();
                            cell->setUnit(unit);
                            unit->sprite_->setPosition(Map.cell_arr[x][y]->sprite_->getPosition());
                            EXEC_SEP(scene_mutex, this->addChild(unit->sprite_, 3));
                            EXEC_SEP(towers_mutex,
                                     UnitTable.towers.push_back(std::dynamic_pointer_cast<base_structures::Tower>(unit)));
                        }
                        break;
                    }
                    case base_structures::ROAD: {
                        auto cell = std::dynamic_pointer_cast<base_structures::Road>(Map.cell_arr[x][y]);
                        if (!cell->isBusy()) {
                            auto unit = cell->setUnit(base_structures::FROZEN);
                            unit->sprite_->setPosition(Map.cell_arr[x][y]->sprite_->getPosition());
                            EXEC_SEP(scene_mutex, this->addChild(unit->sprite_, 3));
                            EXEC_SEP(traps_mutex,
                                     UnitTable.traps.push_back(std::dynamic_pointer_cast<base_structures::MagicTrap>(unit)));
                        } else {
                            log("Can't set unit on busy cell.");
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
            castle_mutex.unlock();
            break;
        }
        case cocos2d::EventKeyboard::KeyCode::KEY_2: {
            castle_mutex.lock();
            bool is_enough_money = Map.castle->spend();
            castle_mutex.unlock();
            if (is_enough_money) {
                switch(Map.cell_arr[x][y]->getType()) {
                    case base_structures::BASEMENT: {
                        auto cell = std::dynamic_pointer_cast<base_structures::Basement>(Map.cell_arr[x][y]);
                        if (cell->isBusy()) {
                            auto unit = std::static_pointer_cast<base_structures::Unit>(
                                    std::make_shared<base_structures::MagicTower>(
                                            *(std::dynamic_pointer_cast<base_structures::Tower>(cell->getUnit())),
                                            base_structures::POISON));
                            towers_mutex.lock();
                            UnitTable.towers.erase(std::find(UnitTable.towers.begin(), UnitTable.towers.end(),
                                                             std::dynamic_pointer_cast<base_structures::Tower>(
                                                                     cell->getUnit())));
                            towers_mutex.unlock();
                            EXEC_SEP(scene_mutex, cell->getUnit()->sprite_->removeFromParentAndCleanup(true));
                            cell->removeUnit();
                            cell->setUnit(unit);
                            unit->sprite_->setPosition(Map.cell_arr[x][y]->sprite_->getPosition());
                            EXEC_SEP(scene_mutex, this->addChild(unit->sprite_, 3));
                            EXEC_SEP(towers_mutex,
                                     UnitTable.towers.push_back(std::dynamic_pointer_cast<base_structures::Tower>(unit)));
                        }
                        break;
                    }
                    case base_structures::ROAD: {
                        auto cell = std::dynamic_pointer_cast<base_structures::Road>(Map.cell_arr[x][y]);
                        if (!cell->isBusy()) {
                            auto unit = cell->setUnit(base_structures::POISON);
                            unit->sprite_->setPosition(Map.cell_arr[x][y]->sprite_->getPosition());
                            EXEC_SEP(scene_mutex, this->addChild(unit->sprite_, 3));
                            EXEC_SEP(traps_mutex,
                                     UnitTable.traps.push_back(std::dynamic_pointer_cast<base_structures::MagicTrap>(unit)));
                        } else {
                            log("Can't set unit on busy cell.");
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
            break;
        }
        case cocos2d::EventKeyboard::KeyCode::KEY_3: {
            castle_mutex.lock();
            bool is_enough_money = Map.castle->spend();
            castle_mutex.unlock();
            if (is_enough_money) {
                switch(Map.cell_arr[x][y]->getType()) {
                    case base_structures::BASEMENT: {
                        auto cell = std::dynamic_pointer_cast<base_structures::Basement>(Map.cell_arr[x][y]);
                        if (cell->isBusy()) {
                            auto unit = std::static_pointer_cast<base_structures::Unit>(
                                    std::make_shared<base_structures::MagicTower>(
                                            *(std::dynamic_pointer_cast<base_structures::Tower>(cell->getUnit())),
                                            base_structures::EXHAUST));
                            towers_mutex.lock();
                            UnitTable.towers.erase(std::find(UnitTable.towers.begin(), UnitTable.towers.end(),
                                                             std::dynamic_pointer_cast<base_structures::Tower>(
                                                                     cell->getUnit())));
                            towers_mutex.unlock();
                            EXEC_SEP(scene_mutex, cell->getUnit()->sprite_->removeFromParentAndCleanup(true));
                            cell->removeUnit();
                            cell->setUnit(unit);
                            unit->sprite_->setPosition(Map.cell_arr[x][y]->sprite_->getPosition());
                            EXEC_SEP(scene_mutex, this->addChild(unit->sprite_, 3));
                            EXEC_SEP(towers_mutex,
                                     UnitTable.towers.push_back(std::dynamic_pointer_cast<base_structures::Tower>(unit)));
                        }
                        break;
                    }
                    case base_structures::ROAD: {
                        auto cell = std::dynamic_pointer_cast<base_structures::Road>(Map.cell_arr[x][y]);
                        if (!cell->isBusy()) {
                            auto unit = cell->setUnit(base_structures::EXHAUST);
                            unit->sprite_->setPosition(Map.cell_arr[x][y]->sprite_->getPosition());
                            EXEC_SEP(scene_mutex, this->addChild(unit->sprite_, 3));
                            EXEC_SEP(traps_mutex,
                                     UnitTable.traps.push_back(std::dynamic_pointer_cast<base_structures::MagicTrap>(unit)));
                        } else {
                            log("Can't set unit on busy cell.");
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
            break;
        }
        case cocos2d::EventKeyboard::KeyCode::KEY_0: {
            castle_mutex.lock();
            bool is_enough_money = Map.castle->spend();
            castle_mutex.unlock();
            if (is_enough_money) {
                switch(Map.cell_arr[x][y]->getType()) {
                    case base_structures::BASEMENT: {
                        auto cell = std::dynamic_pointer_cast<base_structures::Basement>(Map.cell_arr[x][y]);
                        if (!cell->isBusy()) {
                            auto unit = std::dynamic_pointer_cast<base_structures::Tower>(
                                    std::dynamic_pointer_cast<base_structures::Basement>(Map.cell_arr[x][y])->setUnit());
                            unit->sprite_->setPosition(Map.cell_arr[x][y]->sprite_->getPosition());
                            EXEC_SEP(scene_mutex, this->addChild(unit->sprite_, 3));
                            EXEC_SEP(towers_mutex, UnitTable.towers.push_back(unit));
                        } else {
                            log("Can't set unit on busy cell.");
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
            break;
        }
        case cocos2d::EventKeyboard::KeyCode::KEY_U: {
            switch(Map.cell_arr[x][y]->getType()) {
                case base_structures::BASEMENT: {
                    auto cell = std::dynamic_pointer_cast<base_structures::Basement>(Map.cell_arr[x][y]);
                    if (cell->isBusy() && cell->getUnit()->isUpgradable() && (castle_mutex.lock(), Map.castle->spend())) {
                        EXEC_SEP(towers_mutex, cell->getUnit()->Upgrade());
                        auto star = cocos2d::Sprite::create("res/star.png");
                        star->setPosition(cell->getUnit()->sprite_->getPosition() +
                                            cocos2d::Vec2(cell->getUnit()->sprite_->getContentSize().width/2, 0));
                        EXEC_SEP(scene_mutex, this->addChild(star, 4));
                    }
                    castle_mutex.unlock();
                    break;
                }
                case base_structures::ROAD: {
                    auto cell = std::dynamic_pointer_cast<base_structures::Road>(Map.cell_arr[x][y]);
                    if (cell->isBusy() && cell->getUnit()->isUpgradable() && (castle_mutex.lock(),Map.castle->spend())) {
                        EXEC_SEP(traps_mutex, cell->getUnit()->Upgrade());
                        auto star = cocos2d::Sprite::create("res/star.png");
                        star->setPosition(cell->getUnit()->sprite_->getPosition() +
                                          cocos2d::Vec2(cell->getUnit()->sprite_->getContentSize().width/2, 0));
                        EXEC_SEP(scene_mutex, this->addChild(star, 4));
                    }
                    castle_mutex.unlock();
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case cocos2d::EventKeyboard::KeyCode::KEY_A: {
            switch(Map.cell_arr[x][y]->getType()) {
                case base_structures::BASEMENT: {
                    auto cell = std::dynamic_pointer_cast<base_structures::Basement>(Map.cell_arr[x][y]);
                    if (cell->isBusy()) {
                        towers_mutex.lock();
                        auto tower = std::dynamic_pointer_cast<base_structures::Tower>(cell->getUnit());
                        int attack_num = int(tower->getStyle() + 1) % 5;
                        tower->setStyle(static_cast<base_structures::AttackStyle>(attack_num));
                        towers_mutex.unlock();
                        EXEC_SEP(scene_mutex, cell->sprite_->removeAllChildrenWithCleanup(true));
                        auto attack_icon = cocos2d::Label::createWithTTF( std::to_string(attack_num), "fonts/Marker Felt.ttf", 24);
                        attack_icon->setTextColor(Color4B::RED);
                        attack_icon->setPosition(cell->sprite_->getPosition());
                        auto prev = this->getChildByName("attack mode");
                        scene_mutex.lock();
                        if (prev != nullptr)
                            this->removeChild(prev);
                        this->addChild(attack_icon, 5, "attack mode");
                        scene_mutex.unlock();
                    }
                }
            }
        }
    }
}
