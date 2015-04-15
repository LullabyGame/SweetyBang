//
//  SpecialSprite.cpp
//  SweetyBang
//
//  Created by murreywang on 15/4/14.
//
//

#include "SpecialSprite.h"

/**
 * 默认init方法
 */
bool SpecialSprite::init() {
    if (!Sprite::init()) {
        return false;
    }
    return true;
}

/**
 *  创建基础类Item
 *
 *  @param itemType BasicItem类型
 *  @return 创建的Item元素
 */
SpecialSprite* SpecialSprite::createSpecialItem(SpecialItemType itemType) {
    SpecialSprite *item = new SpecialSprite();
    if (item && item->init()) {
        switch (itemType) {
            case SpecialItemType::ForkAcross:
                item->initWithSpriteFrameName("ForkAcross.png");
                item->setItemType(SpecialItemType::ForkAcross);
                break;
            case SpecialItemType::ForkVertical:
                item->initWithSpriteFrameName("ForkVertical.png");
                item->setItemType(SpecialItemType::ForkVertical);
                break;
        }
        item->autorelease();
        return item;
    }
    CC_SAFE_DELETE(item);
    return NULL;
}

int SpecialSprite::getItemType() {
    return this->itemType;
}

void SpecialSprite::setItemType(int itemType) {
    this->itemType = itemType;
}

int SpecialSprite::getItemSpecialType(){
    return this->itemSpecialType;
}

void SpecialSprite::setItemSpecialType(int itemSpecialType){
    this->itemSpecialType = itemSpecialType;
}

std::string SpecialSprite::getItemSpecialNomber(){
    return this->itemSpecialNomber;
}

void SpecialSprite::setItemSpecialNomber(std::string itemSpecialNomber){
    this->itemSpecialNomber = itemSpecialNomber;
}

int SpecialSprite::getItemStatus() {
    return this->itemStatus;
}

void SpecialSprite::setItemStatus(int itemStatus) {
    this->itemStatus = itemStatus;
}