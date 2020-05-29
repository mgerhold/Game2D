#pragma once

#include "Component.h"
#include "Sound.h"
#include <glm/vec2.hpp>

class RigidBody;
class Window;
class AnimationRenderer;
class AnimationController;

class PlayerController : public Component {
public:
	bool					handleEvent(Event e) override;

private:
	void					onAwake() override;
	void					onUpdate(Time dt) override;
	void					setupAnimations();

private:
	RigidBody*				mRigidBody = nullptr;
	const Window*			mWindow = nullptr;
	AnimationRenderer*		mAnimationRenderer = nullptr;
	AnimationController*	mAnimationController = nullptr;
	bool					mIsTouchingMap = false;
	bool					mIsReversed = false;
	glm::vec2				mAnimationScale = glm::vec2(1.f);
	const float				JumpAcceleration = 2000.f;
	const float				MovementVelocity = 600.f;
	Sound					mJumpSound;
};