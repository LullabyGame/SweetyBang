//
//  LoadingScene.h
//  SweetBang
//
//  Loading Scene定义
//
//
#pragma once
#ifndef __LittleKnight__LoadingScene__
#define __LittleKnight__LoadingScene__

#include "cocos2d.h"
#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

class LoadingScene : public Layer {
    
public:
    CREATE_FUNC(LoadingScene);
    virtual bool init();
    static Scene* createScene();
    
private:
    void loadingCallBack(Texture2D *texture);
    
};

#endif /* defined(__LittleKnight__LoadingScene__) */
