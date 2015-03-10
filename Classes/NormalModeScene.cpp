//
//  NormalModeScene.cpp
//  SweetBang
//
//  普通模式Scene实现
//
//

#include "NormalModeScene.h"

NormalModeScene::NormalModeScene(){}

/**
 *  创建Scene
 *
 *  @param level 当前关卡
 *  @return 创建的Scene
 */
Scene* NormalModeScene::createScene(int level) {
    auto *scene = Scene::create();
    auto *layer = NormalModeScene::createLayer(level);
    scene->addChild(layer);
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/yang.mp3");
    
    return scene;
}


/**
 * init方法
 */
bool NormalModeScene::init() {
    if (!Layer::init()) {
        return false;
    }
    return true;
}


/**
 *  创建Layer
 *
 *  @param level 当前关卡
 *  @return
 */
NormalModeScene* NormalModeScene::createLayer(int level) {
    NormalModeScene *normalMode = new NormalModeScene();
    if (normalMode && normalMode->init()) {
        normalMode->autorelease();
        normalMode->setLevel(level);
        normalMode->initLayer();
        return normalMode;
    }
    CC_SAFE_DELETE(normalMode);
    return NULL;
}


/**
 * 初始化Layer
 */
bool NormalModeScene::initLayer() {
    
    /* 设定基础数值 */
    Size visiableSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    /* 添加背景层 */
    background = Sprite::create(BACKGROUND_IMAGE);
    background->setAnchorPoint(Point::ZERO);
    background->setPosition(Point::ZERO);
    background->setContentSize(Size(designResolutionSize.width, designResolutionSize.height));// 根据设计分辨率设定图片大小
    this->addChild(background);
    
    /* 添加"back to main menu"按钮 */
    auto mainMenuLabel = MenuItemLabel::create(LabelTTF::create("Back to Main Menu", "Futura.ttf", 32), CC_CALLBACK_1(NormalModeScene::mainMenuCallback, this));
    mainMenuLabel->setAnchorPoint(Point(0, 0));
    mainMenuLabel->setPosition(Point(visiableSize.width / 2 - 150, visiableSize.height - 90));
    auto mainMenu = Menu::create(mainMenuLabel, NULL);
    mainMenu->setPosition(0, 0);
    background->addChild(mainMenu);
    
    /* 添加当前分数/目标分数显示 TODO */
    std::stringstream levelSS;
    levelSS << this->getLevel();
    std::string levelName = "level/Level_" + levelSS.str() + ".json";
    if(!FileUtils::getInstance()->isFileExist(levelName)) {
        CCLOG("json file is not find");
        return false;
    }
    std::string data = FileUtils::getInstance()->getStringFromFile(levelName);
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
    rapidjson::Value& targetMonster = doc["targetMonster"];
    remainsMonster = targetMonster.GetInt();
    remainsMonsterLabel = Label::create(CCString::createWithFormat("%d",remainsMonster)->_string, "American Typewriter.ttf", 40);
    remainsMonsterLabel->setAnchorPoint(Point(0.1, 0.1));
    remainsMonsterLabel->setPosition((Point(visiableSize.width / 2, visiableSize.height - 150)));
    background->addChild(remainsMonsterLabel);
    
    /* 初始化Tile和Item */
    initTilesAndItems();
    
    /* 添加事件监听 */
    addTouchListeners();
    
    return true;
}


/**
 * 初始化Tile和Item
 */
