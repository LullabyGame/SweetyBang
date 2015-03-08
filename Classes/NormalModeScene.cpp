//
//  NormalModeScene.cpp
//  LittleKnight
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
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/got.mp3");
    
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
    Size visiableSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    // 添加背景层
//    backgroundLayer = LayerColor::create(Color4B(238, 222, 170, 255));
//    backgroundLayer->setPosition(Point(origin.x, origin.y));
//    backgroundLayer->setContentSize(Size(visiableSize.width, visiableSize.height));
//    this->addChild(backgroundLayer);
    
    background = Sprite::create("img/background.png");
    background->setAnchorPoint(Point::ZERO);
    background->setPosition(Point::ZERO);
//    background->setScale(visiableSize.width / 640);
    this->addChild(background);
    
    // 添加"back to main menu"按钮
    auto mainMenuLabel = MenuItemLabel::create(LabelTTF::create("Back to Main Menu", "Futura.ttf", 32), CC_CALLBACK_1(NormalModeScene::mainMenuCallback, this));
    mainMenuLabel->setAnchorPoint(Point(0, 0));
    mainMenuLabel->setPosition(Point(visiableSize.width / 2 - 150, visiableSize.height - 90));
    auto mainMenu = Menu::create(mainMenuLabel, NULL);
    mainMenu->setPosition(0, 0);
    background->addChild(mainMenu);
    
    // 添加当前需要击杀的敌人数量显示
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
    
    // 初始化Tile和Item
    initTilesAndItems();
    
    // 添加事件监听
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
                        float posX = LEFT_PADDING + (TILE_SIDE_LENGTH + TILE_GAP) * col;
                        float posY = BOTTOM_PADDING + (TILE_SIDE_LENGTH + TILE_GAP) * (tiles.Size() - 1 - row);
                        
                        // init items
                        BasicItemType randomType = BasicItemType(arc4random() % TOTAL_ITEM_TYPE);
                        ItemSprite *item = ItemSprite::createBasicItem(randomType);
                        
                        // add item to tile
                        TileSprite *tile = TileSprite::createTile(posX ,posY, TILE_SIDE_LENGTH, TILE_SIDE_LENGTH);
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
        
        int itemType = onTouchTile->getItem()->getItemType();// 获取当前选中Tile的类型
        
        /* 添加遮罩,只显示当前选中的同类型元素
        for (int i = 0; i < MATRIX_WIDTH; i++) {
            for (int j = 0; j < MATRIX_HEIGHT; j++) {
                TileSprite* tile = tileMatrix[i][j];
                if (tile != NULL) {
//                    if (itemType == BasicItemType::sword1){
//                        if (tile->getItem()->getItemType() != BasicItemType::enemy2 &&
//                            tile->getItem()->getItemType() != BasicItemType::sword1) {
//                            layerColor = LayerColor::create(Color4B(54, 54, 54, 100), TILE_SIDE_LENGTH, TILE_SIDE_LENGTH);
//                            layerColor->setPosition(Vec2(tile->getPosX(), tile->getPosY()));
//                            addChild(layerColor);
//                            layerColors.pushBack(layerColor);
//                        }
//                    }else if (itemType == BasicItemType::enemy2){
//                        if (tile->getItem()->getItemType() != BasicItemType::enemy2 &&
//                            tile->getItem()->getItemType() != BasicItemType::sword1) {
//                            layerColor = LayerColor::create(Color4B(54, 54, 54, 100), TILE_SIDE_LENGTH, TILE_SIDE_LENGTH);
//                            layerColor->setPosition(Vec2(tile->getPosX(), tile->getPosY()));
//                            addChild(layerColor);
//                            layerColors.pushBack(layerColor);
//                        }
//                    }else
                    if (tile->getItem()->getItemType() != itemType) {
                        layerColor = LayerColor::create(Color4B(54, 54, 54, 100), TILE_SIDE_LENGTH, TILE_SIDE_LENGTH);
                        layerColor->setPosition(Vec2(tile->getPosX(), tile->getPosY()));
                        addChild(layerColor);
                        layerColors.pushBack(layerColor);
                    }
                }
            }
        }*/
        lastPaintedTile = onTouchTile;
    }
    return true;
}

