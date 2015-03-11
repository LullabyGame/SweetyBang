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
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/moonlight.mp3");
    
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
NormalModeScene* NormalModeScene::createLayer(int stage) {
    NormalModeScene *normalMode = new NormalModeScene();
    if (normalMode && normalMode->init()) {
        normalMode->autorelease();
        normalMode->setStage(stage);
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
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    /* 添加背景层 */
    background = Sprite::create(BACKGROUND_IMAGE);
    background->setAnchorPoint(Point::ZERO);
    background->setPosition(Point::ZERO);
    background->setContentSize(Size(designResolutionSize.width, designResolutionSize.height));// 根据设计分辨率设定图片大小
    this->addChild(background);
    
    /* 初始化关卡信息 */
    loadStageInfo();// 读取关卡信息
    
    /* 初始化工具栏 */
    initMenuBar();
    
    /* 添加事件监听 */
    addTouchListeners();
    
    return true;
}


/**
 * 读取关卡信息
 */
void NormalModeScene::loadStageInfo() {
    std::stringstream stageStream;
    stageStream << this->getStage();
    std::string stageName = STAGE_FILE_PATH + STAGE_FILE_PREFIX + stageStream.str() + ".json";
    
    if(!FileUtils::getInstance()->isFileExist(stageName)) {
        CCLOG("json file is not find");
        return;
    }
    
    std::string data = FileUtils::getInstance()->getStringFromFile(stageName);
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());

    /* 初始化Tile和Item */
    rapidjson::Value& tileInfo = doc["tiles"];
    initTilesAndItems(tileInfo);
    
    /* 初始化目标分数与限制步数 */
    rapidjson::Value& stageScore = doc["score"];
    rapidjson::Value& stageMove = doc["move"];
    this->targetScore = stageScore.GetInt();
    this->targetMoves = stageMove.GetInt();
}


/**
 * 初始化Tile和Item
 */
void NormalModeScene::initTilesAndItems(rapidjson::Value& tileInfo) {
    if (tileInfo.IsArray()) {
        for (int row = 0; row < tileInfo.Size(); row++) {
            rapidjson::Value& cols = tileInfo[row];
            if (cols.IsArray()) {
                for (int col = 0; col < cols.Size(); col++) {
                    int tileType = cols[col].GetInt();
                    if (tileType == 0) {
                        continue;
                    }else {
                        float posX = leftPadding + (tileSideLength + TILE_GAP) * col;
                        float posY = bottomPadding + (tileSideLength + TILE_GAP) * (tileInfo.Size() - 1 - row);
                        
                        // init items
                        BasicItemType randomType = BasicItemType(arc4random() % TOTAL_ITEM_TYPE);
                        ItemSprite *item = ItemSprite::createBasicItem(randomType);
                        
                        // add item to tile
                        TileSprite *tile = TileSprite::createTile(posX ,posY, tileSideLength, tileSideLength);
                        tile->setItem(item);
                        tile->addChild(item);
                        
                        // add tile into matrix
                        tileMatrix[col][tileInfo.Size() - 1 - row] = tile;
                        tile->setArrayX(col);
                        tile->setArrayY(tileInfo.Size() - 1 - row);
                        
                        // add tile to background
                        this->background->addChild(tile);
                    }
                }
            }
        }
    }
}

/**
 * 初始化菜单栏
 */