void NormalModeScene::initTilesAndItems() {
    std::stringstream levelSS;
    levelSS << this->getLevel();
    std::string levelName = "level/Level_" + levelSS.str() + ".json";
    if(!FileUtils::getInstance()->isFileExist(levelName)) {
        CCLOG("json file is not find");
        return;
    }
    
    std::string data = FileUtils::getInstance()->getStringFromFile(levelName);
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
    rapidjson::Value& tiles = doc["tiles"];
    if (tiles.IsArray()) {
        for (int row = 0; row < tiles.Size(); row++) {
            rapidjson::Value& cols = tiles[row];
            if (cols.IsArray()) {
                for (int col = 0; col < cols.Size(); col++) {
                    int tileType = cols[col].GetInt();
                    if (tileType == 0) {
                        continue;
                    }else {
                        float posX = leftPadding + (tileSideLength + TILE_GAP) * col;
                        float posY = bottomPadding + (tileSideLength + TILE_GAP) * (tiles.Size() - 1 - row);
                        
                        // init items
                        BasicItemType randomType = BasicItemType(arc4random() % TOTAL_ITEM_TYPE);
                        ItemSprite *item = ItemSprite::createBasicItem(randomType);
                        
                        // add item to tile
                        TileSprite *tile = TileSprite::createTile(posX ,posY, tileSideLength, tileSideLength);
                        tile->setItem(item);
                        tile->addChild(item);
                        
                        // add tile into matrix
                        tileMatrix[col][tiles.Size() - 1 - row] = tile;
                        tile->setArrayX(col);
                        tile->setArrayY(tiles.Size() - 1 - row);
                        
                        // add tile to background
                        this->background->addChild(tile);
                    }
                }
            }
        }
    }
}


/**
 *  初始化事件监听
 */
