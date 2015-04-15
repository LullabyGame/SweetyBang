//
//  NormalModeScene.h
//  SweetBang
//
//  普通模式Scene定义
//
//
#pragma once
#ifndef __LittleKnight__NormalModeScene__
#define __LittleKnight__NormalModeScene__

#include "cocos2d.h"
#include "AppMacros.h"
#include "TileSprite.h"
#include "ItemSprite.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "MainMenuScene.h"
#include "LineSprite.h"
#include "Settings.h"
#include "SimpleAudioEngine.h"
#include "SpecialSprite.h"

USING_NS_CC;

class NormalModeScene : public Layer {
    
public:
    NormalModeScene();
    CREATE_FUNC(NormalModeScene);
    virtual bool init();
    
    static Scene* createScene(int level);
    static NormalModeScene* createLayer(int level);
    bool initLayer();
    
    int getStage();
    void setStage(int level);
    
    virtual bool onTouchBegan(Touch *touch, Event *event);
    virtual void onTouchEnded(Touch *touch, Event *event);
    virtual void onTouchMoved(Touch *touch, Event *event);

private:
    
    /* VARIABLES */
    Sprite *background;
    
    int stage = 0;// 关卡号
    int remainMoves;
    int targetScore;
    int currentMoves = 1;
    int currentScore = 0;
    long scoreNow = 0;
    
    TileSprite* tileMatrix[MATRIX_WIDTH][MATRIX_HEIGHT] = {0};
    TileSprite* lastPaintedTile;
    TileSprite* moveOnTouchTile;//用于删除重复线段时记录当前点击到的单元格
    LayerColor* speciallayer;// 遮挡会被特殊元素消除的元素
    Sprite* spriteAction;// 遮挡不能连线的元素
    cocos2d::Vector<TileSprite*> linePassedTiles;
    cocos2d::Vector<LineSprite*> lines;
    cocos2d::Vector<LayerColor*> layerColors;
    cocos2d::Vector<TileSprite*> itemSpecialVector;//存储特殊元素消除的item
    
    Label* targetScoreLabel;
    Label* currentScoreLabel;
    Label* targetMoveLabel;
    Label* remainMovesLabel;
    Label* scoreNowLabel;
    
    /* FUNCTIONS */
    void loadStageInfo();
    void initTilesAndItems(rapidjson::Value& tileInfo);
    void initMenuBar();
    
    TileSprite* getOnTouchTile(float onTouchX, float onTouchY);// 查询当前触摸点在Tile矩阵中的位置
    void darwLine(TileSprite* beginTile, TileSprite* endTile);
    bool isSameItemType(TileSprite* lastTile, TileSprite* currentTile);
    void deleteRepetitionLine(TileSprite* onTouchTile);// 当选择到已经经过的格子，删除该格子后的线段
    
    void fallDownItems();
    void fillTiles();
    
    void mainMenuCallback(Ref* sender);
    void back2StageSelectScene();
    void addTouchListeners();
    
    void removeAction(Node * node);
    
    void itemSpecialAction(TileSprite* onTouchTile);//对特殊元素的处理：1、增加动画效果。2、把需要删除的item放入列表中
};

#endif /* defined(__LittleKnight__NormalModeScene__) */
