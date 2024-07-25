/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __GAMEPLAY_SCENE_H__
#define __GAMEPLAY_SCENE_H__
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "CameraFollow.h"
#include "MainMenuScene.h"
#include"Engine.h"
#include"Player.h"
#include "Enemy.h"
#include "Coin.h"
#include"GameOverScene.h"
#include "WinScene.h"
#include "KeyBoardInput.h"
#include "AudioEngine.h"
USING_NS_CC;

class GamePlay : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    Size visibleSize;
    Vec2 origin;
    virtual bool init();
    cocos2d::TMXTiledMap* _tileMap;
    int countIdEnemy = 0;
    Vector <Enemy*> enemiesList;
    Vector <Coin*> coinsList;
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    bool checkIsFinishedMap();
    bool onContactBegin(PhysicsContact& contact);
    void onContactSeparate(PhysicsContact& contact);
    void loadMap(std::string mapName);
    bool setStartPointToPlayer();
    bool setEndPointToNextLevel();
    bool setEnemyOnMap();
    bool setCoinOnMap();
    bool setMargin();
    void update(float dt);
    void onEnter() override;

    Vector<Node*> listMarginBox;

    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    void handleSpaceKeyReleased(float pressDuration);
    void followCharacter();
    void back(Ref* pSender);
    void handleGameOver();
    void handleWinGame();
    void pauseGame();
    void resumeGame();
    void updateCoinCount(int count);
    void updateStageLevel(int currentMapLevel);
    // implement the "static create()" method manually
    float minJumpPressDuration = 0.3f;
    CREATE_FUNC(GamePlay);
private:
    PhysicsWorld* world;
    cocos2d::Label* coinLabel;
    cocos2d::Label* stageLabel;
    void setPhysicsWorld(cocos2d::PhysicsWorld *m_world);
    int currentMapLevel = 1;
    std:: string mapLv;
    int coinCollected;
    bool isPausedGame = false;
    Player* cat;
    Rect *endPointBoundingBox;
    Camera* camera;
    int backgroundGameplayTheme;
    int backgroundMenuTheme;


};

#endif // __GamePlay_SCENE_H__
