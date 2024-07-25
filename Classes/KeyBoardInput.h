#ifndef __KEYBOARD_INPUT_H__
#define __KEYBOARD_INPUT_H__

#include "cocos2d.h"
#include "AnimationHandler.h"
USING_NS_CC;

class KeyboardInput : public Node
{
public:
	static KeyboardInput* getInstance();
	Vec2 getDirection();
	void addKey(EventKeyboard::KeyCode key);
	bool getKey(EventKeyboard::KeyCode key);
	void removeKey(EventKeyboard::KeyCode key);
	bool isKeyPressed(cocos2d::EventKeyboard::KeyCode key) const;
	void setAnimationHandler(AnimationHandler* handler);
	// biến xử lý nhảy cao và thấp
	std::chrono::steady_clock::time_point jumpStartTime; // Biến thời gian để ghi lại thời điểm bắt đầu nhấn phím Space
	std::function<void(float)> onSpaceKeyReleasedCallback;
	void KeyboardInput::setOnSpaceKeyReleasedCallback(const std::function<void(float)>& callback);
	bool isSpacePressed;
private:
	bool init() override; 
	void onKeyPressed(EventKeyboard::KeyCode key, Event* ev);
	void onKeyReleased(EventKeyboard::KeyCode key, Event* ev);
	AnimationHandler* animationHandler;

private:
	static KeyboardInput* _instance;
	std::map<EventKeyboard::KeyCode, bool> _keys;
	float _horizontal, _vertical;

};

#endif // !__KEYBOARD_INPUT_H__
