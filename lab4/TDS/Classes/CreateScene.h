//
// Created by copiedwonder on 12.12.2020.
//

#ifndef LAB4_CREATESCENE_H
#define LAB4_CREATESCENE_H

#include "cocos2d.h"
#include "base_structures.h"

static const int MAP_WIDTH = 21;
static const int MAP_HEIGHT= 16;
static const int TILE_WIDTH = 50;

class MapConstructor : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    cocos2d::Sprite* CellSelected;
    base_structures::Map_ Map = base_structures::Map_(MAP_WIDTH, MAP_HEIGHT);
    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void onMouseDown(cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // implement the "static create()" method manually
    CREATE_FUNC(MapConstructor);
};


#endif //LAB4_CREATESCENE_H
