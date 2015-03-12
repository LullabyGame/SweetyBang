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
    
    /* 添加背景 */
    auto background = Sprite::create("img/background.png");
    background->setAnchorPoint(Point::ZERO);
    background->setPosition(Point::ZERO);
    this->addChild(background);
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("res/res/startUI.plist", "res/res/startUI.png");
    initAllLevels();
    
    return true;
}

/**
 * 展示所有关卡
 */
void MainMenuScene::initAllLevels() {
    UserDefault *save = UserDefault::getInstance();
    g_passLevelCount = save->getIntegerForKey(PlayerPassLevelCountKey, 0);
    log("g_passLevelCount:%d", g_passLevelCount);
    
    //下面代码需要更改
    g_passLevelCount = 1;
    
    LevelSelectContent* levelSelectContent = LevelSelectContent::create();
    this->addChild(levelSelectContent);
    levelSelectContent->initAllLevels();
}
