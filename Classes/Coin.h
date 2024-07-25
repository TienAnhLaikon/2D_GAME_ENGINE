#ifndef _COIN_H_
#define _COIN_H_
#include"Engine.h"
#include "cocos2d.h"
USING_NS_CC;

class Coin : public cocos2d::Node {
private:
	Animation* coinAnimation;
	Sprite* coinSprite;
public:
	std::string coinStatus;

	bool init();
	static Coin* createCoin();
	void playCoinAnimation();
	CREATE_FUNC(Coin);
};
#endif // !_COIN_H
