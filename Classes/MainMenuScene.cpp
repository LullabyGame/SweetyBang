//
//  MainMenuScene.cpp
//  SweetBang
//
//  主菜单Scene实现
//
//

#include "MainMenuScene.h"
#include "GameConstants.h"

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

    /* 添加NormalMode按钮 */
    auto nomrmalModeLable = MenuItemLabel::create(LabelTTF::create("Normal Mode", "Futura.ttf", 32), CC_CALLBACK_1(MainMenuScene::menuNormalModeCallback, this));
    nomrmalModeLable->setTag(1);// 默认加载第一关
    nomrmalModeLable->setAnchorPoint(Point(0, 0));
    nomrmalModeLable->setPosition(Point(visiableSize.width / 2 - 80, visiableSize.height / 2));
    auto normalModeMenu = Menu::create(nomrmalModeLable, NULL);
    normalModeMenu->setPosition(0, 0);
    this->addChild(normalModeMenu);
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("res/res/startUI.plist", "res/res/startUI.png");
    
//    auto colorLayer = LayerColor::create(Color4B(0x24, 0x2c, 0x3c, 255));
//    this->addChild(colorLayer, 0);
    
    initAllLevels();
    
    return true;
}


/**
 *  NormalMode按钮回调
 *
 *  @param sender
 */
void MainMenuScene::menuNormalModeCallback(Ref *sender) {
    MenuItem* selectedMenu = (MenuItem*) sender;
    int tag = selectedMenu->getTag();// 使用tag记录关卡
    Scene* normalModeScene = NormalModeScene::createScene(tag);
    TransitionFlipX *transition = TransitionFlipX::create(1.2, normalModeScene);
    Director::getInstance()->replaceScene(transition);
}


/**
 <#Description#>
 */
void MainMenuScene::initAllLevels(){
    UserDefault *save = UserDefault::getInstance();
    g_passLevelCount = save->getIntegerForKey(PlayerPassLevelCountKey, 0);
    log("g_passLevelCount:%d", g_passLevelCount);
    
    //just for test, in real game, comment the below code
    g_passLevelCount = 1;
    
    levelSelectContent = LevelSelectContent::create();
    this->addChild(levelSelectContent);
    levelSelectContent->initAllLevels(_currentPage);
}
