#ifndef __APPMACROS_H__
#define __APPMACROS_H__

#include "cocos2d.h"

#define DESIGN_RESOLUTION_640X960    0
#define DESIGN_RESOLUTION_640X1136   1
#define DESIGN_RESOLUTION_750X1334   2
#define DESIGN_RESOLUTION_1080X1920  3

/* If you want to switch design resolution, change next line */
#define TARGET_DESIGN_RESOLUTION_SIZE  DESIGN_RESOLUTION_640X1136

typedef struct tagResource {
    cocos2d::Size size;
    char directory[100];
}Resource;

//static Resource smallResource  =  { cocos2d::Size(480, 320),   "iphone" };
//static Resource mediumResource =  { cocos2d::Size(1024, 768),  "ipad"   };
//static Resource largeResource  =  { cocos2d::Size(2048, 1536), "ipadhd" };

static Resource iphone4 = { cocos2d::Size(640, 960) };
static Resource iphone5 = { cocos2d::Size(640, 1136) };
static Resource iphone6 = { cocos2d::Size(750, 1334) };
static Resource iphone6_plus = { cocos2d::Size(1242, 2208) };

#if (TARGET_DESIGN_RESOLUTION_SIZE == DESIGN_RESOLUTION_640X960)
static cocos2d::Size designResolutionSize = cocos2d::Size(640, 960);
#elif (TARGET_DESIGN_RESOLUTION_SIZE == DESIGN_RESOLUTION_640X1136)
static cocos2d::Size designResolutionSize = cocos2d::Size(640, 1136);
#elif (TARGET_DESIGN_RESOLUTION_SIZE == DESIGN_RESOLUTION_750X1334)
static cocos2d::Size designResolutionSize = cocos2d::Size(750, 1334);
//#elif (TARGET_DESIGN_RESOLUTION_SIZE == DESIGN_RESOLUTION_1080X1920)
//static cocos2d::Size designResolutionSize = cocos2d::Size(1080, 1920);
#elif (TARGET_DESIGN_RESOLUTION_SIZE == DESIGN_RESOLUTION_1242X2208)
static cocos2d::Size designResolutionSize = cocos2d::Size(1242, 2208);
#else
#error unknown target design resolution!
#endif

// The font size 24 is designed for small resolution, so we should change it to fit for current design resolution
//#define TITLE_FONT_SIZE  (cocos2d::Director::getInstance()->getOpenGLView()->getDesignResolutionSize().width / smallResource.size.width * 24)

#endif /* __APPMACROS_H__ */
