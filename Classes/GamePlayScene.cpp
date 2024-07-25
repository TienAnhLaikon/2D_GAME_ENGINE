#include "GamePlayScene.h"

USING_NS_CC;
//std::vector <Rect*> listMarginBoundingBox;
bool isJumpKeyPressed = false;
float jumpTime = 0.0f;
float maxJumpTime = 0.2f; // Thời gian nhảy tối đa, có thể điều chỉnh
Scene* GamePlay::createScene()
{    
    // scene is an autorelease object
    auto scene = Scene::createWithPhysics();
   // scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2(0, -300));
    scene->getPhysicsWorld()->setFixedUpdateRate(1000.0f);
    // layet is autorelease object
    auto layer = GamePlay::create();
    layer->setPhysicsWorld(scene->getPhysicsWorld());
    // add layer as a child to scene
    scene->addChild(layer);
    return scene;
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GamePlayScene.cpp\n");
}

bool GamePlay::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    coinCollected = 0;
    enemiesList = Vector<Enemy*>();
    visibleSize = Director::getInstance()->getVisibleSize();
     origin = Director::getInstance()->getVisibleOrigin();

     // Tạo label hiển thị số lượng Coin
     coinLabel = cocos2d::Label::createWithTTF("Coins collected: 0", "fonts/Marker Felt.ttf", 14);
     coinLabel->setPosition(Vec2(100, Director::getInstance()->getVisibleSize().height));
     this->addChild(coinLabel, 1);

     // Tạo label hiển thị số lượng Coin
     stageLabel = cocos2d::Label::createWithTTF("Stage: 1", "fonts/Marker Felt.ttf", 14);
     stageLabel->setPosition(Vec2(400, Director::getInstance()->getVisibleSize().height));
     this->addChild(stageLabel, 1);
 
     camera = this->getDefaultCamera();
    loadMap("Map/Map1.tmx");
    backgroundGameplayTheme = AudioEngine::play2d("SuperMarioTheme.mp3",true,0.5f);


    //this->addChild(_tileMap);
    KeyboardInput::getInstance()->addKey(EventKeyboard::KeyCode::KEY_SPACE);

    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(GamePlay::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(GamePlay::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GamePlay::onContactBegin, this);
    contactListener->onContactSeparate = CC_CALLBACK_1(GamePlay::onContactSeparate, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    this->scheduleUpdate();
    return true;
}
void GamePlay::update(float dt) {
    followCharacter();
    for (auto enemy : enemiesList) {
        enemy->autoMove(0.0f);
    }
    if (cat->getPlayerSprite()->getPosition().x < (_tileMap->getMapSize().width / 2)) {
        cat->getPlayerSprite()->setPositionX((_tileMap->getMapSize().width / 2));
        CCLOG("TRUE");
    }

    //else if (cat->getPlayerSprite()->getPosition().x > (((_tileMap->getMapSize().width) * _tileMap->getTileSize().width - visibleSize.width) + visibleSize.width / 2 + (_tileMap->getMapSize().width))) {
    //    cat->getPlayerSprite()->setPositionX(((_tileMap->getMapSize().width) * _tileMap->getTileSize().width - visibleSize.width) + visibleSize.width / 2 + (_tileMap->getMapSize().width));
    //}
    //CCLOG("%f %f", cat->getPlayerSprite()->getPosition().x, cat->getPlayerSprite()->getPosition().y);

    // tạo thêm 1 biến lưu giữ nút bấm hiện tại. Có hàm check nếu phím hiện tại khác A hoặc D và isJumping == false và isOnGround = true thì gọi hàm StopRunning của cat
        
    // xử lý game over
    if (cat->isFallsOffMap()) {
        handleGameOver();
    }
    if (checkIsFinishedMap()) {
//        AudioEngine::stopAll();

        currentMapLevel++;
        updateStageLevel(currentMapLevel);
        mapLv = "Map/Map" + std::to_string(currentMapLevel) + ".tmx";
        CCLOG("Current Map Level: %d", currentMapLevel);
        CCLOG("%s", mapLv.c_str());

        //AudioEngine::play2d("Stage Clear.mp3", false, 1.0f);

        loadMap(mapLv);
        
    }
}

void GamePlay::onEnter()
{
    Scene::onEnter();
    if (KeyboardInput::getInstance()->getParent() != nullptr)
        KeyboardInput::getInstance()->removeFromParent();
    Size size = Director::getInstance()->getOpenGLView()->getFrameSize();
    this->addChild(KeyboardInput::getInstance());
}

void GamePlay::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);

}

