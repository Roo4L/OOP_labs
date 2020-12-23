//
// Created by copiedwonder on 20.12.2020.
//

#ifndef LAB4_CHOOSELEVELSCENE_H
#define LAB4_CHOOSELEVELSCENE_H

#include "cocos2d.h"

class ChooseLevelScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    std::chrono::time_point<std::chrono::steady_clock> wave_start;

    virtual bool init();

    // a selector callback
    void menuBackCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(ChooseLevelScene);
};

std::vector<std::string> getLevelNames(std::string dir_path);

#endif //LAB4_CHOOSELEVELSCENE_H
