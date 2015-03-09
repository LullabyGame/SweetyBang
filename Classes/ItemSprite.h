//
//  ItemSprite.h
//  LittleKnight
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

// tile中的Boss元素类型
enum BossItemType {
    boss1,
    boss2,
    boss3
};

// 技能元素类型
enum SkillItemType {
    skill1,
    skill2,
    skill3,
    skill4,
    skill5
};

class ItemSprite : public Sprite {
    
public:
    CREATE_FUNC(ItemSprite);
    virtual bool init();
    static ItemSprite* createBasicItem(BasicItemType itemType);
    static ItemSprite* createBossItem(BossItemType itemType);
    
    int getItemType();
    void setItemType(int itemType);
    
private:
    int itemType;
    
};

#endif /* defined(__LittleKnight__ItemSprite__) */
