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

#include "GameOverScene.h"

USING_NS_CC;

Scene* GameOver::createScene()
{
    return GameOver::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameOverScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameOver::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    GameOverBackground = Sprite::create("Game Over Background.jpg");
    //GameOverBackground->setPosition(Director::getInstance()->getVisibleSize() / 2);
    GameOverBackground->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2 + 40));
    CCLOG("%f %f",visibleSize.width,visibleSize.height);
    GameOverBackground->setScale(0.3);

    // thêm background và scene
    this->addChild(GameOverBackground, -1);
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    // add a "close" icon to exit the progress. it's an autorelease object

    // Add label "PLAY" with callback GameOver::play
    auto play = MenuItemLabel::create(Label::createWithTTF("PLAY AGAIN", "fonts/Marker Felt.ttf", 30),
        CC_CALLBACK_1(GameOver::playAgain, this));
    // Assert that play is not null
    CCASSERT(play != nullptr, "problem loading fonts/Marker Felt.ttf");

    //// Position PLAY in the middle of screen
    play->setPosition(Vec2(240, 150));
    // add label "QUIT" with callback GameOver::menuCloseCallback
    auto quit = MenuItemLabel::create(Label::createWithTTF("BACK TO MENU", "fonts/Marker Felt.ttf", 30),
        CC_CALLBACK_1(GameOver::menuCloseCallback, this));
    CCASSERT(quit != nullptr, "problem loading fonts/Marker Felt.ttf");
    quit->setPosition(Vec2(240, 100));

    //// create menu, it's an autorelease object
    auto menu = Menu::create(play, quit, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    return true;
}

void GameOver::playAgain(Ref* pSender) {
    auto gameScene = GamePlay::createScene();
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, gameScene, Color3B(0, 255, 255)));
}
void GameOver::menuCloseCallback(Ref* pSender)
{
    auto mainScene = MainMenu::createScene();
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, mainScene, Color3B(0, 255, 255)));
}
