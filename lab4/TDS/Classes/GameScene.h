//
// Created by copiedwonder on 03.12.2020.
//

#ifndef LAB4_GAMESCENE_H
#define LAB4_GAMESCENE_H

#include <chrono>
#include <string>
#include "base_structures.h"
#include "cocos2d.h"

static std::string level_name_;

class Game : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene(std::string level_name);
    cocos2d::Sprite* CellSelected;
    cocos2d::Label* HPTextField;
    cocos2d::Label* WaveNumTextField;
    cocos2d::Label* GoldTextField;

    base_structures::Map_ Map;
    base_structures::MonsterTable_  MonsterTable;
    base_structures::UnitTable_ UnitTable;
    std::chrono::time_point<std::chrono::steady_clock> wave_start;
    bool lose = false;
    int wave_num = -1;

    virtual bool init();
    void update(float delta);

    // game cycle logic blocks
    bool isWaveEnded();
    bool isGameEnded();
    void releaseMonsters();
    void moveMonsters(float dt);
    void activateTraps();
    void attackTowers();
    void updateDebufs();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void onMouseDown(cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // implement the "static create()" method manually
    CREATE_FUNC(Game);
};

#endif //LAB4_GAMESCENE_H
