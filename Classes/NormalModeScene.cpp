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
    this->remainMoves = stageMove.GetInt();
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
                        ActionInterval *fadein = FadeIn::create(5);
                        item->runAction(fadein);
                        
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
    mainMenuLabel->setPosition(Point(visiableSize.width / 2 - 80, visiableSize.height - 60));
    auto mainMenu = Menu::create(mainMenuLabel, NULL);
    mainMenu->setPosition(0, 0);
    background->addChild(mainMenu);
    
    /* 设定目标分数显示 */
    auto targetLabel = MenuItemLabel::create(LabelTTF::create("Target", "Futura.ttf", 32));
    targetLabel->setColor(Color3B(0, 0, 0));
    targetLabel->setAnchorPoint(Point(0.5, 0.5));
    targetLabel->setPosition(Point(visiableSize.width / 6, visiableSize.height - 100));
    background->addChild(targetLabel);
    
    targetScoreLabel = Label::create(CCString::createWithFormat("%d",targetScore)->_string, "American Typewriter.ttf", 40);
    targetScoreLabel->setTextColor(Color4B(0, 0, 0, 0));
    targetScoreLabel->setAnchorPoint(Point(0.5, 0.5));
    targetScoreLabel->setPosition((Point(visiableSize.width / 6, visiableSize.height - 150)));
    background->addChild(targetScoreLabel);
    
    /* 设定步数显示 */
    auto moveLabel = MenuItemLabel::create(LabelTTF::create("Move", "Futura.ttf", 32));
    moveLabel->setColor(Color3B(0, 0, 0));
    moveLabel->setAnchorPoint(Point(0.5, 0.5));
    moveLabel->setPosition(Point(visiableSize.width / 2, visiableSize.height - 100));
    background->addChild(moveLabel);
    
    remainMovesLabel = Label::create(CCString::createWithFormat("%d",remainMoves)->_string, "American Typewriter.ttf", 40);
    remainMovesLabel->setTextColor(Color4B(0, 0, 0, 0));
    remainMovesLabel->setAnchorPoint(Point(0.5, 0.5));
    remainMovesLabel->setPosition((Point(visiableSize.width / 2, visiableSize.height - 150)));
    background->addChild(remainMovesLabel);
    
    /* 设定当前分数显示 */
    auto scoreLabel = MenuItemLabel::create(LabelTTF::create("Score", "Futura.ttf", 32));
    scoreLabel->setColor(Color3B(0, 0, 0));
    scoreLabel->setAnchorPoint(Point(0.5, 0.5));
    scoreLabel->setPosition(Point(visiableSize.width / 6 * 5, visiableSize.height - 100));
    background->addChild(scoreLabel);
    
    currentScoreLabel = Label::create(CCString::createWithFormat("%d",currentScore)->_string, "American Typewriter.ttf", 40);
    currentScoreLabel->setTextColor(Color4B(0, 0, 0, 0));
    currentScoreLabel->setAnchorPoint(Point(0.5, 0.5));
    currentScoreLabel->setPosition((Point(visiableSize.width / 6 * 5, visiableSize.height - 150)));
    background->addChild(currentScoreLabel);
    
    /* 初始化当前分数显示(默认隐藏) */
    scoreNowLabel = Label::createWithSystemFont(CCString::createWithFormat("%ld", scoreNow)->_string, "Consolas", 80);
    scoreNowLabel->setTextColor(Color4B(255, 0, 0, 0));
    scoreNowLabel->setAnchorPoint(Point(0.5, 0.5));
    scoreNowLabel->setPosition(Point(visiableSize.width / 2, visiableSize.height - 210));
    scoreNowLabel->setVisible(false);
    background->addChild(scoreNowLabel);

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
        
        if (linePassedTiles.size() != 0 && linePassedTiles.size() % 5 == 0) {
            
            /* 增加会产生特殊元素的标识，粒子效果有bug，图片无bug */
            Sprite * testspr = Sprite::create("res/img/chazi.png");
            onTouchTile->getItem()->addChild(testspr,1);
            testspr->setAnchorPoint(Vec2(0.5, 0.5));
            testspr->setPosition(Vec2(tileSideLength * 0.4, tileSideLength * 0.5));
        }
    }
}

/**
 *  onTouchEnded监听
 *
 *  @param touch
 *  @param event
 */
void NormalModeScene::onTouchEnded(Touch *touch, Event *event) {
    Size visiableSize = Director::getInstance()->getVisibleSize();
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
        
        /* 记录分数 */
        scoreNow = linePassedTiles.size() * 20;
        currentScore += scoreNow;
        currentScoreLabel->setString(CCString::createWithFormat("%d",currentScore)->_string);
        if (remainMoves > 0 && currentScore > targetScore) {// game over
            back2StageSelectScene();
        }
        
        /* 显示本次分数 */
        scoreNowLabel->setString("+" + CCString::createWithFormat("%ld",scoreNow)->_string);
        auto showAction = Show::create();
        auto moveUp = MoveBy::create(0.4f, Vec2(0, 30));
        auto hideAction = Hide::create();
        auto moveDown = MoveBy::create(0.4f, Vec2(0, -30));
        auto action = Sequence::create(showAction, moveUp, hideAction, moveDown, NULL);
        scoreNowLabel->runAction(action);
        
        /* 记录步数 */
        remainMoves--;
        remainMovesLabel->setString(CCString::createWithFormat("%d",remainMoves)->_string);
        if (remainMoves <= 0 && currentScore < targetScore) {// game over
            back2StageSelectScene();
        }
        
        /* 播放音效*/
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Ka-Ching.wav");
        
        /* 当连线元素大于等于5个的时候出现特殊元素，每5个出现一个 */
        if (this->linePassedTiles.size() >= 5) {
            for (int i = 0 ; i < linePassedTiles.size(); i++) {
                if ((i+1)%5 == 0) {
                    int start_prox = linePassedTiles.at(i)->getPosX() + tileSideLength / 2;
                    int start_proy = linePassedTiles.at(i)->getPosY() + tileSideLength / 2;
                    
                    /* 思路：随机从tile列表中取出一个tile */
                    TileSprite* tile = tileMatrix[rand()%MATRIX_WIDTH][rand()%MATRIX_HEIGHT];
                    int end_prox = tile->getPosX() + tileSideLength / 2;
                    int end_proy = tile->getPosY() + tileSideLength / 2;
                    /* 叉子飞起效果 */
                    auto move = Sprite::create("res/img/chazi.png");
                    move->setPosition(Vec2(start_prox,start_proy));
                    this->addChild(move);
                    move->runAction(
                                Sequence::create(Spawn::create(MoveBy::create(1, Point(abs(start_prox-end_prox), abs(start_proy-end_proy))), RotateBy::create(1, 720), NULL),
                                CallFuncN::create(CC_CALLBACK_1(NormalModeScene::removeAction,this)),NULL)
                                    );
                }
            }
        }
    }
    
    //
    fallDownItems();
    
    // create new items
    fillTiles();
    
    linePassedTiles.clear();
}

/**
 *  删除粒子效果回调函数
 *
 */
void NormalModeScene::removeAction(Node * node){
    this->removeChild(node);
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
    back2StageSelectScene();
}

void NormalModeScene::back2StageSelectScene() {
    auto scene = MainMenuScene::createScene();
    TransitionFlipX *transition = TransitionFlipX::create(1.2, scene);
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    Director::getInstance()->replaceScene(transition);
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

int NormalModeScene::getStage() {
    return this->stage;
}

void NormalModeScene::setStage(int stage) {
    this->stage = stage;
}
