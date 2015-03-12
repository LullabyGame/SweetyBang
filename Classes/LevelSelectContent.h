
#ifndef __PageTest__LevelSelectContent__
#define __PageTest__LevelSelectContent__

#include "cocos2d.h"
#include "LevelSelectItem.h"
USING_NS_CC;


class LevelSelectContent :public Layer {
public:
    LevelSelectContent();
    ~LevelSelectContent();
    virtual bool init();
    CREATE_FUNC(LevelSelectContent);
    void initAllLevels(ssize_t idx);
private:
    Menu* levelMenu;
};

#endif /* defined(__PageTest__LevelSelectContent__) */
