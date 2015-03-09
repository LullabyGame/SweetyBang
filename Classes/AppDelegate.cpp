#include "AppDelegate.h"

USING_NS_CC;

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs() {
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    /* 初始化Director */
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }
    Size frameSize = glview->getFrameSize();
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);
    
    /* 设定自适应缩放比率 */
    float contentScaleFactor = 0;
    if (frameSize.height == iphone4.size.height && frameSize.width == iphone4.size.width) {
        contentScaleFactor = MIN(designResolutionSize.height / iphone4.size.height, designResolutionSize.width / iphone4.size.width);
    }else if (frameSize.height == iphone5.size.height && frameSize.width == iphone5.size.width) {
        contentScaleFactor = MIN(designResolutionSize.height / iphone5.size.height, designResolutionSize.width / iphone5.size.width);
    }else if (frameSize.height == iphone6.size.height && frameSize.width == iphone6.size.width) {
        contentScaleFactor = MIN(designResolutionSize.height / iphone6.size.height, designResolutionSize.width / iphone6.size.width);
    }else if (frameSize.height == iphone6_plus.size.height && frameSize.width == iphone6_plus.size.width) {
        contentScaleFactor = MIN(designResolutionSize.height / iphone6_plus.size.height, designResolutionSize.width / iphone6_plus.size.width);
    }else {
        contentScaleFactor = MIN(iphone5.size.height / designResolutionSize.height, iphone5.size.width / designResolutionSize.width);
    }
    director->setContentScaleFactor(contentScaleFactor);
    
    /* 初始化全局变量 */
    tileSideLength = director->getWinSize().width / 8;// Tile边长 = 设计分辨率宽 / 8
    bottomPadding = director->getWinSize().height / 10;// 下边距 = 设计分辨率高 / 10
    leftPadding = (director->getWinSize().width - (tileSideLength * MATRIX_WIDTH) - (TILE_GAP * (MATRIX_WIDTH - 1))) / 2;// 左边距 = (设计分辨率宽 - (Tile边长 * 横轴Tile数) - (Tile间隙长 * (横轴Tile数 - 1))) / 2
    itemScale = ((float)TEXTURE_SIDE_WIDTH / frameSize.width) * SCALE_RATE;// item与tile的转换率

    /* 设定Director其他属性 */
    director->setDisplayStats(true);// turn on display FPS
    director->setAnimationInterval(1.0 / 60);// set FPS. the default value is 1.0/60 if you don't call this
    FileUtils::getInstance()->addSearchPath("res");

    /* 加载场景 */
    auto scene = LoadingScene::createScene();// create a scene. it's an autorelease object
    director->runWithScene(scene);// run

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
