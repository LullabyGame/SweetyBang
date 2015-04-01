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
        linePassedTiles.pushBack(lastPaintedTile);
        if (onTouchTile->getItem()->getItemSpecialType() == 1) {
            itemSpecialAction(1,onTouchTile);
        }else if (onTouchTile->getItem()->getItemSpecialType() == 2){
            itemSpecialAction(2,onTouchTile);
        }
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
        //这个if或许需要删除
        if (!linePassedTiles.contains(lastPaintedTile)) {
            linePassedTiles.pushBack(lastPaintedTile);
        }
        lastPaintedTile = onTouchTile;
        linePassedTiles.pushBack(onTouchTile);
        
        
        /* 增加会产生特殊元素的标识*/
        if (linePassedTiles.size() != 0 && linePassedTiles.size() % 5 == 0) {
            Sprite * testspr = Sprite::create("res/img/chazi_shu.png");
            
            testspr->setAnchorPoint(Vec2(0.5, 0.5));
            testspr->setTag(1);
            testspr->setScale(0.7, 0.7);
            testspr->setPosition(Vec2(tileSideLength * 0.4, tileSideLength * 0.5));
            onTouchTile->getItem()->addChild(testspr,1);
            testspr->runAction(RepeatForever::create(RotateBy::create(1, 720)));
            /* 停止动作后重启动作 */
            Director::getInstance()->getActionManager()->pauseTarget(testspr);
            Director::getInstance()->getActionManager()->resumeTarget(testspr);
        }
        /* 如果遇到特殊元素操作 */
        if (onTouchTile != NULL) {
            if (onTouchTile->getItem()->getItemSpecialType() == 1) {
                itemSpecialAction(1,onTouchTile);
            }else if (onTouchTile->getItem()->getItemSpecialType() == 2){
                itemSpecialAction(2,onTouchTile);
            }
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
        /* 当连线元素大于等于5个的时候出现特殊元素，每5个出现一个 */
        /* 特殊元素消除的区域不作为奖励 */
        if (this->linePassedTiles.size() >= 5) {
            for (int i = 0 ; i < linePassedTiles.size(); i++) {
                if ((i+1)%5 == 0) {
                    int start_prox = linePassedTiles.at(i)->getPosX() + tileSideLength / 2;
                    int start_proy = linePassedTiles.at(i)->getPosY() + tileSideLength / 2;
                    
                    /* 随机从tile列表中取出一个tile，如果该元素已经有特殊元素标识，则选择其他元素 */
                    TileSprite* tile = NULL;
                    while (true) {
                        int x = rand()%MATRIX_WIDTH;
                        int y = rand()%MATRIX_HEIGHT;
                        if (tileMatrix[x][y]->getItem()->getItemSpecialType() == 0) {
                            tile = tileMatrix[x][y];
                            break;
                        }
                    }
                    
                    int end_prox = tile->getPosX() + tileSideLength / 2;
                    int end_proy = tile->getPosY() + tileSideLength / 2;
                    /* 叉子飞起效果 */
                    auto move = Sprite::create("res/img/chazi_shu.png");
                    move->setPosition(Vec2(start_prox,start_proy));
                    this->addChild(move);
                    move->runAction(
                                    Sequence::create(
                                        Spawn::create(MoveTo::create(1, Point(end_prox, end_proy)), RotateBy::create(1, 720), NULL),
                                        CallFuncN::create(CC_CALLBACK_1(NormalModeScene::removeAction,this)),
                                        CallFunc::create([tile](){
                                            /* 取一个0或者1的随机数，0给出的特殊元素是一列，1给出的特殊元素是一行 */
                                            int directionType = rand()%2;
                                            if (directionType == 0) {
                                                auto chazi_shu = Sprite::create("res/img/chazi_shu.png");
                                                chazi_shu->setPosition(Vec2(tileSideLength * 0.4, tileSideLength * 0.5));
                                                chazi_shu->setTag(2);
                                                chazi_shu->setScale(0.7, 0.7);
                                                tile->getItem()->addChild(chazi_shu);
                                                /* 给特殊元素设置标记 */
                                                tile->getItem()->setItemSpecialType(1);
                                            }else{
                                                auto chazi_heng = Sprite::create("res/img/chazi_heng.png");
                                                chazi_heng->setPosition(Vec2(tileSideLength * 0.4, tileSideLength * 0.5));
                                                chazi_heng->setTag(2);
                                                chazi_heng->setScale(0.7, 0.7);
                                                tile->getItem()->addChild(chazi_heng);
                                                /* 给特殊元素设置标记 */
                                                tile->getItem()->setItemSpecialType(2);
                                            }
                                        }),NULL
                                    )
                    );
                }
            }
        }
        
        /* 如果连线元素大于3个，把特殊元素导致删除的元素加进来 */
        linePassedTiles.pushBack(itemSpecialVector);
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
        
    }else{
        /* 当连线不够三个的时候不作处理，但是删除特殊元素的效果 */
        for (int i = 0; i < MATRIX_WIDTH; i++) {
            for (int j = 0; j < MATRIX_HEIGHT ; j++) {
                if (tileMatrix[i][j] != NULL) {
                    tileMatrix[i][j]->getItem()->removeChildByTag(4);
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
 *  1、删除重复的线
 *  2、删除特殊元素标识
 *  3、删除特殊元素标记的区域标识
 *
 *  @param onTouchTile
 */
void NormalModeScene::deleteDepetitionLine(TileSprite* onTouchTile) {
    for (auto sprite : lines) {
        if (sprite->getBeginTile() == onTouchTile) {
            removeChild(sprite);
            lines.erase(lines.find(sprite));
            linePassedTiles.erase(linePassedTiles.find(sprite->getEndTile()));
            /* 删除特殊元素标识 */
            sprite->getEndTile()->getItem()->removeChildByTag(1);
            /* 删除特殊元素标记的区域标识 */
            if (sprite->getEndTile()->getItem()->getItemSpecialType() == 1) {
                for (int i = 0; i < MATRIX_HEIGHT; i++) {
                    if (tileMatrix[sprite->getEndTile()->getArrayX()][i] != NULL) {
                        tileMatrix[sprite->getEndTile()->getArrayX()][i]->getItem()->removeChildByTag(4);
                        /* 删掉特殊元素列表中的撤销的元素数据 */
                        itemSpecialVector.erase(itemSpecialVector.find(tileMatrix[sprite->getEndTile()->getArrayX()][i]));
                    }
                }
            }else if (sprite->getEndTile()->getItem()->getItemSpecialType() == 2){
                for (int i = 0; i < MATRIX_WIDTH; i++) {
                    if (tileMatrix[i][sprite->getEndTile()->getArrayY()] != NULL) {
                        tileMatrix[i][sprite->getEndTile()->getArrayY()]->getItem()->removeChildByTag(4);
                        /* 删掉特殊元素列表中的撤销的元素数据 */
                        itemSpecialVector.erase(itemSpecialVector.find(tileMatrix[i][sprite->getEndTile()->getArrayY()]));
                    }
                }
            }
            
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


/**
 *  对特殊元素的处理：
 *      1、增加动画效果。
 *      2、把需要删除的item放入列表中.
 *
 */
void NormalModeScene::itemSpecialAction(int itemSpecialType, TileSprite *onTouchTile){
    /*  此处添加删除范围的动画效果 */
    int x = onTouchTile->getArrayX();
    int y = onTouchTile->getArrayY();
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("res/animation/chahaoxulie.plist");
    Vector<SpriteFrame*> vec;
    char name[20];
    memset(name, 0, 20);

    if (itemSpecialType == 1) {
        for (int i = 0; i < MATRIX_HEIGHT; i++) {
            for (int j = 0; j < 16; j++) {
                sprintf(name, "y1%04d",j);
                vec.pushBack(cache->getSpriteFrameByName(name));
            }
            Animation * animation = Animation::createWithSpriteFrames(vec,0.05f);
            Animate * animate = Animate::create(animation);
            spriteAction = Sprite::create();
            /* 此处防止地图中间有空的tile */
            if (tileMatrix[x][i] != NULL) {
                tileMatrix[x][i]->getItem()->addChild(spriteAction);
                /* 如果第一次点击的元素是特殊元素，那么判断特殊元素的性质把对应需要消除的格子放到itemSpecialVector里面
                 在消除的时候如果判断连线元素大于3个以后，把连线的的vector和特殊元素的vector结合在一起 */
                itemSpecialVector.pushBack(tileMatrix[x][i]);
                spriteAction->setAnchorPoint(Point(0, 0));
                spriteAction->setPosition(Vec2(0, 0));
                spriteAction->setScale(0.7, 0.7);
                spriteAction->setTag(4);
                spriteAction->runAction(RepeatForever::create(animate));
                /* 停止动作后重启动作 */
                Director::getInstance()->getActionManager()->pauseTarget(spriteAction);
                Director::getInstance()->getActionManager()->resumeTarget(spriteAction);
            }
        }
    }else if (itemSpecialType == 2){
        for (int i = 0; i < MATRIX_WIDTH; i++) {
            for (int j = 0; j < 16; j++) {
                sprintf(name, "y1%04d",j);
                vec.pushBack(cache->getSpriteFrameByName(name));
            }
            Animation * animation = Animation::createWithSpriteFrames(vec,0.05f);
            Animate * animate = Animate::create(animation);
            spriteAction = Sprite::create();
            /* 此处防止地图中间有空的tile */
            if (tileMatrix[i][y] != NULL) {
                tileMatrix[i][y]->getItem()->addChild(spriteAction);
                /* 如果第一次点击的元素是特殊元素，那么判断特殊元素的性质把对应需要消除的格子放到itemSpecialVector里面
                 在消除的时候如果判断连线元素大于3个以后，把连线的的vector和特殊元素的vector结合在一起 */
                itemSpecialVector.pushBack(tileMatrix[i][y]);
                spriteAction->setAnchorPoint(Point(0, 0));
                spriteAction->setPosition(Vec2(0, 0));
                spriteAction->setScale(0.7, 0.7);
                spriteAction->setTag(4);
                spriteAction->runAction(RepeatForever::create(animate));
                /* 停止动作后重启动作 */
                Director::getInstance()->getActionManager()->pauseTarget(spriteAction);
                Director::getInstance()->getActionManager()->resumeTarget(spriteAction);
            }
        }
    }
}
