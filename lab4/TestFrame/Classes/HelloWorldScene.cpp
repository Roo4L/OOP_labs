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

#include "HelloWorldScene.h"
#include "gtest/gtest.h"
#include "base_structures.h"
#include <fstream>
#include <string>

namespace base_structures {
    TEST(MapTest, CellConstructor) {
        Cell a(0, 0);
        EXPECT_EQ(BASE_CELL, a.getType());
        Cell b(1, 30);
        EXPECT_EQ(BASE_CELL, b.getType());
    }

    TEST(MapTest, CastleConsturctor) {
        Castle a(0, 0);
        EXPECT_EQ(CASTLE, a.getType());
        EXPECT_EQ(200, a.getHp());
        EXPECT_EQ(100, a.getGold());
        Castle b(0, 0, 1304, 1000);
        EXPECT_EQ(CASTLE, b.getType());
        EXPECT_EQ(1304, b.getHp());
        EXPECT_EQ(1000, b.getGold());
        EXPECT_THROW(Castle c(0, 0, -4, 1000), std::invalid_argument);
    }

    TEST(MapTest, CastleIncome) {
        Castle a(0, 0);
        Monster m;
        a.income(m);
        EXPECT_EQ(CASTLE, a.getType());
        EXPECT_EQ(150, a.getGold());
        EXPECT_EQ(200, a.getHp());
        Castle b(0, 0, 1304, 1000);
        Monster mm(100, 100, 120, WEEK);
        b.income(mm);
        EXPECT_EQ(CASTLE, b.getType());
        EXPECT_EQ(1304, b.getHp());
        EXPECT_EQ(1120, b.getGold());
    }

    TEST(MapTest, CastleDoDamage) {
        Castle a(0, 0);
        Monster m;
        a.doDamage(m);
        EXPECT_EQ(CASTLE, a.getType());
        EXPECT_EQ(100, a.getGold());
        EXPECT_EQ(100, a.getHp());
        Castle b(0, 0, 1304, 1000);
        Monster mm(1100, 100, 120, WEEK);
        b.doDamage(mm);
        EXPECT_EQ(CASTLE, b.getType());
        EXPECT_EQ(204, b.getHp());
        EXPECT_EQ(1000, b.getGold());
        Castle c(0, 0, 50, 50);
        Monster mmm(100, 100, 100, WEEK);
        c.doDamage(mmm);
        EXPECT_EQ(CASTLE, c.getType());
        EXPECT_EQ(-50, c.getHp());
        EXPECT_EQ(50, c.getGold());
    }

    TEST(MapTest, DangeonConstructor) {
        Dangeon a(0, 0);
        EXPECT_EQ(DANGEON, a.getType());
        EXPECT_EQ(-1, a.getCurWaveNum());
        EXPECT_TRUE(a.isActive());
        std::ofstream obuf("/home/copiedwonder/OOP_Labs/lab4/TestFrame/FileSamples/DangeonConstructorTest.bin", std::ios::binary);
        int k = 0;
        MonsterDescriptor desc = {100, 100, 100, WEEK};
        double spawn_time = 3.;
        obuf.write((char *) &k, sizeof(int));
        obuf.write((char *) &desc, sizeof(MonsterDescriptor));
        obuf.write((char *) &spawn_time, sizeof(double));
        k = 100;
        if (!obuf.write((char *) &k, sizeof(int)))
            throw std::invalid_argument("DangeonContructorTest.bin can't be created.");
        obuf.close();
        std::ifstream ibuf("/home/copiedwonder/OOP_Labs/lab4/TestFrame/FileSamples/DangeonConstructorTest.bin", std::ios::binary);
        if (!ibuf.is_open())
            throw std::invalid_argument("Test file doesn't exist or currupted.");
        Dangeon b(0, 0, ibuf);
        EXPECT_EQ(DANGEON, b.getType());
        EXPECT_EQ(-1, b.getCurWaveNum());
        EXPECT_TRUE(b.isActive());
        ibuf.close();
        //TODO Check monster in waves
    }

    TEST(MapTest, RoadConstructor) {
        Road a(0, 0);
        EXPECT_EQ(ROAD, a.getType());
        EXPECT_EQ(nullptr, a.getNext());
        EXPECT_EQ(cocos2d::Vec2(0, 0), a.getDirection());
        std::shared_ptr<Road> n = std::make_shared<Road>(1, 0);
        Road b(0, 0, n);
        EXPECT_EQ(ROAD, b.getType());
        EXPECT_EQ(n, b.getNext());
        EXPECT_EQ(cocos2d::Vec2(1, 0), b.getDirection());
    }

