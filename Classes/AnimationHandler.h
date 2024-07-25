#ifndef __ANIMATION_HANDLER_H__
#define __ANIMATION_HANDLER_H__

#include "cocos2d.h"

class AnimationHandler {
public:
    virtual void moveLeft() = 0;
    virtual void moveRight() = 0;
    virtual void jump() = 0;
    virtual void lowJump() = 0;
    virtual void stopRunning() = 0;


};

#endif // __ANIMATION_HANDLER_H__
