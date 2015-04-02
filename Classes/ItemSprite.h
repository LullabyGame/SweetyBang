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

/* 区别特殊元素的性质 */
enum SpecialType{
    Aommon,
    Across,
    Vertical,
    Region
};

class ItemSprite : public Sprite {
    
public:
    CREATE_FUNC(ItemSprite);
    virtual bool init();
    static ItemSprite* createBasicItem(BasicItemType itemType);
    
    int getItemType();
    void setItemType(int itemType);
    int getItemSpecialType();
    void setItemSpecialType(int itemSpecialType);
    std::string getItemSpecialNomber();
    void setItemSpecialNomber(std::string itemSpecialNomber);
    
private:
    int itemType;
    int itemSpecialType; //存储特殊元素的类型，1、竖，2、横
    std::string itemSpecialNomber;//存储特殊元素的编码
    
};

#endif /* defined(__LittleKnight__ItemSprite__) */
