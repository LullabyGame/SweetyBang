//
//  MainMenuScene.cpp
//  SweetBang
//
//  主菜单Scene实现
//
//

#include "MainMenuScene.h"

/**
 *  创建Scene
 *
 *  @return 创建的Scene
 */
Scene* MainMenuScene::createScene() {
    auto *scene = Scene::create();
    auto *layer = MainMenuScene::create();
    scene->addChild(layer);
    
    return scene;
}


/**
 * init方法
 * 定义各种菜单按钮
 */
bool MainMenuScene::init() {
    if(!Layer::init()) {
        return false;
    }
    
    Size visiableSize = Director::getInstance()->getVisibleSize();
    
    TableView * tv = TableView::create(this,Size(visiableSize.width,visiableSize.height));
    this->addChild(tv);
    return true;
}

/**
 * 展示所有关卡
 */
void MainMenuScene::initAllLevels(TableViewCell * cell, ssize_t idx) {
    UserDefault *save = UserDefault::getInstance();
    g_passLevelCount = save->getIntegerForKey(PlayerPassLevelCountKey, 0);
    log("g_passLevelCount:%d", g_passLevelCount);
    
    //下面代码需要更改
    g_passLevelCount = 24;
    
    LevelSelectContent* levelSelectContent = LevelSelectContent::create();
    cell->addChild(levelSelectContent);
    levelSelectContent->initAllLevels(idx);
}


/**
 * 控制每个菜单列表的尺寸
 *
 */
Size MainMenuScene::cellSizeForTable(TableView *table) {
    return Size(640,1136+200);
}
/**
 * 根据索引idx创建菜单
 *
 * @return 返回当前菜单
 */
TableViewCell* MainMenuScene::tableCellAtIndex(TableView *table, ssize_t idx) {
    char Icon[20];   //根据idx选中显示的图片
    sprintf(Icon, "img/background%zd.png", idx);
    TableViewCell * cell = table->dequeueCell();
    log("%zd",idx);
    Size visiableSize = Director::getInstance()->getVisibleSize();
    if (!cell){
        cell = TableViewCell::create();
        /* 添加背景 */
        auto background = Sprite::create(Icon);
        background->setAnchorPoint(Point::ZERO);
        background->setPosition(Vec2(0,0));
        cell->addChild(background);
    }
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("res/res/startUI.plist", "res/res/startUI.png");
    initAllLevels(cell,idx);
    
    return cell;
}

/**
 * 控制显示几个菜单
 *
 */
ssize_t MainMenuScene::numberOfCellsInTableView(TableView *table) {
    return 3;
}
