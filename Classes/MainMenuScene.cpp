﻿/****************************************************************************
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

#include "MainMenuScene.h"

USING_NS_CC;

Scene* MainMenu::createScene()
{
    return MainMenu::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in MainMenuScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MainMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    mainMenuBackground = Sprite::create("Main Menu Background.jpg");
    mainMenuBackground->setPosition(Director::getInstance()->getVisibleSize() / 2);
    mainMenuBackground->setScale(0.5);

    // thêm background và scene
    this->addChild(mainMenuBackground,-1);
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    // add a "close" icon to exit the progress. it's an autorelease object

    // Add label "PLAY" with callback MainMenu::play
    auto play = MenuItemLabel::create(Label::createWithTTF("PLAY", "fonts/Marker Felt.ttf", 40),
        CC_CALLBACK_1(MainMenu::play, this));
    // Assert that play is not null
    CCASSERT(play != nullptr, "problem loading fonts/Marker Felt.ttf");

    // Position PLAY in the middle of screen
    play->setPosition(Vec2(240, 180));
    // add label "QUIT" with callback MainMenu::menuCloseCallback
    auto quit = MenuItemLabel::create(Label::createWithTTF("QUIT", "fonts/Marker Felt.ttf", 40),
        CC_CALLBACK_1(MainMenu::menuCloseCallback, this));
    CCASSERT(quit != nullptr, "problem loading fonts/Marker Felt.ttf");
    quit->setPosition(Vec2(240, 130));

    // create menu, it's an autorelease object
    auto menu = Menu::create(play,quit, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1); 
    CCLOG("BATMAN");
    // background music
    //AudioEngine::play2d("SuperMarioTheme.mp3", true,0.5f);
    //AudioManager::playAudio("SuperMarioTheme.mp3", true, 0.5f);
    
    return true;
}

void MainMenu::play(Ref* pSender) {
    auto gameScene = GamePlay::createScene();
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, gameScene, Color3B(0, 255, 255)));
}
void MainMenu::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
