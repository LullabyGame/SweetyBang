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
 * 绘制关卡节点,写死每页10关
 */
void LevelSelectContent::initAllLevels(ssize_t idx) {
//    this->removeAllChildren();
    int idxs = (int) idx; 
    //假设每页10关
    int firstNumber = idxs * 10 + 1;
    
    for (int i = 0; i < 10; i++ ) {
        levelMenu = Menu::create();
        this->addChild(levelMenu);
        
        auto oneLevelItem = LevelSelectItem::create(firstNumber + i);
        levelMenu->addChild(oneLevelItem);
        oneLevelItem->setPosition(350, 150 + i * 100);
        oneLevelItem->setScale(itemScale, itemScale);
        
        levelMenu->setPosition(0, 0);
        levelMenu->setOpacity(0);
        auto fadeAction = FadeIn::create(0.5);
        levelMenu->runAction(fadeAction);
    }
}