bool GamePlay::checkIsFinishedMap()
{
    auto t1 = endPointBoundingBox->getMinX();
    auto t2 = endPointBoundingBox->getMinY();
   // CCLOG("%f %f c", cat->getPlayerSprite()->getPosition().x, cat->getPlayerSprite()->getPosition().y);
    if (endPointBoundingBox->containsPoint(cat->getPlayerSprite()->getPosition())) {
       CCLOG("TRUE");
        return true;

    }
    else {
        //CCLOG("FALSE");
        return false;
    }
}

void GamePlay::setPhysicsWorld(PhysicsWorld* m_world)
{
    world = m_world;
}
bool GamePlay::onContactBegin(PhysicsContact& contact) {
    auto shapeA = contact.getShapeA()->getBody();
    auto shapeB = contact.getShapeB()->getBody();
    if (shapeA->getCollisionBitmask() == 1 && shapeB->getCollisionBitmask() == 2 ||
        shapeA->getCollisionBitmask() == 2 && shapeB->getCollisionBitmask() == 1) {
        if (shapeA->getNode() != nullptr && shapeB->getNode() != nullptr) {
            if (shapeA->getCollisionBitmask() == 1 && shapeB->getCollisionBitmask() == 2) {
                //auto cat = dynamic_cast<Sprite*>(shapeB->getNode());
                //cat->getPhysicsBody()->applyImpulse(Vec2(0,600));
                cat->isJumping = false;
                cat->isFalling = false;


                if (!cat->isJumping && cat->getPlayerSprite()->getPhysicsBody()->getVelocity().x == 0) {
                   cat->playIdleAnimation();
                }
                else {
                    cat->playRunningAnimation();
                }
            }
            else if(shapeA->getCollisionBitmask() == 2 && shapeB->getCollisionBitmask() == 1){
                //auto cat = dynamic_cast<Sprite*>(shapeA->getNode());
                //cat->getPhysicsBody()->applyImpulse(Vec2(0, 600));
                cat->isJumping = false;
                cat->isFalling = false;

               if (!cat->isJumping && cat->getPlayerSprite()->getPhysicsBody()->getVelocity().x == 0) {
                    cat->playIdleAnimation();
                }
               else {
                   cat->playRunningAnimation();
               } 
            }
        }
        cat->isJumping = false;
        cat->isOnGround = true;
        cat->redFlag = true;

    }
    // xử lý enemy quay đầu
    if (shapeA->getCollisionBitmask() == 0 && shapeB->getCollisionBitmask() == 12 ||
        shapeA->getCollisionBitmask() == 12 && shapeB->getCollisionBitmask() == 0) {
        if (shapeA->getNode() != nullptr && shapeB->getNode() != nullptr) {
            if (shapeA->getCollisionBitmask() == 0 && shapeB->getCollisionBitmask() == 12) {
                Enemy* enemy = static_cast<Enemy*>(shapeB->getNode());
                if (enemy->getDirection() == Direction::RIGHT) {
                    enemy->setDirection(Direction::LEFT);
                }
                else {
                    enemy->setDirection(Direction::RIGHT);
                }

            }
            else if (shapeA->getCollisionBitmask() == 12 && shapeB->getCollisionBitmask() == 0) {
                Enemy* enemy = static_cast<Enemy*>(shapeA->getNode());

                if (enemy->getDirection() == Direction::RIGHT) {
                    enemy->setDirection(Direction::LEFT);
                }
                else {
                    enemy->setDirection(Direction::RIGHT);
                }
            }
        }
    }
    // xử lý nếu player chạm vào enemy thì game sẽ kết thúc
    if (shapeA->getCollisionBitmask() == 2 && shapeB->getCollisionBitmask() == 12 ||
        shapeA->getCollisionBitmask() == 12 && shapeB->getCollisionBitmask() == 2) {
        if (shapeA->getNode() != nullptr && shapeB->getNode() != nullptr) {
            if (shapeA->getCollisionBitmask() == 2 && shapeB->getCollisionBitmask() == 12) {
                handleGameOver();
            }
            else if (shapeA->getCollisionBitmask() == 12 && shapeB->getCollisionBitmask() == 2) {
                handleGameOver();
            }
        }
    }
    // xử lý nếu player chạm vào Coin
    if (shapeA->getCollisionBitmask() == 2 && shapeB->getCollisionBitmask() == 0 ||
        shapeA->getCollisionBitmask() == 0 && shapeB->getCollisionBitmask() == 2) {
        
        if (shapeA->getNode() != nullptr && shapeB->getNode() != nullptr) {
            if (shapeA->getCollisionBitmask() == 2 && shapeB->getCollisionBitmask() == 0) {
           
                Coin* coin = static_cast<Coin*>(shapeB->getNode());
                if (coin->coinStatus == "Uncollected") {
                    coin->removeFromParent();
                    AudioEngine::play2d("Get-Coin.mp3", false, 0.5f);
                    coinCollected++;
                    updateCoinCount(coinCollected);
                }
            }
            else if (shapeA->getCollisionBitmask() == 0 && shapeB->getCollisionBitmask() == 2) {
                Coin* coin = static_cast<Coin*>(shapeA->getNode());
                if (coin->coinStatus == "Uncollected") {
                    coin->removeFromParent();
                    AudioEngine::play2d("Get-Coin.mp3", false, 0.5f);
                    coinCollected++;
                    updateCoinCount(coinCollected);

                }

            }
        }
    }


    return true;
}

