//
//  TileSprite.h
//  LittleKnight
//
//  Tile元素定义
//
//

#ifndef __LittleKnight__TileSprite__
#define __LittleKnight__TileSprite__

#include "cocos2d.h"
#include "ItemSprite.h"
#include "Settings.h"

USING_NS_CC;

class TileSprite : public LayerColor {
    
public:
    CREATE_FUNC(TileSprite);
    virtual bool init();
    static TileSprite* createTile(float posX, float posY, float width, float height);
    
    int getArrayX();
    void setArrayX(int arrayX);
    int getArrayY();
    void setArrayY(int arrayY);
    float getPosX();
    void setPosX(float posX);
    float getPosY();
    void setPosY(float posY);
    float getWidth();
    void setWidth(float width);
    float getHeight();
    void setHeight(float height);
    Vec2 getTileSize();
    void setTileSize(Vec2 size);
    ItemSprite* getItem();
    void setItem(ItemSprite *item);
    
private:
    int arrayX;
    int arrayY;
    float posX;
    float posY;
    float width;
    float height;
    Vec2 tileSize;
    ItemSprite* item;
    
};

#endif /* defined(__LittleKnight__TileSprite__) */
