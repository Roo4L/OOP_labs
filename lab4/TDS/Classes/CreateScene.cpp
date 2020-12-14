//
// Created by copiedwonder on 12.12.2020.
//
/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "CreateScene.h"
#include "ui/CocosGUI.h"
#include "base_structures.h"
#include <iostream>
#include "Popup.h"

USING_NS_CC;

Scene* MapConstructor::createScene()
{
    return MapConstructor::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MapConstructor::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    for (auto& row : Map.cell_arr) {
        for (auto& cell : row) {
            this->addChild(cell->sprite_, 0);
        }
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto _mouseListener = EventListenerMouse::create();
    _mouseListener->onMouseDown = CC_CALLBACK_1(MapConstructor::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener->clone(), this);

    auto key_listener = EventListenerKeyboard::create();
    key_listener->onKeyReleased = CC_CALLBACK_2(MapConstructor::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(key_listener, this);

    CellSelected = cocos2d::Sprite::create("CellSelected.png");
    CellSelected->setContentSize(Size(TILE_WIDTH, TILE_WIDTH));
    this->addChild(CellSelected, 2);
    return true;
}

void MapConstructor::onMouseDown(cocos2d::Event* event) {
    EventMouse* e = (EventMouse *)event;
    log("Click! X: %f. Y: %f.", e->getCursorX(), e->getCursorY());
    int x = static_cast<int>(round(e->getCursorX()/TILE_WIDTH));
    int y = static_cast<int>(round(e->getCursorY()/TILE_WIDTH));
    log("CellSelected: (%d,%d)", x, y);
    this->CellSelected->setPosition(this->Map.cell_arr[x][y]->sprite_->getPosition());
}
void MapConstructor::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
}

void MapConstructor::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, Event* event) {

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    int x = this->CellSelected->getPositionX() / TILE_WIDTH;
    int y = this->CellSelected->getPositionY() / TILE_WIDTH;
    switch(keyCode) {
        case cocos2d::EventKeyboard::KeyCode::KEY_0:
            this->removeChild(this->Map.cell_arr[x][y]->sprite_);
            this->Map.cell_arr[x][y] = std::static_pointer_cast<base_structures::Cell>(
                                                std::make_shared<base_structures::Cell>(x, y));
            this->addChild(this->Map.cell_arr[x][y]->sprite_, 0);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_1:
            this->removeChild(this->Map.cell_arr[x][y]->sprite_);
            this->Map.cell_arr[x][y] = std::static_pointer_cast<base_structures::Cell>(
                                                std::make_shared<base_structures::Castle>(x, y));
            this->addChild(this->Map.cell_arr[x][y]->sprite_, 0);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_2:
            this->removeChild(this->Map.cell_arr[x][y]->sprite_);
            this->Map.cell_arr[x][y] = std::static_pointer_cast<base_structures::Cell>(
                                                std::make_shared<base_structures::Dangeon>(x, y));
            this->addChild(this->Map.cell_arr[x][y]->sprite_, 0);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_3:
            this->removeChild(this->Map.cell_arr[x][y]->sprite_);
            this->Map.cell_arr[x][y] = std::static_pointer_cast<base_structures::Cell>(
                                                std::make_shared<base_structures::Road>(x, y));
            this->addChild(this->Map.cell_arr[x][y]->sprite_, 0);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_4:
            this->removeChild(this->Map.cell_arr[x][y]->sprite_);
            this->Map.cell_arr[x][y] = std::static_pointer_cast<base_structures::Cell>(
                                                std::make_shared<base_structures::Basement>(x, y));
            this->addChild(this->Map.cell_arr[x][y]->sprite_, 0);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_ENTER:
            switch(Map.cell_arr[x][y]->getType()) {
                case base_structures::CASTLE: {
                    auto castle = std::dynamic_pointer_cast<base_structures::Castle>(Map.cell_arr[x][y]);
                    auto editBoxHP = ui::EditBox::create(Size(100, 40), ui::Scale9Sprite::create("editBox_back.png"));
                    editBoxHP->setFont("fonts/arial.ttf", 50);
                    editBoxHP->setFontColor(cocos2d::Color3B::BLACK);
                    editBoxHP->setPlaceHolder("HP:");
                    editBoxHP->setPlaceholderFontColor(cocos2d::Color3B::BLACK);
                    editBoxHP->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height /2));
                    editBoxHP->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
                    editBoxHP->setMaxLength(6);
                    editBoxHP->setDelegate(this);
                    auto editBoxGold = ui::EditBox::create(Size(100, 40), ui::Scale9Sprite::create("editBox_back.png"));
                    editBoxGold->setFontSize(24);
                    editBoxGold->setFontColor(cocos2d::Color3B::BLACK);
                    editBoxGold->setPlaceHolder("Gold:");
                    editBoxGold->setPlaceholderFontColor(cocos2d::Color3B::BLACK);
                    editBoxGold->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height /2 - editBoxHP->getContentSize().height));
                    this->addChild(editBoxGold, 3);
                    this->addChild(editBoxHP, 3);
                    break;
                }
                case base_structures::DANGEON: {
                    UICustom::Popup *popup = UICustom::Popup::createAsConfirmDialogue("Test 1",
                                                                                      "This is a confirmation Popup",
                                                                                      [=]() {
                                                                                          log("Ok is pressed");
                                                                                      });
                    this->addChild(popup, 4);
                    break;
                }
                default:
                    break;
            }
    }
}

void MapConstructor::editBoxEditingDidEndWithAction(cocos2d::ui::EditBox* editBox, EditBoxEndAction action) {
}