void GamePlay::onContactSeparate(PhysicsContact& contact)
{
    auto shapeA = contact.getShapeA()->getBody();
    auto shapeB = contact.getShapeB()->getBody();
    if (shapeA->getCollisionBitmask() == 1 && shapeB->getCollisionBitmask() == 2 ||
        shapeA->getCollisionBitmask() == 2 && shapeB->getCollisionBitmask() == 1) {
        if (shapeA->getNode() != nullptr && shapeB->getNode() != nullptr) {
            if (shapeA->getCollisionBitmask() == 1 && shapeB->getCollisionBitmask() == 2) {

            }
            else if (shapeA->getCollisionBitmask() == 2 && shapeB->getCollisionBitmask() == 1) {

            }
        }
        cat->isOnGround = false;
    }
}

void GamePlay::loadMap(std::string mapName)
{
    if (_tileMap) {
        _tileMap->removeFromParentAndCleanup(true);
        _tileMap->removeAllChildren();
    }
    // load file map từ .tmx
    _tileMap = new TMXTiledMap();
    if (!_tileMap->initWithTMXFile(mapName)) {
        handleWinGame();
    }
    else {
        _tileMap->setPosition(Vec2());

        // lấy ra layer Ground từ map
        TMXLayer* groundLayer = _tileMap->getLayer("Ground");
        TMXObjectGroup* groundObjectLayer = _tileMap->getObjectGroup("Ground Line");

        if (groundObjectLayer) {
            // Lấy danh sách các đối tượng từ object layer
            ValueVector objects = groundObjectLayer->getObjects();

            // Duyệt qua từng đối tượng và gán physics body
            for (const auto& object : objects) {
                ValueMap objectProperties = object.asValueMap();

                float x = objectProperties["x"].asFloat();
                float y = objectProperties["y"].asFloat();
                float width = objectProperties["width"].asFloat();
                float height = objectProperties["height"].asFloat();

                // Lấy ra tọa độ trái và phải từ physics body

            // Tạo sprite và physics body
                auto sprite = Sprite::create();
                auto physicsBody = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(1.0f, 0.0f, 0.0f));
                physicsBody->setDynamic(false);
                physicsBody->setGravityEnable(false);
                physicsBody->setContactTestBitmask(true);
                physicsBody->setCollisionBitmask(1);
                sprite->setPhysicsBody(physicsBody);
                // Đặt vị trí cho sprite và thêm vào scene
                sprite->setPosition(Vec2(x + width / 2, y + height / 2));
                _tileMap->addChild(sprite);
            }
        }
        if (!setStartPointToPlayer()) {
            CCLOG("ERROR to set start point");
        }
        if (!setEndPointToNextLevel()) {
            CCLOG("ERROR to set end point");
        }
        if (!setEnemyOnMap()) {
            CCLOG("ERROR to set enemy on map");
        }
        if (!setCoinOnMap()) {
            CCLOG("ERROR to set coin on map");
        }
        if (!setMargin()) {
            CCLOG("ERROR to set margin on map");
        }

        if (_tileMap) {
            _tileMap->setPosition(Vec2::ZERO);
            _tileMap->setAnchorPoint(Vec2::ZERO);
            this->addChild(_tileMap);
        }
    }


}

