//
//  TileSprite.cpp
//  LittleKnight
//
//  Tile元素实现
//
//

#include "TileSprite.h"

/**
 * 默认init方法
 */
bool TileSprite::init() {
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 120))) {
        return false;
    }
    return true;
}


/**
 *  创建Tile
 *
 *  @param posX   x轴坐标
 *  @param posY   y轴坐标
 *  @param width  Tile宽
 *  @param height Tile高
 *  @return 创建的Tile元素
 */
TileSprite* TileSprite::createTile(float posX, float posY, float width, float height) {
    TileSprite *tile = new TileSprite();
    if (tile && tile->init()) {
        tile->setPosX(posX);
        tile->setPosY(posY);
        tile->setWidth(width);
        tile->setHeight(height);
        
        tile->setAnchorPoint(Vec2(0, 0));
        tile->setContentSize(Size(Vec2(tile->getWidth(), tile->getHeight())));
        tile->setPosition(Vec2(tile->getPosX(), tile->getPosY()));
        
        tile->autorelease();
        return tile;
    }
    CC_SAFE_DELETE(tile);
    return NULL;
}

/*Get & Set方法*/

int TileSprite::getArrayX() {
    return this->arrayX;
}

void TileSprite::setArrayX(int arrayX) {
    this->arrayX = arrayX;
}

int TileSprite::getArrayY() {
    return this->arrayY;
}

void TileSprite::setArrayY(int arrayY) {
    this->arrayY = arrayY;
}


float TileSprite::getPosX() {
    return this->posX;
}

void TileSprite::setPosX(float posX) {
    this->posX = posX;
}

void TileSprite::setPosY(float posY) {
    this->posY = posY;
}

float TileSprite::getPosY() {
    return this->posY;
}

float TileSprite::getWidth() {
    return this->width;
}

void TileSprite::setWidth(float width) {
    this->width = width;
}

float TileSprite::getHeight() {
    return this->height;
}

void TileSprite::setHeight(float height) {
    this->height = height;
}

ItemSprite* TileSprite::getItem() {
    return this->item;
}

void TileSprite::setItem(ItemSprite* item) {
    if (item == NULL) {
        this->item = NULL;
    }else {
        float scale = ((float)TILE_SIDE_LENGTH / TEXTURE_SIDE_WIDTH) * SCALE_RATE;
        this->item = item;
        this->item->setAnchorPoint(Point(0.5, 0.5));
        this->item->setPosition(Vec2(this->getWidth() / 2, this->getHeight() / 2));
        this->item->setScale(scale, scale);
    }
}