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
    switch (this->mode) {
        case INSERT:
            log("CellSelected: (%d,%d)", x, y);
            this->CellSelected->setPosition(this->Map.cell_arr[x][y]->sprite_->getPosition());
            break;
        case WAY: {
            int x_prev = this->CellSelected->getPositionX() / TILE_WIDTH;
            int y_prev = this->CellSelected->getPositionY() / TILE_WIDTH;
            log("CellSelected: (%d,%d)", x, y);
            if (x_prev != x || y_prev != y) {
                this->CellSelected->setPosition(this->Map.cell_arr[x][y]->sprite_->getPosition());
                switch (this->Map.cell_arr[x][y]->getType()) {
                    case base_structures::ROAD: {
                        switch (this->Map.cell_arr[x_prev][y_prev]->getType()) {
                            case base_structures::ROAD: {
                                std::dynamic_pointer_cast<base_structures::Road>(Map.cell_arr[x_prev][y_prev])->setNext(
                                        std::dynamic_pointer_cast<base_structures::Road>(Map.cell_arr[x][y])
                                );
                                break;
                            }
                            case base_structures::DANGEON: {
                                std::dynamic_pointer_cast<base_structures::Dangeon>(Map.cell_arr[x_prev][y_prev])->setNext(
                                        std::dynamic_pointer_cast<base_structures::Road>(Map.cell_arr[x][y])
                                );
                                break;
                            }
                        }
                        auto draw = cocos2d::DrawNode::create();
                        draw->drawLine(Point(x_prev * TILE_WIDTH, y_prev * TILE_WIDTH),
                                       Point(x * TILE_WIDTH, y * TILE_WIDTH), Color4F::WHITE);
                        this->addChild(draw, 2);
                        break;
                    }
                    case base_structures::CASTLE: {
                        std::dynamic_pointer_cast<base_structures::Road>(Map.cell_arr[x_prev][y_prev])->setNext(nullptr);
                        this->mode = INSERT;
                        break;
                    }
                }
            }
            break;
        }
        default:
            break;
    }
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
            this->addChild(this->Map.cell_arr[x][y]->sprite_, 1);
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
        case cocos2d::EventKeyboard::KeyCode::KEY_ALT: {
            if (Map.cell_arr[x][y]->getType() == base_structures::DANGEON) {
                if (this->mode == INSERT) {
                    this->mode = WAY;
                }
                else {
                    this->mode = INSERT;
                }
            }
            break;
        }
        case cocos2d::EventKeyboard::KeyCode::KEY_S: {
            log("Save key was pressed.");
            UICustom::MapSavePopup* dialog = UICustom::MapSavePopup::create(this->Map);
            this->pause();
            _eventDispatcher->pauseEventListenersForTarget(this);
            this->addChild(dialog, 4);
            break;
        }
        case cocos2d::EventKeyboard::KeyCode::KEY_ENTER:
            switch(Map.cell_arr[x][y]->getType()) {
                case base_structures::CASTLE: {
                    break;
                }
                case base_structures::DANGEON: {
                    std::shared_ptr<base_structures::Dangeon> dang = std::dynamic_pointer_cast<base_structures::Dangeon>(Map.cell_arr[x][y]);
                    UICustom::DangeonMenu* dialog = UICustom::DangeonMenu::create(dang);
                    this->pause();
                    _eventDispatcher->pauseEventListenersForTarget(this);
                    this->addChild(dialog, 4);
                    break;
                }
                case base_structures::ROAD: {
                    break;
                }
                default:
                    break;
            }
    }
}



void MapConstructor::editBoxEditingDidEndWithAction(cocos2d::ui::EditBox* editBox, EditBoxEndAction action) {
}



