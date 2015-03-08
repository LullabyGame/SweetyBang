//
//  Settings.h
//  LittleKnight
//
//  配置文件
//
//

#ifndef LittleKnight_Settings_h
#define LittleKnight_Settings_h

const int TEXTURE_SIDE_WIDTH = 72;
const int TEXTURE_SIDE_HEIGHT = 72;
const float SCALE_RATE = 0.8;
const int TOTAL_ITEM_TYPE = 6;// Item的种类(数量)
const int MATRIX_WIDTH = 7;// 矩阵横轴的Tile数量
const int MATRIX_HEIGHT = 9;// 矩阵纵轴的Tile数量
const int TILE_GAP = 2;// Tile之间的间隙宽度
const int LEFT_PADDING = 15;// 左边距
const int BOTTOM_PADDING = 150;// 下边距
const int TILE_SIDE_LENGTH = 80;// Tile边长
const int TILE_CHINK = 10; // Tile的缓冲区大小

extern int levelProgress;// 当前用户的关卡进度

#endif