bool GamePlay::setStartPointToPlayer()
{// lấy ra layer "Start Point" từ map để làm điểm xuất phát cho player
    TMXObjectGroup* startPointObjectGroup = _tileMap->getObjectGroup("Start Point");
    // kiểm tra xem Object Group có tồn tại không
    if (startPointObjectGroup) {
        // lấy thông tin của Object tại index 0 
        ValueMap startPoint = startPointObjectGroup->getObject("Start Point");
        // lấy ra tọa độ x và y từ object đó
        float startX = startPoint["x"].asFloat();
        float startY = startPoint["y"].asFloat();
        CCLOG("startX: %f, startY: %f", startX, startY);
        if (cat) {
            cat->getPlayerSprite()->setPosition(Vec2(startX, startY));
            CCLOG("cat is true");
            CCLOG("%f %f",cat->getPosition().x, cat->getPosition().y);
        }
        else {
            // khởi tạo player
            cat = Player::createPlayer();
            if (cat) {
                cat->getPlayerSprite()->setPosition(Vec2(startX, startY));
                CCLOG("cat is false");
                this->addChild(cat, 2);

            }
        }
    
        return true;
    }
    return false;
}

bool GamePlay::setEndPointToNextLevel()
{
    // lấy đối tượng end point
    TMXObjectGroup* endPointObjectGroup = _tileMap->getObjectGroup("End Point");
    if (endPointObjectGroup) {
        ValueMap endPoint = endPointObjectGroup->getObject("End Point");
        // lấy ra tọa độ x và y từ object đó
        float endPointX = endPoint["x"].asFloat();
        float endPointY = endPoint["y"].asFloat();
        float w = 100;
        float h = 100;
        CCLOG("endPointX:%f  endPointY: %f", endPointX, endPointY);
        endPointBoundingBox = new Rect(endPointX, endPointY, w, h);
        //auto node = Node::create();
        //node->setPosition(endPointX, endPointY);
        //node->setContentSize(Size(w, h));
        //auto nodeBody = PhysicsBody::createBox(node->getContentSize(), PhysicsMaterial(0.3f, 0.0f, 0.0f));
        //nodeBody->setContactTestBitmask(true);
        //nodeBody->setCollisionBitmask(9);
        ////nodeBody->setPositionOffset(Vec2(0, -10.0f));
        //nodeBody->setDynamic(false);
        //node->setPhysicsBody(nodeBody);
        //// Cài đặt giới hạn xoay cho physics body
        //nodeBody->setRotationEnable(false); // Không cho phép xoay
        //_tileMap->addChild(node);
        return true;
    }
    return false;
}

bool GamePlay::setEnemyOnMap()
{    // Tạo ra và hiển thị kẻ địch từ thông tin vị trí của các Enemy object layer
    TMXObjectGroup* enemyObjectLayer = _tileMap->getObjectGroup("Enemy");
    if (enemyObjectLayer) {
        // Lấy danh sách các object trong Enemy Object Layer
        ValueVector& objects = enemyObjectLayer->getObjects();
        //Vector<ValueMap> enemyObjects;

        // Lặp qua từng object và tạo kẻ địch
        for (const auto& enemyObject : objects) {
            ValueMap objectProperties = enemyObject.asValueMap();
            float x = objectProperties["x"].asFloat();
            float y = objectProperties["y"].asFloat();
            countIdEnemy++;
            // Tạo và hiển thị kẻ địch tại vị trí từ object trong Tiled Map
            auto enemy = Enemy::create();
            enemy->eId = countIdEnemy;
            CCLOG("count e: %d", enemy->eId);
            enemy->setPosition(Vec2(x, y));
            enemiesList.pushBack(enemy);
            _tileMap->addChild(enemy, 999);
        }
        return true;
    }
    return false;
}

