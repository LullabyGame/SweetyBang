//
//  GameConstants.h
//  PageTest
//
//  Created by wx5 on 12/31/14.
//
//

#ifndef __PageTest__GameConstants__
#define __PageTest__GameConstants__

#ifndef PageTest_GameConstants_h
#define PageTest_GameConstants_h

typedef enum{
    kAlreadyPass,
    kNotPassYet,
    kLockLevel
}LevelItemType;


const char PlayerPassLevelCountKey[] = "PlayerPassLevelCountKey";

extern int g_passLevelCount;
extern int g_currentLevel;


#endif

#endif /* defined(__PageTest__GameConstants__) */
