//
//  LineSprite.h
//  LittleKnight
//
//  Line对象的定义
//
//

#ifndef __LittleKnight__LineSprite__
#define __LittleKnight__LineSprite__

#include "cocos2d.h"
#include "TileSprite.h"

USING_NS_CC;

class LineSprite : public Sprite {
    
public:
    CREATE_FUNC(LineSprite);
    virtual bool init();
    static LineSprite* createLine(TileSprite* beginTile, TileSprite* endTile);
    
    TileSprite* getBeginTile();
    void setBeginTile(TileSprite* beginTile);
    TileSprite* getEndTile();
    void setEndTile(TileSprite* endTile);
    
private:
    TileSprite* beginTile;// 线段起始的Tile
    TileSprite* endTile;// 线段结束的Tile
    void draw(TileSprite* beginTile, TileSprite* endTile);
    
};

#endif /* defined(__LittleKnight__LineSprite__) */
