//
//  ItemSprite.h
//  SweetBang
//
//  Item元素定义
//
//
#pragma once
#ifndef __LittleKnight__ItemSprite__
#define __LittleKnight__ItemSprite__

#include "cocos2d.h"

USING_NS_CC;

// tile中的基础元素类型
enum BasicItemType {
    Croissant,
    Cupcake,
    Danish,
    Donut,
    Macaroon,
    SugarCookie
};

class ItemSprite : public Sprite {
    
public:
    CREATE_FUNC(ItemSprite);
    virtual bool init();
    static ItemSprite* createBasicItem(BasicItemType itemType);
    
    int getItemType();
    void setItemType(int itemType);
    
private:
    int itemType;
    
};

#endif /* defined(__LittleKnight__ItemSprite__) */