bool GamePlay::setCoinOnMap()
{// Tạo ra và hiển thị kẻ địch từ thông tin vị trí của các Enemy object layer
    TMXObjectGroup* coinObjectLayer = _tileMap->getObjectGroup("Coin");
    if (coinObjectLayer) {
        // Lấy danh sách các object trong Enemy Object Layer
        ValueVector& objects = coinObjectLayer->getObjects();
        //Vector<ValueMap> enemyObjects;

                // Lặp qua từng object và tạo kẻ địch
        for (const auto& coinObject : objects) {
            ValueMap objectProperties = coinObject.asValueMap();
            float x = objectProperties["x"].asFloat();
            float y = objectProperties["y"].asFloat();

            // Tạo và hiển thị kẻ địch tại vị trí từ object trong Tiled Map
            auto coin = Coin::createCoin();
            coin->setPosition(Vec2(x, y));
            coinsList.pushBack(coin);
            _tileMap->addChild(coin, 999);

        }
        return true;
    }
    return false;
}

bool GamePlay::setMargin()
{
    // Tạo ra và hiển thị kẻ địch từ thông tin vị trí của các Enemy object layer
    TMXObjectGroup* marginObjectLayer = _tileMap->getObjectGroup("Margin");
    if (marginObjectLayer) {
        // Lấy danh sách các object trong Enemy Object Layer
        ValueVector& objects = marginObjectLayer->getObjects();

                // Lặp qua từng object và tạo kẻ địch
        for (const auto& marginObject : objects) {
            ValueMap objectProperties = marginObject.asValueMap();
            float x = objectProperties["x"].asFloat();
            float y = objectProperties["y"].asFloat();
            float w = 15;
            float h = 15;
            auto node = Node::create();
            node->setPosition(x, y);
            node->setContentSize(Size(w, h));
            auto nodeBody = PhysicsBody::createBox(node->getContentSize(), PhysicsMaterial(0.3f, 0.0f, 0.0f));
            nodeBody->setContactTestBitmask(true);
            nodeBody->setCollisionBitmask(0);
            nodeBody->setPositionOffset(Vec2(0, -10.0f));
            nodeBody->setDynamic(false);
            node->setPhysicsBody(nodeBody);
            // Cài đặt giới hạn xoay cho physics body
            nodeBody->setRotationEnable(false); // Không cho phép xoay
            _tileMap->addChild(node);
            listMarginBox.pushBack(node);
        }

    }

    return true;
}

void GamePlay::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {

    //case EventKeyboard::KeyCode::KEY_A:
    //        cat->move(Player::Direction::LEFT);
    //
    //    break;
    //case EventKeyboard::KeyCode::KEY_D:
    //        cat->move(Player::Direction::RIGHT);
    //    break;
    //case EventKeyboard::KeyCode::KEY_SPACE:
    //    if (!cat->isJumping) {
    //        isJumpKeyPressed = true;
    //        jumpTime = 0.0f; // Bắt đầu tính thời gian nhảy
    //    }
    //    break;
        
    case EventKeyboard::KeyCode::KEY_ESCAPE:
        if (!isPausedGame) {
            pauseGame();
        }
        break;
    case EventKeyboard::KeyCode::KEY_ENTER:
        resumeGame();
    }

}

void GamePlay::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{

    switch (keyCode)
    {
    //case EventKeyboard::KeyCode::KEY_A:
    //    // Dừng lại khi nút A hoặc D được thả ra

    //    cat->stopRunning();
    //    break;
    //case EventKeyboard::KeyCode::KEY_D:
    //    // Dừng lại khi nút A hoặc D được thả ra
    //    cat->stopRunning();
    //    break;
    //    if (cat->isOnGround) {
    //case EventKeyboard::KeyCode::KEY_SPACE:
    //    isJumpKeyPressed = false;
    //    if (jumpTime < maxJumpTime) {
    //        cat->lowJump();
    //    }
    //    else {
    //        cat->jump();
    //    }
    //    jumpTime = 0.0f;
    //    break;
    //    }
    //case EventKeyboard::KeyCode::KEY_ESCAPE:
    //    break;
    }
}

