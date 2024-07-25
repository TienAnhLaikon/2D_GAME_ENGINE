#include "CameraFollow.h"

CameraFollow* CameraFollow::create(Sprite* target, Rect fieldOfView)
{
    auto newObject = new CameraFollow();
    if (newObject != nullptr && newObject->init(target, fieldOfView)) {
        newObject->autorelease();
        return newObject;
    }
    CC_SAFE_DELETE(newObject);
    return nullptr;
}

bool CameraFollow::init(Sprite* target, Rect fieldOfView)
{
    if (!target) {
        return false;
    }
    this->_target = target;
    this->_fieldOfView = fieldOfView;

    this->scheduleUpdate();
    return true;
}

void CameraFollow::update(float dt)
{
    if (!_target)
        return;

    // Get the camera's position
    cocos2d::Camera* camera = cocos2d::Director::getInstance()->getRunningScene()->getDefaultCamera();
    cocos2d::Vec2 targetPosition = _target->getPosition();

    float left_x = _fieldOfView.getMinX();
    float down_y = _fieldOfView.getMinY();
    float right_x = _fieldOfView.getMaxX();
    float up_y = _fieldOfView.getMaxY();

    if (targetPosition.x >= left_x && targetPosition.x <= right_x && targetPosition.y >= down_y && targetPosition.y <= up_y) {
        camera->setPosition(targetPosition);
    }
    else {
        if (targetPosition.x < left_x && targetPosition.y < down_y) {
            camera->setPosition(Vec2(left_x, down_y));
        }
        else if (targetPosition.x < left_x && targetPosition.y > up_y) {
            camera->setPosition(Vec2(left_x, up_y));
        }
        else if (targetPosition.x > right_x && targetPosition.y < down_y) {
            camera->setPosition(Vec2(right_x, down_y));
        }
        else if (targetPosition.x > right_x && targetPosition.y > up_y) {
            camera->setPosition(Vec2(right_x, up_y));
        }
        else if (targetPosition.y < down_y) {
            camera->setPosition(Vec2(targetPosition.x, down_y));
        }
        else if (targetPosition.y > up_y) {
            camera->setPosition(Vec2(targetPosition.x, up_y));
        }
        else if (targetPosition.x < left_x) {
            camera->setPosition(Vec2(left_x, targetPosition.y));
        }
        else if (targetPosition.x > right_x) {
            camera->setPosition(Vec2(right_x, targetPosition.y));
        }
    }
}