void NormalModeScene::initMenuBar() {
    Size visiableSize = Director::getInstance()->getVisibleSize();
    
    /* 添加"Main Menu"按钮 */
    auto mainMenuLabel = MenuItemLabel::create(LabelTTF::create("Main Menu", "Futura.ttf", 32), CC_CALLBACK_1(NormalModeScene::mainMenuCallback, this));
    mainMenuLabel->setColor(Color3B(0, 0, 0));
    mainMenuLabel->setAnchorPoint(Point::ZERO);
    mainMenuLabel->setPosition(Point(visiableSize.width / 2 - 80, visiableSize.height - 90));
    auto mainMenu = Menu::create(mainMenuLabel, NULL);
    mainMenu->setPosition(0, 0);
    background->addChild(mainMenu);
    
    /* 设定步数显示 */
    auto moveLabel = MenuItemLabel::create(LabelTTF::create("Move:", "Futura.ttf", 32));
    moveLabel->setColor(Color3B(0, 0, 0));
    moveLabel->setAnchorPoint(Point::ZERO);
    moveLabel->setPosition(Point(visiableSize.width / 8, visiableSize.height - 150));
    background->addChild(moveLabel);
    
    currentMoveLabel = Label::create(CCString::createWithFormat("%d",currentMoves)->_string, "American Typewriter.ttf", 40);
    currentMoveLabel->setTextColor(Color4B(0, 0, 0, 0));
    currentMoveLabel->setAnchorPoint(Point(Point::ZERO));
    currentMoveLabel->setPosition((Point(visiableSize.width / 8 + 100, visiableSize.height - 150)));
    background->addChild(currentMoveLabel);
    
    auto moveSplitLabel = MenuItemLabel::create(LabelTTF::create("/", "Futura.ttf", 32));
    moveSplitLabel->setColor(Color3B(0, 0, 0));
    moveSplitLabel->setAnchorPoint(Point::ZERO);
    moveSplitLabel->setPosition(Point(visiableSize.width / 8 + 150, visiableSize.height - 150));
    background->addChild(moveSplitLabel);
    
    targetMoveLabel = Label::create(CCString::createWithFormat("%d",targetMoves)->_string, "American Typewriter.ttf", 40);
    targetMoveLabel->setTextColor(Color4B(0, 0, 0, 0));
    targetMoveLabel->setAnchorPoint(Point(Point::ZERO));
    targetMoveLabel->setPosition((Point(visiableSize.width / 8 + 170, visiableSize.height - 150)));
    background->addChild(targetMoveLabel);
    
    /* 设定分数显示 */
    auto scoreLabel = MenuItemLabel::create(LabelTTF::create("Score:", "Futura.ttf", 32));
    scoreLabel->setColor(Color3B(0, 0, 0));
    scoreLabel->setAnchorPoint(Point::ZERO);
    scoreLabel->setPosition(Point(visiableSize.width / 2 + 20, visiableSize.height - 150));
    background->addChild(scoreLabel);
    
    targetScoreLabel = Label::create(CCString::createWithFormat("%d",targetScore)->_string, "American Typewriter.ttf", 40);
    targetScoreLabel->setTextColor(Color4B(0, 0, 0, 0));
    targetScoreLabel->setAnchorPoint(Point(Point::ZERO));
    targetScoreLabel->setPosition((Point(visiableSize.width / 2 + 120, visiableSize.height - 150)));
    background->addChild(targetScoreLabel);
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

/**
 *  onTouchMoved监听
 *
 *  @param touch
 *  @param event
 */
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

/**
 *  onTouchEnded监听
 *
 *  @param touch
 *  @param event
 */
void NormalModeScene::onTouchEnded(Touch *touch, Event *event) {
    // remove lines
    for(auto line : lines) {
        removeChild(line);
    }
    lines.clear();

    // 移除Item
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
        
        /* 记录步数 */
        currentMoves++;
        currentMoveLabel->setString(CCString::createWithFormat("%d",currentMoves)->_string);
        if (currentMoves >= targetMoves && currentScore < targetScore) {
            auto scene = MainMenuScene::createScene();
            TransitionScene *transition = TransitionFade::create(1, scene);
            Director::getInstance()->replaceScene(transition);
            CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        }
        
        /* 播放音效*/
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Ka-Ching.wav");
    }
    
    //
    fallDownItems();
    
    // create new items
    fillTiles();
    
    linePassedTiles.clear();
}


/**
 *  item下落处理
 */
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


/**
 *  填补空的tile
 */
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


/**
 *  查询当前触摸点的tile
 *
 *  @param onTouchX 触摸点横坐标
 *  @param onTouchY 触摸点纵坐标
 *
 *  @return 选中的Tile
 */
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


/**
 *  判断相邻的两个Tile中的Item是否为同一类型
 *
 *  @param lastTile    上一个Tile
 *  @param currentTile 当前Tile
 *
 *  @return
 */
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


/**
 *  删除重复的线
 *
 *  @param onTouchTile <#onTouchTile description#>
 */
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


/**
 *  返回主菜单(Main Menu按钮回调)
 *
 *  @param sender <#sender description#>
 */
void NormalModeScene::mainMenuCallback(Ref *sender) {
    auto scene = MainMenuScene::createScene();
    TransitionFlipX *transition = TransitionFlipX::create(1.2, scene);
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    Director::getInstance()->replaceScene(transition);
}

int NormalModeScene::getStage() {
    return this->stage;
}

void NormalModeScene::setStage(int stage) {
    this->stage = stage;
}