void GamePlay::handleSpaceKeyReleased(float pressDuration)
{
    if (pressDuration >= minJumpPressDuration) {
        cat->jump(); // Gọi phương thức xử lý nhảy cao của Player
        CCLOG("JUMP HIGH");
    }
    else {
        cat->lowJump(); // Gọi phương thức xử lý nhảy thấp của Player
        cat->getPlayerSprite()->getPhysicsBody()->applyImpulse(Vec2(cat->getPlayerSprite()->getPhysicsBody()->getVelocity().x, 6000));
        CCLOG("JUMP LOW");

    }
}

void GamePlay::followCharacter() {
    
    if (cat) {
        Vec2 catPos = cat->getPlayerSprite()->getPosition();
        //CCLOG("%f %f", catPos.x, catPos.y);
        // điểm cố định màn hình bên trái trục x là 215
        // Màn hình bên trái
        Size mapSize = _tileMap->getMapSize();
        auto mapSizeWidth = mapSize.width * 16;
        if (catPos.x >= mapSizeWidth - visibleSize.width / 2) {
            camera->setPosition(Vec2(mapSizeWidth - visibleSize.width / 2, 160));
        }
        else if (catPos.x <= mapSizeWidth - visibleSize.width / 2) {
            camera->setPosition(Vec2(catPos.x, 160));
        }
        if (catPos.x <= visibleSize.width / 2) {
            camera->setPosition(Vec2(visibleSize.width / 2, 160));
        }
        else if (catPos.x >= visibleSize.width / 2 && catPos.x <= mapSizeWidth - visibleSize.width / 2) {
            camera->setPosition(Vec2(catPos.x, 160));
        }
        // Cập nhật vị trí của label "Coins Collected"
        coinLabel->setPosition(Vec2(camera->getPositionX() - visibleSize.width / 2 + coinLabel->getContentSize().width / 2 + 10,
            Director::getInstance()->getVisibleSize().height - coinLabel->getContentSize().height / 2));
        // Cập nhật vị trí của label "Stage"
        stageLabel->setPosition(Vec2(camera->getPositionX() + visibleSize.width / 2 - stageLabel->getContentSize().width / 2 - 10,
            Director::getInstance()->getVisibleSize().height - stageLabel->getContentSize().height / 2));
    }
    else {
        CCLOG("NO PLAYER EXIST");
    }
}
void GamePlay::back(Ref* pSender)
{
    AudioEngine::stopAll();
    auto homeScene = MainMenu::createScene();
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, homeScene, Color3B(0, 255, 255)));
}
void GamePlay::handleGameOver()
{
    AudioEngine::stopAll();
    AudioEngine::play2d("Die-Sound.mp3",false,0.5f);
    auto gameOverScene = GameOver::create();
    Director::getInstance()->replaceScene(gameOverScene);
}

void GamePlay::handleWinGame()
{
    AudioEngine::stopAll();
    AudioEngine::play2d("World Clear.mp3", false, 0.5f);

    auto gameWinScene = Win::create();
    if (gameWinScene) {
        Director::getInstance()->replaceScene(gameWinScene);
    }
}

void GamePlay::pauseGame()
{
    isPausedGame = true;
    AudioEngine::pauseAll();
    AudioEngine::play2d("Pause.mp3", false, 0.5f);

    Director::getInstance()->pause();
}

void GamePlay::resumeGame()
{
    isPausedGame = false;

    Director::getInstance()->resume();
    AudioEngine::resumeAll();

}

void GamePlay::updateCoinCount(int count)
{
    coinLabel->setString(StringUtils::format("Coins collected: %d", count));

}

void GamePlay::updateStageLevel(int currentMapLevel)
{
    stageLabel->setString(StringUtils::format("Stage: %d", currentMapLevel));
}



