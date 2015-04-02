//
//  ItemSprite.cpp
//  SweetBang
//
//  Created by JinTongyao on 1/22/15.
//
//

#include "ItemSprite.h"

/**
 * 默认init方法
 */
bool ItemSprite::init() {
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
ItemSprite* ItemSprite::createBasicItem(BasicItemType itemType) {
    ItemSprite *item = new ItemSprite();
    if (item && item->init()) {
        switch (itemType) {
            case BasicItemType::Croissant:
                item->initWithSpriteFrameName("Croissant.png");
                item->setItemType(BasicItemType::Croissant);
                break;
            case BasicItemType::Cupcake:
                item->initWithSpriteFrameName("Cupcake.png");
                item->setItemType(BasicItemType::Cupcake);
                break;
            case BasicItemType::Danish:
                item->initWithSpriteFrameName("Danish.png");
                item->setItemType(BasicItemType::Danish);
                break;
            case BasicItemType::Donut:
                item->initWithSpriteFrameName("Donut.png");
                item->setItemType(BasicItemType::Donut);
                break;
            case BasicItemType::Macaroon:
                item->initWithSpriteFrameName("Macaroon.png");
                item->setItemType(BasicItemType::Macaroon);
                break;
            case BasicItemType::SugarCookie:
                item->initWithSpriteFrameName("SugarCookie.png");
                item->setItemType(BasicItemType::SugarCookie);
                break;
        }
        item->autorelease();
        return item;
    }
    CC_SAFE_DELETE(item);
    return NULL;
}


/*Get & Set方法*/

int ItemSprite::getItemType() {
    return this->itemType;
}

void ItemSprite::setItemType(int itemType) {
    this->itemType = itemType;
}

int ItemSprite::getItemSpecialType(){
    return this->itemSpecialType;
}

void ItemSprite::setItemSpecialType(int itemSpecialType){
    this->itemSpecialType = itemSpecialType;
}

std::string ItemSprite::getItemSpecialNomber(){
    return this->itemSpecialNomber;
}

void ItemSprite::setItemSpecialNomber(std::string itemSpecialNomber){
    this->itemSpecialNomber = itemSpecialNomber;
}
