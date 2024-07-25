#include"Coin.h"

bool Coin::init()
{
	if (!Node::init()) {
		return false;
	}
    coinStatus = "Uncollected";
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("coin.plist", "coin.png");
    // tạo animation cho đối tượng kẻ địch Bread
    coinAnimation = Engine::createAnimation("coin", 9, 0.16f);
    AnimationCache::getInstance()->addAnimation(coinAnimation, "coinSpinAnimation");

    // tạo Sprite cho đối tượng
    coinSprite = Sprite::createWithSpriteFrameName("coin1.png");

    // tạo Physics Box cho Coin
    auto coinBody = PhysicsBody::createBox(coinSprite->getContentSize() - Size(5,5), PhysicsMaterial(1.0f, 0.0f, 0.0f));
    coinBody->setContactTestBitmask(true);
    coinBody->setCollisionBitmask(0);
   // coinBody->setPositionOffset(Vec2(0, 0.0f));
    coinBody->setGravityEnable(false);
    coinBody->setDynamic(false);
    this->addComponent(coinBody);
    coinSprite->setScale(0.7);
    // Cài đặt giới hạn xoay cho physics body
    playCoinAnimation();
    this->addChild(coinSprite);
	return true;
}

Coin* Coin::createCoin()
{
    return Coin::create();
}

void Coin::playCoinAnimation()
{
    coinSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("coinSpinAnimation"))));

}
