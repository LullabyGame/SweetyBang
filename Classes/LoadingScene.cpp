//
//  LoadingScene.cpp
//  SweetBang
//
//  Loading Scene实现
//
//

#include "LoadingScene.h"

/**
 *  创建Scene
 *
 *  @return 创建的Scene
 */
Scene* LoadingScene::createScene() {
    auto *scene = Scene::create();
    auto *layer = LoadingScene::create();
    scene->addChild(layer);
    
    return scene;
}


/**
 * init方法实现
 */
bool LoadingScene::init() {
    if(!Layer::init()) {
        return false;
    }
    
    /* 添加logo */
    auto *logo = Sprite::create(LOGO_IMAGE);
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    logo->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    logo->setScale(itemScale, itemScale);
    this->addChild(logo);
    
    /* 预读取音乐和音效 */
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/Ka-Ching.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/moonlight.mp3");
    
    /* 初始化素材图片 */
    Director::getInstance()->getTextureCache()->addImageAsync(MAIN_TEXTURE_IMAGE, CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(MAIN_TEXTURE_CONFIG);

    return true;
}


/**
 *  Loading完成后回调
 *
 *  @param texture
 */
void LoadingScene::loadingCallBack(Texture2D *texture) {
    auto scene = MainMenuScene::createScene();
    TransitionScene *transition = TransitionFade::create(1, scene);
    Director::getInstance()->replaceScene(transition);
}