//
//  MainMenuScene.h
//  SweetBang
//
//  主菜单Scene定义
//
//
#pragma once
#ifndef __LittleKnight__MainMenuScene__
#define __LittleKnight__MainMenuScene__

#include "cocos2d.h"
#include "Settings.h"
#include "LevelSelectContent.h"
#include <cocos-ext.h>

USING_NS_CC_EXT;

USING_NS_CC;

class MainMenuScene : public Layer ,TableViewDataSource {
    
public:
    CREATE_FUNC(MainMenuScene);
    virtual bool init();
    static Scene* createScene();

    virtual cocos2d::Size cellSizeForTable(TableView *table);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
private:
    void initAllLevels(TableViewCell * cell, ssize_t idx);
    
};

#endif /* defined(__LittleKnight__MainMenuScene__) */
