//
//  Settings.h
//  SweetBang
//
//  配置文件
//
//
#pragma once
#ifndef LittleKnight_Settings_h
#define LittleKnight_Settings_h

/* 全局常量定义 */
const int TEXTURE_SIDE_WIDTH = 72;
const int TEXTURE_SIDE_HEIGHT = 72;
const float SCALE_RATE = 9;

const int TOTAL_ITEM_TYPE = 6;// Item的种类(数量)
const int MATRIX_WIDTH = 7;// 矩阵横轴的Tile数量
const int MATRIX_HEIGHT = 9;// 矩阵纵轴的Tile数量

const int TILE_GAP = 2;// Tile之间的间隙宽度
const int TILE_CHINK = 10; // Tile的缓冲区大小

const std::string LOGO_IMAGE = "img/lullaby.jpg";
const std::string BACKGROUND_IMAGE = "img/background.png";
const std::string MAIN_TEXTURE_IMAGE = "img/sweety.png";
const std::string MAIN_TEXTURE_CONFIG = "img/sweety.plist";

const std::string STAGE_FILE_PATH = "stage/";
const std::string STAGE_FILE_PREFIX = "Stage_";

const char PlayerPassLevelCountKey[] = "PlayerPassLevelCountKey";


/* 全局变量声明 */
extern int levelProgress;// 当前用户的关卡进度
extern float leftPadding;// 左边距
extern float bottomPadding;// 底边距
extern int tileSideLength;// tile边长
extern float itemScale;// item与tile的转换率
extern int g_passLevelCount;//已通过的关卡

//初始化三种关卡的状态
typedef enum{
    kAlreadyPass,
    kNotPassYet,
    kLockLevel
}LevelItemType;


#endif
