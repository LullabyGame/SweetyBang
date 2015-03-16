//
// 控制关卡图片显示位置
//
//

#include "LevelSelectContent.h"

LevelSelectContent::LevelSelectContent() {
    
}
LevelSelectContent::~LevelSelectContent() {
    
}

bool LevelSelectContent::init() {
    if ( !Layer::init() ){
        return false;
    }
    return true;
}

/**
 * 绘制关卡节点,写死每页10关
 */
void LevelSelectContent::initAllLevels(ssize_t idx) {
//    this->removeAllChildren();
    int idxs = (int) idx; 
    /*假设每页10关*/
    int firstNumber = idxs * 10 + 1;
    
    /* json读取 */
    std::string stageName = "res/stage/LevelPosition.json";
    
    if(!FileUtils::getInstance()->isFileExist(stageName)) {
        CCLOG("json file is not find");
        return;
    }
    
    std::string data = FileUtils::getInstance()->getStringFromFile(stageName);
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
    rapidjson::Value& tileInfo = doc["levels1"];
    if (tileInfo.IsArray()) {
        for (int row = 0; row < tileInfo.Size(); row++) {
            rapidjson::Value& cols = tileInfo[row];
            rapidjson::Value &positionX = cols["PositionX"];
            rapidjson::Value &positionY = cols["PositionY"];
            int posX = positionX.GetInt();
            int posY = positionY.GetInt();
            
            levelMenu = Menu::create();
            this->addChild(levelMenu);
            
            auto oneLevelItem = LevelSelectItem::create(firstNumber + row);
            levelMenu->addChild(oneLevelItem);
            oneLevelItem->setPosition(posX, posY);
            oneLevelItem->setScale(itemScale, itemScale);
            
            levelMenu->setPosition(0, 0);
            levelMenu->setOpacity(0);
            auto fadeAction = FadeIn::create(0.5);
            levelMenu->runAction(fadeAction);
        }
    }
}