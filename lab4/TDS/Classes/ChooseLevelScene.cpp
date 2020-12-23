//
// Created by copiedwonder on 20.12.2020.
//

#include "ChooseLevelScene.h"
#include "GameScene.h"
#include "MenuScene.h"
#include <vector>
#include <string>
#include <experimental/filesystem>
#include "ui/CocosGUI.h"

static const std::string SAVE_PATH = "/home/copiedwonder/OOP_Labs/lab4/saves/";

USING_NS_CC;

Scene* ChooseLevelScene::createScene()
{
    return ChooseLevelScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool ChooseLevelScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    std::vector<std::string> levels = getLevelNames(SAVE_PATH);
    for (const auto& level_name : levels) {
        auto button = cocos2d::ui::Button::create("button_normal.png", "button_selected.png");

        button->setTitleText(level_name);
        button->setTitleColor(Color3B::WHITE);
        button->setTitleFontSize(26);
        int index = std::find(levels.begin(), levels.end(), level_name) - levels.begin();
        button->setPosition(cocos2d::Vec2(visibleSize.width/2,
                                          visibleSize.height - 100 - index * button->getContentSize().height * 1.1));

        button->addTouchEventListener([level_name](Ref* sender, cocos2d::ui::Widget::TouchEventType type){
            switch (type)
            {
                case ui::Widget::TouchEventType::BEGAN:
                    break;
                case ui::Widget::TouchEventType::ENDED:
                    Director::getInstance()->replaceScene(::Game::createScene(level_name));
                    break;
                default:
                    break;
            }
        });

        auto key_listener = EventListenerKeyboard::create();
        key_listener->onKeyReleased = CC_CALLBACK_2(ChooseLevelScene::onKeyReleased, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(key_listener, this);

        this->addChild(button, 1);
    }
    return true;
}


std::vector<std::string> getLevelNames(std::string dir_path) {
    std::vector<std::string> ret;
    for (const auto& entry : std::experimental::filesystem::directory_iterator(dir_path)) {
        ret.push_back(entry.path().filename().string());
        log("Entry added: %s", entry.path().string().c_str());
    }
    return ret;
}


void ChooseLevelScene::menuBackCallback(Ref* pSender)
{

}

void ChooseLevelScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
    switch(keyCode) {
        case cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE: {
            Director::getInstance()->replaceScene(::Menu::create());
            break;
        }
        default:
            break;
    }
}
