#include "KeyBoardInput.h"
KeyboardInput* KeyboardInput::_instance;
KeyboardInput* KeyboardInput::getInstance() {
	if (_instance == nullptr)
	{
		_instance = new KeyboardInput();
		_instance->init();
		_instance->retain();
	}
	return _instance;
}

void KeyboardInput::setOnSpaceKeyReleasedCallback(const std::function<void(float)>& callback)
{
	onSpaceKeyReleasedCallback = callback;

}

bool KeyboardInput::init() {
	if (!Node::init())
	{
		log("Init KeyboardInput failed");
		return false;
	}

	this->addKey(EventKeyboard::KeyCode::KEY_W);
	this->addKey(EventKeyboard::KeyCode::KEY_A);
	this->addKey(EventKeyboard::KeyCode::KEY_S);
	this->addKey(EventKeyboard::KeyCode::KEY_D);
	this->addKey(EventKeyboard::KeyCode::KEY_SPACE);

	this->addKey(EventKeyboard::KeyCode::KEY_DOWN_ARROW);
	this->addKey(EventKeyboard::KeyCode::KEY_LEFT_ARROW);
	this->addKey(EventKeyboard::KeyCode::KEY_RIGHT_ARROW);
	this->addKey(EventKeyboard::KeyCode::KEY_UP_ARROW);

	// event keyboard
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(KeyboardInput::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(KeyboardInput::onKeyReleased, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

Vec2 KeyboardInput::getDirection()
{
	return Vec2(_horizontal, _vertical);
}

void KeyboardInput::addKey(EventKeyboard::KeyCode key)
{
	if (_keys.find(key) != _keys.end()) return;

	_keys.insert({ key, false });
}

bool KeyboardInput::getKey(EventKeyboard::KeyCode key)
{
	if (_keys.find(key) == _keys.end()) return false;
	return _keys[key];
}

void KeyboardInput::removeKey(EventKeyboard::KeyCode key)
{
	if (_keys.find(key) != _keys.end()) return;

	_keys.erase(key);
}

bool KeyboardInput::isKeyPressed(cocos2d::EventKeyboard::KeyCode key) const
{
	auto it = _keys.find(key);
	if (it != _keys.end()) {
		return it->second;
	}
	return false;
}

void KeyboardInput::setAnimationHandler(AnimationHandler* handler)
{
	animationHandler = handler;
}

void KeyboardInput::onKeyPressed(EventKeyboard::KeyCode key, Event* ev)
{
	if (_keys.find(key) == _keys.end()) return;

	_keys[key] = true;
	switch (key)
	{
	case EventKeyboard::KeyCode::KEY_SPACE:
		jumpStartTime = std::chrono::steady_clock::now(); // Bắt đầu tính thời gian
		isSpacePressed = true;
		break;
	case EventKeyboard::KeyCode::KEY_A:
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		_horizontal = -1;
		animationHandler->moveLeft();

		break;
	case EventKeyboard::KeyCode::KEY_D:
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		_horizontal = 1;
		animationHandler->moveRight();

		break;
	default:
		break;
	}
}

void KeyboardInput::onKeyReleased(EventKeyboard::KeyCode key, Event* ev)
{
	if (_keys.find(key) == _keys.end()) return;

	_keys[key] = false;
	switch (key)
	{

	case EventKeyboard::KeyCode::KEY_A:
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		if (_keys[EventKeyboard::KeyCode::KEY_D]
			|| _keys[EventKeyboard::KeyCode::KEY_RIGHT_ARROW])
		{
			_horizontal = 1;
		}
		else
		{
			_horizontal = 0;
		}
		animationHandler->stopRunning();

		break;
	case EventKeyboard::KeyCode::KEY_D:
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		if (_keys[EventKeyboard::KeyCode::KEY_A]
			|| _keys[EventKeyboard::KeyCode::KEY_LEFT_ARROW])
		{
			_horizontal = -1;
		}
		else
		{
			_horizontal = 0;
		}
		animationHandler->stopRunning();

		break;
	case EventKeyboard::KeyCode::KEY_SPACE:
		if (isSpacePressed) {
			std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
			std::chrono::duration<float> duration = endTime - jumpStartTime;
			float pressDuration = duration.count();

			// Thông báo cho GamePlayScene xử lý việc nhảy dựa trên thời gian giữ phím
			if (onSpaceKeyReleasedCallback) {
				onSpaceKeyReleasedCallback(pressDuration);
			}
		}
		isSpacePressed = false;
		break;
	default:
		break;
	}
}