    TEST(MapTest, RoadSetUnit) {
        Road a(0, 0);
        std::shared_ptr<MagicTrap> u = std::dynamic_pointer_cast<MagicTrap>(a.setUnit());
        EXPECT_EQ(ROAD, a.getType());
        EXPECT_EQ(nullptr, a.getNext());
        EXPECT_EQ(cocos2d::Vec2(0, 0), a.getDirection());
        EXPECT_TRUE(a.isBusy());
        EXPECT_EQ(u->getType(), FROZEN);
        EXPECT_EQ(u->getEffectLevel(), 0);
        EXPECT_EQ(u->getLevel(), 0);
    }
    TEST(MapTest, BasementConstructor) {
        Basement a(0, 0);
        EXPECT_EQ(BASEMENT, a.getType());
    }
    TEST(MapTest, BasementSetUnit) {
        Basement a(0, 0);
        std::shared_ptr<Tower> u = std::dynamic_pointer_cast<Tower>(a.setUnit());
        EXPECT_EQ(BASEMENT, a.getType());
        EXPECT_TRUE(a.isBusy());
        EXPECT_EQ(u->getLevel(), 0);
    }

    TEST(MapTest, MapLoad) {
        Map_ Map;
        Map.cell_arr.resize(2);
        Map.cell_arr[0].emplace_back(std::make_shared<Cell>(0, 0));
        Map.castle = std::make_shared<Castle>(1, 0, 150, 50);
        Map.cell_arr[1].emplace_back(Map.castle);
        if (Map.save("/home/copiedwonder/OOP_Labs/lab4/TestFrame/FileSamples/test_map_1.bin") != 0)
            throw std::invalid_argument("Couldn't make save file.");
        Map_ Map2;
        if (Map2.load("/home/copiedwonder/OOP_Labs/lab4/TestFrame/FileSamples/test_map_1.bin") != 0)
            throw std::invalid_argument("Save not found or currupted.");
        EXPECT_EQ(Map2.cell_arr[0][0]->getType(), BASE_CELL);
        EXPECT_EQ(Map2.cell_arr[1][0]->getType(), CASTLE);
        EXPECT_EQ(Map2.castle->getHp(), 150);
        EXPECT_EQ(Map2.castle->getGold(), 50);
    }

    TEST(MonsterTest, MonsterConstructor) {
        Monster a;
        EXPECT_EQ(a.getHP(), 100);
        EXPECT_EQ(a.getCost(), 50);
        EXPECT_EQ(a.getSpeed(), 40);
        EXPECT_EQ(a.getModel(), WEEK);
        EXPECT_EQ(a.getRelation(), nullptr);
        Monster b(1000, 203, 200, STRONG);
        EXPECT_EQ(b.getHP(), 1000);
        EXPECT_EQ(b.getCost(), 200);
        EXPECT_EQ(b.getSpeed(), 203);
        EXPECT_EQ(b.getModel(), STRONG);
        EXPECT_EQ(b.getRelation(), nullptr);
    }

    TEST(MonsterTest, MonsterSetRelation) {
        Monster a;
        std::shared_ptr<Road> b = std::make_shared<Road>(0, 0);
        a.setRelation(b);
        EXPECT_EQ(a.getRelation(), b);
        EXPECT_EQ(a.getHP(), 100);
        EXPECT_EQ(a.getCost(), 50);
        EXPECT_EQ(a.getSpeed(), 40);
        EXPECT_EQ(a.getModel(), WEEK);
    }

    TEST(MonsterTest, MonsterGetDamage) {
        Monster a;
        a.getDamage(30);
        EXPECT_EQ(a.getRelation(), nullptr);
        EXPECT_EQ(a.getHP(), 70);
        EXPECT_EQ(a.getCost(), 50);
        EXPECT_EQ(a.getSpeed(), 40);
        EXPECT_EQ(a.getModel(), WEEK);
    }

    TEST(MonsterTest, MonsterApplyDebuf) {
        Monster a;
        a.applyDebuf({std::chrono::duration<double>(3), 0.25, FROZEN});
        //TODO check wheither debuf is applied
    }

    TEST(UnitTest, TowerConstructor) {
        Tower a(3, 4);
        EXPECT_EQ(a.getStyle(), CLOSESTCASTLE);
        EXPECT_EQ(a.getLevel(), 0);
        EXPECT_EQ(a.sprite_->getPositionX(), 3 * 40);
        EXPECT_EQ(a.sprite_->getPositionY(), 4 * 40);
    }

