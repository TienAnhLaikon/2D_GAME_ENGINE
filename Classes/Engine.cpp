#include "Engine.h"

static Engine* s_instance = nullptr;

Engine* Engine::getInstance()
{
    if (!s_instance)
    {
        s_instance = new Engine();
    }
    return s_instance;
}

void Engine::destroyInstance()
{
    CC_SAFE_DELETE(s_instance);
}

Engine::Engine()
{
    // Constructor
}

Engine::~Engine()
{
    // Destructor
}

Animation* Engine::createAnimation(std::string prefixName, int pFrameOrder, float delay)
{
    Vector<SpriteFrame*> animFrames;
    for (int i = 1; i <= pFrameOrder; i++)
    {
        char buffer[20] = { 0 };
        sprintf(buffer, "%d.png", i);
        std::string str = prefixName + buffer;
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        if (frame != nullptr)
        {
            animFrames.pushBack(frame);
        }
    }
    auto animation = Animation::createWithSpriteFrames(animFrames, delay);
    return animation;
}
