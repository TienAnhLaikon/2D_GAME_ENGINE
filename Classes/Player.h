#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cocos2d.h"
#include "Engine.h"
#include "cocos/ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "AudioEngine.h"
#include "KeyBoardInput.h"
#include "AnimationHandler.h"

USING_NS_CC;

class Player : public cocos2d::Node, public AnimationHandler
{
private:
    Animation* catAnimation;
    Animation* runAnimation;
    Animation* jumpAnimation;
    Sprite* catSprite;
    std::string catStatus;

    cocos2d::Vec2 currentVelocity; // Vận tốc hiện tại của nhân vật
    float acceleration = 100.0f; // Hệ số gia tốc khi di chuyển
    float maxSpeed = 500.0f; // Vận tốc tối đa mà nhân vật có thể đạt được
    float deceleration = 200.0f; // Hệ số giảm tốc khi ngừng di chuyển
 
public:
    static enum class Direction {
        LEFT,
        RIGHT
    };
    bool init();
    static Player* createPlayer();
    Sprite* getPlayerSprite();
    Animation* getAnimation(std::string animationName);
    Animate* getAnimate();
    void changeTextureWithFrameName(const std::string& frameName);
    std::string getCatStatus();
    void playIdleAnimation();
    void playRunningAnimation();
    void playJumpingAnimation();
    void setStatus(std::string status);
    void move(Direction direction);
    void handleSpaceKeyReleased(float pressDuration);

    virtual void moveLeft() override;
    virtual void moveRight() override;
    virtual void jump() override;
    void lowJump();
    void stopJump();
    void stopRunning();
    bool isFallsOffMap();
    void update(float dt) override;
    bool isJumping = false;
    bool isOnGround = false;
    bool isFalling = false;
    bool redFlag;
    float minJumpPressDuration = 0.3f;
    float pressDurationTime;
    CREATE_FUNC(Player);



};

#endif 