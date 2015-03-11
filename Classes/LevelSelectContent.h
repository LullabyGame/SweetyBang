//
//  LevelSelectContent.h
//  PageTest
//
//  Created by wx5 on 12/31/14.
//
//

#ifndef __PageTest__LevelSelectContent__
#define __PageTest__LevelSelectContent__

#include "cocos2d.h"
#include "LevelSelectItem.h"
USING_NS_CC;


class LevelSelectContent :public Layer{
public:
    LevelSelectContent();
    ~LevelSelectContent();
    virtual bool init();
    CREATE_FUNC(LevelSelectContent);
    void initAllLevels();
private:
    Menu* levelMenu;
    Menu* levelMenu2;
    
};

#endif /* defined(__PageTest__LevelSelectContent__) */
