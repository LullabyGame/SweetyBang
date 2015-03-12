//
// 控制关卡状态显示
//

#include "LevelSelectItem.h"

//#include "GameSound.h"

LevelSelectItem::LevelSelectItem(int level):_level(level),_type(kLockLevel) {
    if(level > (g_passLevelCount + 1)){
        this->setEnabled(false);
        _type = kLockLevel;
    }else if(level == (g_passLevelCount + 1)){
        this->setEnabled(true);
        _type = kNotPassYet;
    }else if(level < (g_passLevelCount + 1)){
        this->setEnabled(true);
        _type = kAlreadyPass;
    }
}

/**
 * 创建一个子菜单项
 *
 * @param normalSprite 一般情况下的精灵
 * @param selectedSprite 选中情况下的精灵
 * @param disabledSprite 不可用情况下的精灵
 */
LevelSelectItem* LevelSelectItem::create(int level) {
    LevelSelectItem *ret = new LevelSelectItem(level);
    auto callback = CC_CALLBACK_1(LevelSelectItem::selectCallBack, ret);
    
    auto normalSprite = Sprite::createWithSpriteFrameName(ret->getFrameNameByType(ret->_type));
    auto selectedSprite = Sprite::createWithSpriteFrameName(ret->getFrameNameByType(ret->_type));
    auto disabledSprite = Sprite::createWithSpriteFrameName(ret->getFrameNameByType(ret->_type));
    // 初始化子菜单项
    ret->initWithNormalSprite(normalSprite, selectedSprite, disabledSprite, callback);
    ret->extraInit();
    ret->autorelease();
    return ret;
}

/**
 * 后台显示点击的哪个关卡
 *
 */
void LevelSelectItem::selectCallBack(Ref* sender) {
    if(_type != kLockLevel){
        log("level item");
        log("start level:%d",_level);
        //跳转关卡
        Scene* normalModeScene = NormalModeScene::createScene(_level);
        Director::getInstance()->replaceScene(normalModeScene);
    }
}

/**
 * 根据关卡的状态添加数字图片
 *
 */
void LevelSelectItem::extraInit() {
    if(_type != kLockLevel){
        char levelBuffer[20] = {0};
        sprintf(levelBuffer, "%d", _level);
        Label* valueLabel = nullptr;
        if(_type == kAlreadyPass){
            valueLabel = Label::createWithBMFont("res/fonts/blueLevel.fnt", levelBuffer);
            
        }else if(_type == kNotPassYet){
            valueLabel = Label::createWithBMFont("res/fonts/whiteLevel.fnt", levelBuffer);
        }
        this->addChild(valueLabel);
        auto size = this->getContentSize();
        valueLabel->setPosition(size.width * 0.5, size.height * 0.5);
    }
}

/**
 * 获取关卡图片
 *
 */
std::string LevelSelectItem::getFrameNameByType(const LevelItemType& type) {
    switch (type) {
        case kLockLevel:
        {
            return "lockLevel.png";
        }
            break;
        case kNotPassYet:
        {
            return "notPassYet.png";
        }
            break;
        case kAlreadyPass:
        {
            return "alreadyPass.png";
        }
            break;
        default:
            break;
    }
}