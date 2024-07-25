#include "Player.h"


bool Player::init()
{
    if (!Node::init()) {
        return false;
    }
    catStatus = "Idle";
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("cat.plist", "cat.png");
    // tạo animation cho mèo
    catAnimation = Engine::createAnimation("idle", 8, 0.16f);
    runAnimation = Engine::createAnimation("run", 8, 0.16f);
    jumpAnimation = Engine::createAnimation("jump", 8, 0.10f);
    AnimationCache::getInstance()->addAnimation(catAnimation, "IdleAnimation");
    AnimationCache::getInstance()->addAnimation(runAnimation, "RunAnimation");
    AnimationCache::getInstance()->addAnimation(jumpAnimation, "JumpAnimation");
    // tạo Sprite

            catSprite = Sprite::createWithSpriteFrameName("idle1.png");
        // tạo đối tượng mèo
        //catSprite->setPosition(this->getPosition());
         //tạo physics box cho mèo
        auto catBody = PhysicsBody::createBox(catSprite->getContentSize() - Size(20,20), PhysicsMaterial(1, 1, 1));
        catBody->setMass(50.0f);
        catBody->setContactTestBitmask(true);
        catBody->setCollisionBitmask(2);
        catBody->setPositionOffset(Vec2(0, -10.0f));
        catBody->setGravityEnable(true);
        catBody->setDynamic(true);
        catSprite->setPhysicsBody(catBody);
        // Cài đặt giới hạn xoay cho physics body
        catBody->setRotationEnable(false); // Không cho phép xoay
        playIdleAnimation();
        this->addChild(catSprite);
        this->scheduleUpdate();

        auto keyboardInput = KeyboardInput::getInstance();
            KeyboardInput::getInstance()->setOnSpaceKeyReleasedCallback(CC_CALLBACK_1(Player::handleSpaceKeyReleased, this));

        keyboardInput->setAnimationHandler(this);
    return true;
}

Player* Player::createPlayer()
{
    return Player::create();
}
Sprite* Player::getPlayerSprite()
{
    return catSprite;
}

void Player::changeTextureWithFrameName(const std::string& frameName)
{
    cocos2d::SpriteFrame* spriteFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);

    if (spriteFrame) {
        // set the sprite frame for player's sprite
        catSprite->setSpriteFrame(spriteFrame);
    }
    else {
        CCLOG("Sprite frame with name '%s' not found in the cache.", frameName.c_str());
    }
}

std::string Player::getCatStatus()
{
    return catStatus;
}

void Player::playIdleAnimation()
{
    catSprite->stopAllActions();
    catSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("IdleAnimation"))));
}

void Player::playRunningAnimation()
{
    catSprite->stopAllActions();
    catSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("RunAnimation"))));
}

void Player::playJumpingAnimation()
{
    catSprite->stopAllActions();
    Animate* jump = Animate::create(AnimationCache::getInstance()->getAnimation("JumpAnimation"))->clone();
    catSprite->runAction(jump);
}

void Player::setStatus(std::string status)
{
    catStatus = status;
}

void Player::move(Direction direction)
{
    // Dùng hàm moveLeft và moveRight để xử lý di chuyển tương ứng
    switch (direction) {
    case Direction::LEFT:
        moveLeft();
        break;

    case Direction::RIGHT:
        moveRight();
        break;

    default:
        break;
    }
}

void Player::handleSpaceKeyReleased(float pressDuration)
{
    pressDurationTime = pressDuration;
        if (pressDuration >= minJumpPressDuration) {
            jump(); // Gọi phương thức xử lý nhảy cao của Player
            CCLOG("JUMP CAT");
        }
        else {
            lowJump(); // Gọi phương thức xử lý nhảy thấp của Player
            CCLOG("JUMP DOG");

        }
    
}

 void Player::moveLeft() 
{
    catSprite->setFlippedX(true);


    if (!isJumping) {
        playRunningAnimation();
    }
}

void Player::moveRight() 
{
    catSprite->setFlippedX(false);
    if (!isJumping) {
        playRunningAnimation();
    }

}

void Player::jump()
{
    if (isOnGround) {
        AudioEngine::play2d("Jump-High.mp3", false, 0.5f);
        catStatus = "Jumping";
        isJumping = true;
        isOnGround = false;

        catSprite->getPhysicsBody()->applyImpulse(Vec2(catSprite->getPhysicsBody()->getVelocity().x, 10000));


        if (isJumping) {
            playJumpingAnimation();
        }
    }
}

void Player::lowJump()
{
    if (isOnGround) {
        AudioEngine::play2d("Jump-Small.mp3", false, 0.5f);
        catStatus = "LowJumping";
        isOnGround = false;
        catSprite->getPhysicsBody()->applyImpulse(Vec2(catSprite->getPhysicsBody()->getVelocity().x, 6000));
        if (!isJumping && !isFalling) {
            isJumping = true;
        }
        if (isJumping) {
            playJumpingAnimation();
        }
    }

    
}

void Player::stopJump()
{
    catSprite->getPhysicsBody()->setVelocity(Vec2(catSprite->getPhysicsBody()->getVelocity().x, 0));

}

void Player::stopRunning()
{
    if (!isJumping) {
        playIdleAnimation();
    }
}

bool Player::isFallsOffMap()
{
    if (catSprite->getPositionY() < -50) {
        return true;
    }
    else {
        return false;
    }
}


void Player::update(float dt)
{


 /*   if (catSprite->getPosition().x < -60) {
        catSprite->setPositionX(-60);
    }
    else if (catSprite->getPosition().x > 1829) {
        catSprite->setPositionX(1829.0);
    }*/
            // Trong hàm update hoặc hàm xử lý cập nhật vị trí của nhân vật

    if (isOnGround) {
        catSprite->getPhysicsBody()->setVelocity(Vec2::ZERO);
        catStatus = "Idle";
  /*      if (KeyboardInput::getInstance()->isKeyPressed(EventKeyboard::KeyCode::KEY_SPACE)) {
            catSprite->getPhysicsBody()->applyImpulse(Vec2(catSprite->getPhysicsBody()->getVelocity().x, 10000));

        }*/

        if (KeyboardInput::getInstance()->getKey(EventKeyboard::KeyCode::KEY_A)) {
            catSprite->getPhysicsBody()->applyImpulse(Vec2(-1, 0) * 4500);
            catStatus = "Moving";


        }
        if (KeyboardInput::getInstance()->getKey(EventKeyboard::KeyCode::KEY_D)) {
            catSprite->getPhysicsBody()->applyImpulse(Vec2(1, 0) * 4500);
            catStatus = "Moving";


        }

    }
    else {
        catSprite->getPhysicsBody()->setVelocity(Vec2(0, catSprite->getPhysicsBody()->getVelocity().y));
        if (!isJumping) {
            changeTextureWithFrameName("jump8.png");
        }
        if (KeyboardInput::getInstance()->getKey(EventKeyboard::KeyCode::KEY_A)) {
            catSprite->getPhysicsBody()->applyImpulse(Vec2(-1, 0) * 4500);
            catStatus = "Moving";

        }
        if (KeyboardInput::getInstance()->getKey(EventKeyboard::KeyCode::KEY_D)) {
            catSprite->getPhysicsBody()->applyImpulse(Vec2(1, 0) * 4500);
            catStatus = "Moving";

        }
    }

}
