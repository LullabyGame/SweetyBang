//
//  SpecialSprite.h
//  SweetyBang
//
//  Created by murreywang on 15/4/14.
//
//

#ifndef __SweetyBang__SpecialSprite__
#define __SweetyBang__SpecialSprite__

#include "cocos2d.h"

USING_NS_CC;

// tile中的基础元素类型
enum SpecialItemType {
    ForkAcross,
    ForkVertical
};

class SpecialSprite : public Sprite{
    
public:
    CREATE_FUNC(SpecialSprite);
    virtual bool init();
    static SpecialSprite* createSpecialItem(SpecialItemType itemType);
    
    int getItemType();
    void setItemType(int itemType);
    int getItemSpecialType();
    void setItemSpecialType(int itemSpecialType);
    std::string getItemSpecialNomber();
    void setItemSpecialNomber(std::string itemSpecialNomber);
    int getItemStatus();
    void setItemStatus(int itemStatus);
    
private:
    int itemType;
    int itemSpecialType; //存储特殊元素的类型，7、竖，6、横
    std::string itemSpecialNomber;//存储特殊元素的编码
    int itemStatus;//存储特殊元素是否可用的状态。0、可用，1、不可用

};

#endif /* defined(__SweetyBang__SpecialSprite__) */
