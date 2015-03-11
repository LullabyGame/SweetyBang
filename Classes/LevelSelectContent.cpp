//
//  LevelSelectContent.cpp
//  PageTest
//
//  Created by wx5 on 12/31/14.
//
//

#include "LevelSelectContent.h"

LevelSelectContent::LevelSelectContent(){
    
}
LevelSelectContent::~LevelSelectContent(){
    
}


bool LevelSelectContent::init(){
    if ( !Layer::init() ){
        return false;
    }
    
    //initAllLevels();
    return true;
}

/**
 绘制关卡节点，目前只画了两个，可以放到配置文件中
 */
void LevelSelectContent::initAllLevels(int page){
    this->removeAllChildren();
    
    _levelMenu = Menu::create();
    this->addChild(_levelMenu);
    
    auto oneLevelItem = LevelSelectItem::create(1);
    _levelMenu->addChild(oneLevelItem);
    oneLevelItem->setPosition(350, 150);
    
    _levelMenu2 = Menu::create();
    this->addChild(_levelMenu2);
    
    auto oneLevelItem2 = LevelSelectItem::create(2);
    _levelMenu2->addChild(oneLevelItem2);
    oneLevelItem2->setPosition(450, 250);
    
    _levelMenu->setPosition(0, 0);
    _levelMenu->setOpacity(0);
    auto fadeAction = FadeIn::create(0.5);
    _levelMenu->runAction(fadeAction);
    
    _levelMenu2->setPosition(0, 0);
    _levelMenu2->setOpacity(0);
    auto fadeAction2 = FadeIn::create(0.5);
    _levelMenu2->runAction(fadeAction2);
}