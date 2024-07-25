#ifndef _CAMERA_FOLLOW_H_
#define _CAMERA_FOLLOW_H_

#include "cocos2d.h"
USING_NS_CC;

class CameraFollow :public Node
{
private:
	Node* _target;
	Rect _fieldOfView;
public:
	static CameraFollow* create(Sprite* target, Rect fieldOfView);

	virtual bool init(Sprite* target, Rect fieldOfView);

	void update(float dt);
};

#endif // !__CAMERA_FOLLOW_H__