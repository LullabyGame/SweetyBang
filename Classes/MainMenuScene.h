//
//  MainMenuScene.h
//  SweetBang
//
//  主菜单Scene定义
//
//
#pragma once
#ifndef __LittleKnight__MainMenuScene__
#define __LittleKnight__MainMenuScene__

#include "cocos2d.h"
#include "Settings.h"
#include "NormalModeScene.h"
#include "LevelSelectContent.h"

USING_NS_CC;

class MainMenuScene : public Layer {
    
public:
    CREATE_FUNC(MainMenuScene);
    virtual bool init();
    static Scene* createScene();
    
private:
    void menuNormalModeCallback(Ref *sender);
    LevelSelectContent* levelSelectContent;
    void initAllLevels();
    
};

#endif /* defined(__LittleKnight__MainMenuScene__) */
