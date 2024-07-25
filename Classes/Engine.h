#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "cocos2d.h"

USING_NS_CC;

class Engine
{
public:
    static Engine* getInstance();
    static void destroyInstance();

    static Animation* createAnimation(std::string prefixName, int pFrameOrder, float delay);

private:
    Engine();
    ~Engine();
};

#endif // __ENGINE_H__