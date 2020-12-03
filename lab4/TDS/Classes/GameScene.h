//
// Created by copiedwonder on 03.12.2020.
//

#ifndef LAB4_GAMESCENE_H
#define LAB4_GAMESCENE_H

#include <chrono>
#include "cocos2d.h"

class Game : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    std::chrono::time_point<std::chrono::steady_clock> wave_start;

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(Game);
};

#endif //LAB4_GAMESCENE_H
