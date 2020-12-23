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

#include "MenuScene.h"
#include "base_structures.h"

USING_NS_CC;

Scene* ::Menu::createScene()
{
    return ::Menu::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool ::Menu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto background = Sprite::create("MenuBackground.png");

    if (background == nullptr ||
        background->getContentSize().width <= 0 ||
        background->getContentSize().height <= 0)
    {
        problemLoading("'MenuBackground.png'");
    }
    else
    {
        background->setContentSize(Size(visibleSize.width, visibleSize.height));
        float x = origin.x + visibleSize.width / 2;
        float y = origin.y + visibleSize.height / 2;
        background->setPosition(Vec2(x,y));
    }
    this->addChild(background, 0);

    auto startLabel = Label::createWithTTF("Start Game", "fonts/Marker Felt.ttf", 40);
    if (startLabel == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    auto startItem = MenuItemLabel::create(
                            startLabel,
                            CC_CALLBACK_1(::Menu::menuStartGameCallback, this));

    if (startItem == nullptr)
    {
        problemLoading("startLabel");
    }
    else
    {
        startItem->setPosition(Vec2(origin.x + visibleSize.width/2,
                                     origin.y + visibleSize.height / 2));
    }

    auto createLabel = Label::createWithTTF("Create game level", "fonts/Marker Felt.ttf", 40);
    if (createLabel == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    auto createItem = MenuItemLabel::create(
            createLabel,
            CC_CALLBACK_1(::Menu::menuCreateCallback, this));

    if (createItem == nullptr ||
        createItem->getContentSize().width <= 0 ||
        createItem->getContentSize().height <= 0)
    {
        problemLoading("createLabel");
    }
    else
    {
        createItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                    origin.y + visibleSize.height / 2 - (startItem->getContentSize().height * 1.2)));
    }

    auto closeItem = MenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            CC_CALLBACK_1(::Menu::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = cocos2d::Menu::create(startItem, createItem, closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 2);

    return true;
}

void ::Menu::menuStartGameCallback(cocos2d::Ref *pSender) {
    auto choose_level = ::ChooseLevelScene::createScene();
    cocos2d::Director::getInstance()->replaceScene(choose_level);
}


void ::Menu::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
}

void ::Menu::menuCreateCallback(cocos2d::Ref *pSender) {
    auto create = ::MapConstructor::createScene();
    cocos2d::Director::getInstance()->replaceScene(create);
}