    TEST(UnitTest, TowerGetDistance) {
        Tower a(3, 4);
        Monster x;
        x.sprite_->setPosition({40, 100});
        EXPECT_DOUBLE_EQ(a.getDistance(x), 100);
    }
    TEST(UnitTest, TowerSetStyle) {
        Tower a(0, 0);
        a.setStyle(CLOSESTTOWER);
        EXPECT_EQ(a.getStyle(), CLOSESTTOWER);
    }
    TEST(UnitTest, TowerIsReachable) {
        Tower a(3, 4);
        Monster x, y, z;
        x.sprite_->setPosition({50, 100});
        y.sprite_->setPosition({30, 100});
        z.sprite_->setPosition({40, 100});
        EXPECT_TRUE(a.isReachable(x));
        EXPECT_FALSE(a.isReachable(y));
        EXPECT_TRUE(a.isReachable(z));
    }
    TEST(UnitTest, TowerIsUpgradable) {
        Tower a(0 ,0);
        EXPECT_TRUE(a.isUpgradable());
        Tower b(0, 0, TOWER_DESCR.size() - 1);
        EXPECT_FALSE(b.isUpgradable());
    }
    TEST(UnitTest, TowerUpgrade) {
        Tower a(0, 1);
        EXPECT_EQ(a.Upgrade(), 0);
        EXPECT_EQ(a.getLevel(), 1);
        EXPECT_EQ(a.Upgrade(), -1);
        EXPECT_EQ(a.getLevel(), 1);
    }
    TEST(UnitTest, TowerAttack) {
        Tower a(3, 4);
        a.setStyle(CLOSESTTOWER);
        MonsterTable_ mt;
        std::shared_ptr<Monster> m = std::make_shared<Monster>();
        m->sprite_->setPosition({50, 100});
        mt.push_back(m);
        m = std::make_shared<Monster>();
        m->sprite_->setPosition({60, 100});
        mt.push_back(m);
        m = a.Attack(mt);
        EXPECT_EQ(m, mt.back());
        EXPECT_EQ(m->getHP(), 90);
    }
    TEST(UnitTest, MagicTowerIsEffectUpgradable) {
        MagicTower a(Tower(3, 4), FROZEN);
        EXPECT_TRUE(a.isEffectUpgradable());
        MagicTower b(Tower(0, 0), FROZEN, MAGICTOWER_DESCR.size() -1);
        EXPECT_FALSE(b.isEffectUpgradable());
    }
    TEST(UnitTest, MagicTowerUpgradeEffect) {
        MagicTower a(Tower(3, 4), FROZEN);
        EXPECT_EQ(a.UpgradeEffect(), 0);
        EXPECT_EQ(a.getEffectLevel(), 1);
        EXPECT_EQ(a.UpgradeEffect(), -1);
        EXPECT_EQ(a.getEffectLevel(), 1);
    }

    TEST(UnitTest, MagicTrapConstructor) {
        MagicTrap a(3, 4, FROZEN);
        EXPECT_EQ(a.getEffectLevel(), 0);
        EXPECT_EQ(a.getLevel(), 0);
        EXPECT_EQ(a.getType(), FROZEN);
        MagicTrap b(0, 0, POISON, 1, 1);
        EXPECT_EQ(b.getEffectLevel(), 1);
        EXPECT_EQ(b.getType(), POISON);
        EXPECT_EQ(b.getLevel(), 1);
    }

    TEST(UnitTest, MagicTrapIsUpgradable) {
        MagicTrap a(3, 4, FROZEN);
        EXPECT_TRUE(a.isUpgradable());
        MagicTrap b(0, 0, EXHAUST, 0,TRAP_DESCR.size() - 1);
        EXPECT_FALSE(b.isUpgradable());
    }
    TEST(UnitTest, MagicTrapUpgrade) {
        MagicTrap a(0, 1, FROZEN);
        EXPECT_EQ(a.Upgrade(), 0);
        EXPECT_EQ(a.getLevel(), 1);
        EXPECT_EQ(a.Upgrade(), -1);
        EXPECT_EQ(a.getLevel(), 1);
    }
    TEST(UnitTest, MagicTrapIsEffectUpgradable) {
        MagicTrap a(0, 0, FROZEN);
        EXPECT_TRUE(a.isEffectUpgradable());
        MagicTrap b(0, 0, FROZEN, MAGICTOWER_DESCR.size() -1);
        EXPECT_FALSE(b.isEffectUpgradable());
    }
    TEST(UnitTest, MagicTrapUpgradeEffect) {
        MagicTrap a(0, 1, FROZEN);
        EXPECT_EQ(a.UpgradeEffect(), 0);
        EXPECT_EQ(a.getEffectLevel(), 1);
        EXPECT_EQ(a.UpgradeEffect(), -1);
        EXPECT_EQ(a.getEffectLevel(), 1);
    }
    TEST(UnitTest, MagicTrapIsReachable) {
        MagicTrap a(3, 4, FROZEN);
        Monster x, y, z;
        x.sprite_->setPosition({80, 130});
        y.sprite_->setPosition({90, 130});
        z.sprite_->setPosition({70, 130});
        EXPECT_TRUE(a.isReachable(x));
        EXPECT_TRUE(a.isReachable(y));
        EXPECT_FALSE(a.isReachable(z));
    }
}

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

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

    auto testItem = MenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            CC_CALLBACK_1(HelloWorld::menuTestCallback, this));

    if (testItem == nullptr ||
        testItem->getContentSize().width <= 0 ||
        testItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + testItem->getContentSize().width/2;
        float y = origin.y + testItem->getContentSize().height/2;
        testItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(testItem, closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");
    if (sprite == nullptr)
    {
        problemLoading("'HelloWorld.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
    }
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void HelloWorld::menuTestCallback(cocos2d::Ref *pSender) {
    ::testing::InitGoogleTest();
    RUN_ALL_TESTS();
}
