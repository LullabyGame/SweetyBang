//
// 控制关卡图片显示位置
//
//

#include "LevelSelectContent.h"

LevelSelectContent::LevelSelectContent() {
    
}
LevelSelectContent::~LevelSelectContent() {
    
}

bool LevelSelectContent::init() {
    if ( !Layer::init() ){
        return false;
    }
    return true;
}

/**
 绘制关卡节点，目前只画了两个，可以放到配置文件中
 */
void LevelSelectContent::initAllLevels() {
    this->removeAllChildren();
    
    levelMenu = Menu::create();
    this->addChild(levelMenu);
    
    auto oneLevelItem = LevelSelectItem::create(1);
    levelMenu->addChild(oneLevelItem);
    oneLevelItem->setPosition(350, 150);
    
    levelMenu2 = Menu::create();
    this->addChild(levelMenu2);
    
    auto oneLevelItem2 = LevelSelectItem::create(2);
    levelMenu2->addChild(oneLevelItem2);
    oneLevelItem2->setPosition(450, 250);
    
    levelMenu->setPosition(0, 0);
    levelMenu->setOpacity(0);
    auto fadeAction = FadeIn::create(0.5);
    levelMenu->runAction(fadeAction);
    
    levelMenu2->setPosition(0, 0);
    levelMenu2->setOpacity(0);
    auto fadeAction2 = FadeIn::create(0.5);
    levelMenu2->runAction(fadeAction2);
}