void NormalModeScene::onTouchMoved(Touch *touch, Event *event) {
    Point touchPoint = touch->getLocation();
    TileSprite* onTouchTile = getOnTouchTile(touchPoint.x, touchPoint.y);
    
    if (onTouchTile != NULL) {
        int touchCenterX = onTouchTile->getPosX() + TILE_SIDE_LENGTH / 2;
        int touchCenterY = onTouchTile->getPosY() + TILE_SIDE_LENGTH / 2;
        
        // touch tile and last tile is the same tile
        if (onTouchTile == lastPaintedTile) {
            return;
        }
        
        // touch point is in the chink area
        if (abs(touchPoint.x - touchCenterX) > (TILE_SIDE_LENGTH / 2 - TILE_CHINK) ||
            abs(touchPoint.y - touchCenterY) > (TILE_SIDE_LENGTH / 2 - TILE_CHINK)) {
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
            //当删除连线的时候，需要调用标记怪物死亡
//            markDeath(linePassedTiles);
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
        //调用标记怪物死亡
//        markDeath(linePassedTiles);
    }
}

void NormalModeScene::onTouchEnded(Touch *touch, Event *event) {
    // remove lines
    for(auto line : lines) {
        removeChild(line);
    }
    lines.clear();
    //删除所有遮盖元素的层
    for (LayerColor* layerC : layerColors) {
        removeChild(layerC);
    }
    layerColors.clear();
    
    //删除死亡怪物标记
    for (Sprite* dieds : diedSprites) {
        removeChild(dieds);
    }
    diedSprites.clear();

    // remove tile items
    if (this->linePassedTiles.size() >= 3) {
        for (TileSprite* tile : linePassedTiles) {
            auto particle = ParticleSystemQuad::create("res/animation/kuosan-3.plist");
            particle->setAutoRemoveOnFinish(true);
            particle->setStartColor(Color4F(238, 222, 170, 100));
            particle->setEndColor(Color4F(238, 222, 170, 100));
            particle->setPosition(Vec2(tile->getPosX()+TILE_SIDE_LENGTH/2,tile->getPosY()+TILE_SIDE_LENGTH/2));
            
//            if (tile->getItem()->getItemType() == BasicItemType::enemy2) {
//                remainsMonster--;
//                if (remainsMonster <= 0) {
//                    int nextLevel = this->level + 1;
//                    Scene* normalModeScene = NormalModeScene::createScene(nextLevel);
//                    TransitionFlipX *transition = TransitionFlipX::create(1.2, normalModeScene);
//                    Director::getInstance()->replaceScene(transition);
//                }
//                remainsMonsterLabel->setString(CCString::createWithFormat("%d",remainsMonster)->_string);
//            }
            
            tile->removeChild(tile->getItem());
            tile->setItem(NULL);
            addChild(particle);
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
    int leftRange = LEFT_PADDING;
    int rightRange = LEFT_PADDING + (MATRIX_WIDTH * TILE_SIDE_LENGTH) + ((MATRIX_WIDTH - 1) * TILE_GAP);
    int topRange = BOTTOM_PADDING + (MATRIX_HEIGHT * TILE_SIDE_LENGTH) + ((MATRIX_HEIGHT - 1) * TILE_GAP);
    int bottomRange = BOTTOM_PADDING;
    
    if(onTouchX < leftRange || onTouchX > rightRange || onTouchY > topRange || onTouchY < bottomRange) {
        return NULL;
    }
    
    int arrayX = (onTouchX - leftRange) / (TILE_SIDE_LENGTH + TILE_GAP);
    int arrayY = (onTouchY - bottomRange) / (TILE_SIDE_LENGTH + TILE_GAP);
    
    TileSprite* tile = tileMatrix[arrayX][arrayY];
    if (tile != NULL) {
        if (onTouchX > tile->getPosX() + TILE_SIDE_LENGTH || onTouchY > tile->getPosY() + TILE_SIDE_LENGTH) {
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
    }
//    else if(lastTile->getItem()->getItemType() == BasicItemType::sword1 && currentTile->getItem()->getItemType() == BasicItemType::enemy2) {
//        return true;
//    }else if (lastTile->getItem()->getItemType() == BasicItemType::enemy2 && currentTile->getItem()->getItemType() == BasicItemType::sword1) {
//        return true;
//    }
    else {
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

/*
void NormalModeScene::markDeath(cocos2d::Vector<TileSprite *> tiles){
    
    //记录有多少把剑
    int sword = 0;
    for (auto linepassedtile : tiles) {
        int itemtype = linepassedtile->getItem()->getItemType();
        if (itemtype == BasicItemType::sword1) {
            sword ++;
        }
    }
    
    //如果剑的数量足够杀死怪则标识，否则不标识，假设两把以上的剑可以杀死怪
    //此处以后可以根据怪物血量判断是否可以杀死怪物
    if (sword > 1) {
        //先删除所有的标记，重新添加，可以防止撤销线段时未连线的怪物仍被标记
        for (auto dieds : diedSprites) {
            removeChild(dieds);
        }
        for (auto linepassedtile : tiles) {
            int itemtype = linepassedtile->getItem()->getItemType();
            if (itemtype == BasicItemType::enemy2) {
                deadSprite = Sprite::create("res/img/died.png");
                float scale = ((float)TILE_SIDE_LENGTH / TEXTURE_SIDE_LENGTH) * SCALE_RATE;
                deadSprite->setScale(scale,scale);
                deadSprite->setPosition(Vec2(linepassedtile->getPosX()+TILE_SIDE_LENGTH/2, linepassedtile->getPosY()+TILE_SIDE_LENGTH/2));
                addChild(deadSprite);
                diedSprites.pushBack(deadSprite);
            }
        }
    }else {
        for (auto dieds : diedSprites) {
            removeChild(dieds);
        }
    }
}
*/
