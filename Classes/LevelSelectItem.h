
#ifndef __PageTest__LevelSelectItem__
#define __PageTest__LevelSelectItem__

#include "cocos2d.h"
#include "Settings.h"

#include "NormalModeScene.h"
#include <string>
USING_NS_CC;



class LevelSelectItem : public MenuItemSprite {
    CC_SYNTHESIZE(int, _level, Level);
    CC_SYNTHESIZE(LevelItemType, _type, Type);
public:
    LevelSelectItem(int level);
    static LevelSelectItem* create(int level);
private:
    void selectCallBack(Ref* sender);
    void extraInit();
    std::string getFrameNameByType(const LevelItemType& type);
};

#endif /* defined(__PageTest__LevelSelectItem__) */
