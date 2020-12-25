//
//  Popup.h
//  dreamWeek
//
//  Created by Bilal Mirza on 07/08/2017.
//
//

#ifndef Popup_h
#define Popup_h

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "base_structures.h"

/*
 -Displays a popup
 -Resizes itself
 -Automatically swallows rest of the touches
 -Exit is called if touch is outside Bg
 */

namespace UICustom {
    
    
    class PopupDelegates : public cocos2d::LayerRadialGradient
    {
        
    public:
        static PopupDelegates *create();
        
    protected:
        virtual bool init();
        virtual void setUpTouches();
        
        virtual void show(const bool animated = true);
        virtual void dismiss(const bool animated = true);
        void setAnimated(const bool animated){_isAnimated = animated;};
        
        cocos2d::ui::ImageView *_bg = nullptr;
    private:
        bool _isAnimated = false;
    };
    
    
    
    class Popup : public PopupDelegates
    {
        
    public:
        static Popup* createAsMessage(const std::string &title, const std::string &msg);
        static Popup* createAsConfirmDialogue(const std::string &title, const std::string &msg ,const std::function<void ()> &YesFunc);
        static Popup* create(const std::string &title, const std::string &msg, cocos2d::Label *lbl, const std::function<void ()> &YesFunc);
        
        
    protected:
        void initBg(const cocos2d::Size size,const std::string &title);
    };

    class DangeonMenu : public Popup {
        cocos2d::ui::TextField* HPfield;
        cocos2d::ui::TextField* ModelField;
        cocos2d::ui::TextField* SpawnField;
        cocos2d::ui::TextField* WaveField;
        cocos2d::Label* stats;


        void dismiss(const bool animated = true) override;
    public:
        static DangeonMenu* create(std::shared_ptr<base_structures::Dangeon> dang);
    };

    class MapSavePopup : public Popup {
        cocos2d::ui::TextField* nameField;

        void noDismiss(const bool animated = true);
        void yesDismiss(const bool animated = true);
    public:
        static MapSavePopup* create(base_structures::Map_& Map);
    };

    class LosePopup : public Popup {
        cocos2d::Label* label;

        void noDismiss(const bool animated = true);
        void yesDismiss(std::string level_name, const bool animated = true);
    public:
        static LosePopup* create(std::string level_name);
    };

    class WinPopup : public Popup {
        cocos2d::Label* label;

        void noDismiss(const bool animated = true);
        void yesDismiss(std::string level_name, const bool animated = true);
    public:
        static WinPopup* create(std::string level_name);
    };
}

#endif /* Popup_h */

