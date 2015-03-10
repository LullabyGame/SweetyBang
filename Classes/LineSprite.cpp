//
//  LineSprite.cpp
//  SweetBang
//
//  Line对象的实现
//
//

#include "LineSprite.h"

/**
 * init方法
 */
bool LineSprite::init() {
    if(!Sprite::init()) {
        return false;
    }
    return true;
}


/**
 *  创建连接两个Tile间的Line
 *
 *  @param beginTile 起始Tile
 *  @param endTile   结束Tile
 *
 *  @return 创建的Line
 */
LineSprite* LineSprite::createLine(TileSprite* beginTile, TileSprite* endTile) {
    LineSprite* line = new LineSprite();
    if(line && line->init()) {
        line->autorelease();
        line->setBeginTile(beginTile);
        line->setEndTile(endTile);
        line->draw(beginTile, endTile);
        return line;
    }
    CC_SAFE_DELETE(line);
    return NULL;
}


/**
 *  画线方法
 *
 *  @param beginTile 起始Tile
 *  @param endTile   结束Tile
 */
void LineSprite::draw(TileSprite* beginTile, TileSprite* endTile) {
    DrawNode* drawNode = DrawNode::create();
    drawNode->drawSegment(Vec2(beginTile->getPosX() + (beginTile->getWidth() / 2), beginTile->getPosY() + (beginTile->getHeight() / 2)),
                          Vec2(endTile->getPosX() + (endTile->getWidth() / 2), endTile->getPosY() + (endTile->getHeight() / 2)),
                          6, Color4F(111, 0, 0, 50));
    this->addChild(drawNode, 20);
}

/*Get & Set方法*/

TileSprite* LineSprite::getBeginTile() {
    return this->beginTile;
}

void LineSprite::setBeginTile(TileSprite* beginTile) {
    this->beginTile = beginTile;
}

TileSprite* LineSprite::getEndTile() {
    return this->endTile;
}

void LineSprite::setEndTile(TileSprite* endTile) {
    this->endTile = endTile;
}