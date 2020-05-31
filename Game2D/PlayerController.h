#pragma once

#include "Component.h"
#include "Sound.h"
#include "Tilemap.h"
#include <glm/vec2.hpp>
#include <functional>

class RigidBody;
class Window;
class AnimationRenderer;
class AnimationController;

class PlayerController : public Component {
public:
	using Callback = std::function<void(void)>;

public:
	bool					handleEvent(Event e) override;
	void					setWinCallback(Callback func);

private:
	void					onAwake() override;
	void					onUpdate(Time dt) override;
	void					setupAnimations();

private:
	RigidBody*				mRigidBody = nullptr;
	const Window*			mWindow = nullptr;
	AnimationRenderer*		mAnimationRenderer = nullptr;
	AnimationController*	mAnimationController = nullptr;
	Tilemap*				mTilemap = nullptr;
	bool					mIsTouchingMap = false;
	bool					mIsReversed = false;
	glm::vec2				mAnimationScale = glm::vec2(1.f);
	Sound					mJumpSound;
	Sound					mFallSound;
	Callback				mWinCallback = nullptr;

	const float				JumpAcceleration = 2000.f;
	const float				MovementVelocity = 600.f;
	const glm::vec2			StartingPosition = glm::vec2(550.f, 400.f);
};