void NormalModeScene::addTouchListeners() {
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(NormalModeScene::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(NormalModeScene::onTouchEnded, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(NormalModeScene::onTouchMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}


/**
 *  onTouchBegan监听
 *
 *  @param touch
 *  @param event
 *  @return
 */
bool NormalModeScene::onTouchBegan(Touch *touch, Event *event) {
    Point touchPoint = touch->getLocation();
    TileSprite* onTouchTile = getOnTouchTile(touchPoint.x, touchPoint.y);
    
    if (onTouchTile != NULL) {
        // CCLOG("当前选中的为Tile[%d][%d]", currentTile->getArrayX(), currentTile->getArrayY());
        lastPaintedTile = onTouchTile;
    }
    return true;
}

void NormalModeScene::onTouchMoved(Touch *touch, Event *event) {
    Point touchPoint = touch->getLocation();
    TileSprite* onTouchTile = getOnTouchTile(touchPoint.x, touchPoint.y);
    
    if (onTouchTile != NULL) {
        int touchCenterX = onTouchTile->getPosX() + tileSideLength / 2;
        int touchCenterY = onTouchTile->getPosY() + tileSideLength / 2;
        
        // touch tile and last tile is the same tile
        if (onTouchTile == lastPaintedTile) {
            return;
        }
        
        // touch point is in the chink area
        if (abs(touchPoint.x - touchCenterX) > (tileSideLength / 2 - TILE_CHINK) ||
            abs(touchPoint.y - touchCenterY) > (tileSideLength / 2 - TILE_CHINK)) {
            return;
        }
        
        // not near the last painted tile
        if (abs(lastPaintedTile->getArrayX() - onTouchTile->getArrayX()) > 1 ||
            abs(lastPaintedTile->getArrayY() - onTouchTile->getArrayY()) > 1) {
            return;
        }
        
        // repetition tile
        if (linePassedTiles.contains(onTouchTile)) {
            deleteDepetitionLine(onTouchTile);
            lastPaintedTile = onTouchTile;
            return;
        }
        
        // not the same type with the last tile
        if (!isSameItemType(lastPaintedTile, onTouchTile)) {
            return;
        }
        
        // draw line
        LineSprite* line = LineSprite::createLine(lastPaintedTile, onTouchTile);
        this->addChild(line);
        lines.pushBack(line);
        
        // add the on touch tile into the "linePassedTiles" list
        if (!linePassedTiles.contains(lastPaintedTile)) {
            linePassedTiles.pushBack(lastPaintedTile);
        }
        lastPaintedTile = onTouchTile;
        linePassedTiles.pushBack(onTouchTile);
    }
}

void NormalModeScene::onTouchEnded(Touch *touch, Event *event) {
    // remove lines
    for(auto line : lines) {
        removeChild(line);
    }
    lines.clear();

    // remove tile items
    if (this->linePassedTiles.size() >= 3) {
        for (TileSprite* tile : linePassedTiles) {
            
            /* 加载粒子效果 */
            auto particle = ParticleSystemQuad::create("res/animation/kuosan-3.plist");
            particle->setAutoRemoveOnFinish(true);
            particle->setStartColor(Color4F(238, 222, 170, 100));
            particle->setEndColor(Color4F(238, 222, 170, 100));
            particle->setPosition(Vec2(tile->getPosX() + tileSideLength / 2,tile->getPosY() + tileSideLength / 2));
            addChild(particle);
            
            /* 移除item */
            tile->removeChild(tile->getItem());
            tile->setItem(NULL);
            
        }
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Ka-Ching.wav");
    }
    
    //
    fallDownItems();
    
    // create new items
    fillTiles();
    
    linePassedTiles.clear();
}

void NormalModeScene::fallDownItems() {
    for (int i = 0; i < MATRIX_WIDTH; i++) {
        int removedItem4Col = 0;
        for (int j = 0; j < MATRIX_HEIGHT; j++) {
            TileSprite* tile = tileMatrix[i][j];
            if (tile == NULL) {
                continue;
            }
            if (tile->getItem() == NULL) {
                removedItem4Col++;
            }else {
                if (removedItem4Col > 0) {
                    int newRow = j - removedItem4Col;
                    ItemSprite* fallItem = tile->getItem();
                    tileMatrix[i][newRow]->setItem(fallItem);
                    tileMatrix[i][newRow]->addChild(fallItem);
                    tile->removeChild(fallItem);
                    tile->setItem(NULL);
                }
            }
        }
    }
}

void NormalModeScene::fillTiles() {
    for (int i = 0; i < MATRIX_WIDTH; i++) {
        for (int j = 0; j < MATRIX_HEIGHT; j++) {
            TileSprite* tile = tileMatrix[i][j];
            if (tile != NULL && tile->getItem() == NULL) {
                BasicItemType randomType = BasicItemType(arc4random() % TOTAL_ITEM_TYPE);
                ItemSprite *item = ItemSprite::createBasicItem(randomType);
                tile->setItem(item);
                tile->addChild(item);
            }
        }
    }
}


TileSprite* NormalModeScene::getOnTouchTile(float onTouchX, float onTouchY) {
    int leftRange = leftPadding;
    int rightRange = leftPadding + (MATRIX_WIDTH * tileSideLength) + ((MATRIX_WIDTH - 1) * TILE_GAP);
    int topRange = bottomPadding + (MATRIX_HEIGHT * tileSideLength) + ((MATRIX_HEIGHT - 1) * TILE_GAP);
    int bottomRange = bottomPadding;
    
    if(onTouchX < leftRange || onTouchX > rightRange || onTouchY > topRange || onTouchY < bottomRange) {
        return NULL;
    }
    
    int arrayX = (onTouchX - leftRange) / (tileSideLength + TILE_GAP);
    int arrayY = (onTouchY - bottomRange) / (tileSideLength + TILE_GAP);
    
    TileSprite* tile = tileMatrix[arrayX][arrayY];
    if (tile != NULL) {
        if (onTouchX > tile->getPosX() + tileSideLength || onTouchY > tile->getPosY() + tileSideLength) {
            return NULL;
        }
        return tile;
    }else {
        return NULL;
    }
}


bool NormalModeScene::isSameItemType(TileSprite* lastTile, TileSprite* currentTile) {
    if (lastTile->getItem() == NULL || currentTile->getItem() == NULL) {
        return false;
    }
    
    if (lastTile->getItem()->getItemType() == currentTile->getItem()->getItemType()) {
        return true;
    }else {
        return false;
    }
}


void NormalModeScene::mainMenuCallback(Ref *sender) {
    auto scene = MainMenuScene::createScene();
    TransitionFlipX *transition = TransitionFlipX::create(1.2, scene);
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    Director::getInstance()->replaceScene(transition);
}

void NormalModeScene::deleteDepetitionLine(TileSprite* onTouchTile) {
    for (auto sprite : lines) {
        if (sprite->getBeginTile() == onTouchTile) {
            removeChild(sprite);
            lines.erase(lines.find(sprite));
            linePassedTiles.erase(linePassedTiles.find(sprite->getEndTile()));
            deleteDepetitionLine(sprite->getEndTile());
            break;
        }
    }
}

int NormalModeScene::getLevel() {
    return this->level;
}

void NormalModeScene::setLevel(int level) {
    this->level = level;
}
