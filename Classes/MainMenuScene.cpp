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
    //下面代码需要更改，内存库存储
    UserDefault *save = UserDefault::getInstance();
    g_passLevelCount = save->getIntegerForKey(PlayerPassLevelCountKey, 8);
    log("g_passLevelCount:%d", g_passLevelCount);
    
    TableView * tv = TableView::create(this,Size(visiableSize.width,visiableSize.height));
    /* 控制列表只显示到用户完成的关卡位置 */
    tv->setContentOffset(Vec2(0, -(visiableSize.height * (g_passLevelCount/10))));
    this->addChild(tv);
    return true;
}

/**
 * 展示所有关卡
 */
void MainMenuScene::initAllLevels(Sprite * background, ssize_t idx) {
    LevelSelectContent* levelSelectContent = LevelSelectContent::create();
    background->addChild(levelSelectContent);
    levelSelectContent->initAllLevels(idx);
}


/**
 * 控制每个菜单列表的尺寸
 *
 */
Size MainMenuScene::cellSizeForTable(TableView *table) {
    Size visiableSize = Director::getInstance()->getVisibleSize();
    
    log("visiableSize: %f",visiableSize.height);
    return Size(visiableSize.width,visiableSize.height);
}
/**
 * 根据索引idx创建菜单
 *
 * @return 返回当前菜单
 */
TableViewCell* MainMenuScene::tableCellAtIndex(TableView *table, ssize_t idx) {
    char Icon[26];   //根据idx选中显示的图片
    sprintf(Icon, "res/img/background%zd.png", idx);
    TableViewCell * cell = table->dequeueCell();
    log("page: %zd",idx);
    log("img: %s",Icon);
    Size visiableSize = Director::getInstance()->getVisibleSize();
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("res/res/startUI.plist", "res/res/startUI.png");
    Sprite * background;
    if (!cell){
        cell = TableViewCell::create();
        /* 添加背景 */
        background = Sprite::create(Icon);
        background->setAnchorPoint(Point::ZERO);
        background->setPosition(Point::ZERO);
        background->setTag(2);// TODO
        cell->addChild(background);
        initAllLevels(background, idx);
    }else{
        /* 取出消失的cell修改内容 */
        background = (Sprite*)cell->getChildByTag(2);
        background->setTexture(Icon);
        background->removeAllChildren();
        initAllLevels(background,idx);
    }
    return cell;
}

/**
 * 控制显示几个菜单
 *
 */
ssize_t MainMenuScene::numberOfCellsInTableView(TableView *table) {
    return 5;
}
