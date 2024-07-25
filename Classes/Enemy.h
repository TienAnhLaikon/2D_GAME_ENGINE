#ifndef _ENEMY_H_
#define _ENEMY_H_
#include"Engine.h"
#include "cocos2d.h"
USING_NS_CC;
static enum class Direction {
	LEFT,
	RIGHT
};
class Enemy : public cocos2d::Node {
	private:
		Animation* breadRunAnimation;
		float startPointX;
		int enemyID;
		float endPointX;
		Direction currentDirection;
public:
	std::string breadStatus;
	Sprite* breadSprite;
	int eId;
	bool init();
	static Enemy* createEnemy();
	void playRunAnimation();
	void autoMove(float delta);
	Direction getDirection();
	void setDirection(Direction p_Direction);
	void update();
	bool checkColisionWithPointObject();
	CREATE_FUNC(Enemy);
};
#endif // !_ENEMY_H

