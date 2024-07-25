#include "Enemy.h"

bool Enemy::init()
{
    if (!Node::init()) {
        return false;
    }
    breadStatus = "Running";
    currentDirection = Direction::RIGHT;
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("bread.plist", "bread.png");
    // tạo animation cho đối tượng kẻ địch Bread
    breadRunAnimation = Engine::createAnimation("breadRun", 4, 0.16f);
    AnimationCache::getInstance()->addAnimation(breadRunAnimation, "BreadRunAnimation");

    // tạo Sprite cho đối tượng
    breadSprite = Sprite::createWithSpriteFrameName("breadRun1.png");

    // tạo Physics Box cho bread
    auto breadBody = PhysicsBody::createBox(breadSprite->getContentSize() - Size(20,20), PhysicsMaterial(1.0f, 0.0f, 0.0f));
    breadBody->setContactTestBitmask(true);
    breadBody->setCollisionBitmask(12);
    breadBody->setLinearDamping(1.0f);
    breadBody->setPositionOffset(Vec2(0, -5.0f));
    //breadBody->setGravityEnable(false);
    breadBody->setDynamic(true);
    this->addComponent(breadBody);
    breadSprite->setScale(0.6);
    // Cài đặt giới hạn xoay cho physics body
    breadBody->setRotationEnable(false); // Không cho phép xoay
    playRunAnimation();
    this->addChild(breadSprite);
    return true;
}

Enemy* Enemy::createEnemy()
{
    auto enemy = Enemy::create();
    return enemy;
}

void Enemy::playRunAnimation()
{
    breadSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("BreadRunAnimation"))));
}

void Enemy::autoMove(float delta)
{
    // xử lý di chuyển
    float velocity = 30; // tốc độ di chuyển
    if (currentDirection == Direction::RIGHT) {
        breadSprite->setFlippedX(false);
        this->getPhysicsBody()->setVelocity(Vec2(velocity, 0));

    }
    else if (currentDirection == Direction::LEFT) {
        breadSprite->setFlippedX(true);
        this->getPhysicsBody()->setVelocity(Vec2(-velocity, 0));
    }

    
}

Direction Enemy::getDirection()
{
    return currentDirection;
}

void Enemy::setDirection(Direction p_Direction)
{
    currentDirection = p_Direction;
}

void Enemy::update()
{

}


bool Enemy::checkColisionWithPointObject()
{// lấy danh sách các Body mà Enemy đang va chạm
    return false;
}

