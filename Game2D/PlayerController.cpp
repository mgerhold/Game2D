#include "PlayerController.h"
#include "RigidBody.h"
#include "Entity.h"
#include "BoxCollider.h"
#include "Window.h"
#include "AnimationRenderer.h"
#include "AnimationController.h"
#include "EntityContainer.h"
#include <iostream>
#include <string>

using namespace std::literals::string_literals;

bool PlayerController::handleEvent(Event e) {
	switch (e.type) {
		case Event::Type::KeyPress:
			switch (e.key) {
				case Key::Space:
					if (mIsTouchingMap) {
						mRigidBody->accelerate(glm::vec2(0.f, mRigidBody->getGravity().y < 0.f ? JumpAcceleration : -JumpAcceleration));
						mJumpSound.play();
					}
					return true;
			}
			break;
	}
	return false;
}

void PlayerController::onAwake() {
	getEntity()->setPosition(StartingPosition);
	mJumpSound.setSoundBuffer(getEntity()->getContext().soundBufferHolder.get(SoundID::PlayerJump));
	mFallSound.setSoundBuffer(getEntity()->getContext().soundBufferHolder.get(SoundID::PlayerFall));

	mRigidBody = getEntity()->getComponent<RigidBody>();
	mWindow = &(getEntity()->getContext().window);
	mAnimationRenderer = getEntity()->getComponent<AnimationRenderer>();
	mAnimationController = getEntity()->getComponent<AnimationController>();
	setupAnimations();
	getEntity()->setOrigin(mAnimationRenderer->getAnimation().getSize().x / 2.f, 0.f);

	const auto find = getEntity()->getEntityContainer()->getEntitiesWithComponent<Tilemap>();
	mTilemap = find.front()->getComponent<Tilemap>();
	assert(mTilemap != nullptr);

	auto collider = getEntity()->getComponent<BoxCollider>();

	// register collision callback for animations
	collider->registerCallback(Collider::CallbackType::Enter, [this] (CollisionInfo info) {
		auto tilemap = info.other->getComponent<Tilemap>();
		if (tilemap) {
			mIsTouchingMap = true;
			/*if (*mAnimationController->getCurrentAnimationName() != "idle"s)
				mAnimationController->setAnimation("idle");*/
		}
	});

	collider->registerCallback(Collider::CallbackType::Leave, [this](CollisionInfo info) {
		auto tilemap = info.other->getComponent<Tilemap>();
		if (tilemap) {
			mIsTouchingMap = false;
			const auto targetAnimationName = mIsReversed ? "jump_reversed"s : "jump"s;
			if (*mAnimationController->getCurrentAnimationName() != targetAnimationName)
				mAnimationController->setAnimation(targetAnimationName);
		}
	});

	// register collision callback for deadly tiles
	collider->registerCallback(Collider::CallbackType::Enter, [this](CollisionInfo info) {
		auto tilemap = info.other->getComponent<Tilemap>();
		if (tilemap) {

		}
	});
}

void PlayerController::onUpdate(Time dt) {
	if (mWindow->isKeyPressed(Key::Right)) {
		mRigidBody->setVelocity(glm::vec2(MovementVelocity, mRigidBody->getVelocity().y));
		const auto targetAnimationName = mIsReversed ? "run_reversed"s : "run"s;
		if (mIsTouchingMap && *mAnimationController->getCurrentAnimationName() != targetAnimationName) {
			mAnimationController->setAnimation(targetAnimationName);
		}
	}
	if (mWindow->isKeyPressed(Key::Left)) {
		mRigidBody->setVelocity(glm::vec2(-MovementVelocity, mRigidBody->getVelocity().y));
		const auto targetAnimationName = mIsReversed ? "run_reversed"s : "run"s;
		if (mIsTouchingMap && *mAnimationController->getCurrentAnimationName() != targetAnimationName) {
			mAnimationController->setAnimation(targetAnimationName);
		}
	}
	if (!mWindow->isKeyPressed(Key::Left) && !mWindow->isKeyPressed(Key::Right)) {
		mRigidBody->setVelocity(glm::vec2(0.f, mRigidBody->getVelocity().y));
		const auto targetAnimationName = mIsReversed ? "idle_reversed"s : "idle"s;
		if (mIsTouchingMap && *mAnimationController->getCurrentAnimationName() != targetAnimationName) {
			mAnimationController->setAnimation(targetAnimationName);
		}
	}

	if (mRigidBody->getVelocity().x > 0.f && mAnimationScale.x != 1.f) {
		mAnimationScale = glm::vec2(1.f, mAnimationScale.y);
	} else if (mRigidBody->getVelocity().x < 0.f && mAnimationScale.x != -1.f) {
		mAnimationScale = glm::vec2(-1.f, mAnimationScale.y);
	}
	if (!mIsReversed && mRigidBody->getGravity().y > 0.f && mIsTouchingMap && mRigidBody->getVelocity().y > 0.f) {
		mIsReversed = true;
	} else if (mIsReversed && mRigidBody->getGravity().y < 0.f && mIsTouchingMap && mRigidBody->getVelocity().y < 0.f) {
		mIsReversed = false;
	}
	if (mAnimationRenderer->getAnimation().getScale() != mAnimationScale)
		mAnimationRenderer->getAnimation().setScale(mAnimationScale);

	constexpr float mapRectTolerance = 2000.f;
	auto mapRect = FloatRect(
		-mapRectTolerance,
		-mapRectTolerance,
		static_cast<float>(mTilemap->getWidth() * mTilemap->getTileWidth()) + mapRectTolerance,
		static_cast<float>(mTilemap->getHeight() * mTilemap->getTileHeight() + mapRectTolerance)
	);
	if (!mapRect.isInside(getEntity()->getPosition().x, getEntity()->getPosition().y)) {
		// outside of map => reset to start
		mFallSound.play();
		getEntity()->setPosition(StartingPosition);
		mRigidBody->setVelocity(glm::vec2(0.f));
	}
}

void PlayerController::setupAnimations() {
	Animation idleAnimation;
	idleAnimation.setTexture(getEntity()->getContext().textureHolder.get(TextureID::PlayerIdle));
	idleAnimation.generateAnimationStates(2, 1, Time::seconds(0.4f));
	idleAnimation.setLooping(true);
	idleAnimation.setOrigin(idleAnimation.getSize().x / 2.f, 0.f);
	mAnimationController->addAnimationState("idle", idleAnimation);

	Animation idleAnimationReversed;
	idleAnimationReversed.setTexture(getEntity()->getContext().textureHolder.get(TextureID::PlayerIdleReversed));
	idleAnimationReversed.generateAnimationStates(2, 1, Time::seconds(0.4f));
	idleAnimationReversed.setLooping(true);
	idleAnimationReversed.setOrigin(idleAnimationReversed.getSize().x / 2.f, 0.f);
	mAnimationController->addAnimationState("idle_reversed", idleAnimationReversed);

	Animation runAnimation;
	runAnimation.setTexture(getEntity()->getContext().textureHolder.get(TextureID::PlayerRun));
	runAnimation.generateAnimationStates(4, 1, Time::seconds(0.1f));
	runAnimation.setLooping(true);
	runAnimation.setOrigin(runAnimation.getSize().x / 2.f, 0.f);
	mAnimationController->addAnimationState("run", runAnimation);

	Animation runAnimationReversed;
	runAnimationReversed.setTexture(getEntity()->getContext().textureHolder.get(TextureID::PlayerRunReversed));
	runAnimationReversed.generateAnimationStates(4, 1, Time::seconds(0.1f));
	runAnimationReversed.setLooping(true);
	runAnimationReversed.setOrigin(runAnimationReversed.getSize().x / 2.f, 0.f);
	mAnimationController->addAnimationState("run_reversed", runAnimationReversed);

	Animation jumpAnimation;
	jumpAnimation.setTexture(getEntity()->getContext().textureHolder.get(TextureID::PlayerJump));
	jumpAnimation.generateAnimationStates(1, 1, Time::seconds(1.f));
	jumpAnimation.setLooping(false);
	jumpAnimation.setHold(true);
	jumpAnimation.setOrigin(runAnimation.getSize().x / 2.f, 0.f);
	mAnimationController->addAnimationState("jump", jumpAnimation);

	Animation jumpAnimationReversed;
	jumpAnimationReversed.setTexture(getEntity()->getContext().textureHolder.get(TextureID::PlayerJumpReversed));
	jumpAnimationReversed.generateAnimationStates(1, 1, Time::seconds(1.f));
	jumpAnimationReversed.setLooping(false);
	jumpAnimationReversed.setHold(true);
	jumpAnimationReversed.setOrigin(runAnimation.getSize().x / 2.f, 0.f);
	mAnimationController->addAnimationState("jump_reversed", jumpAnimationReversed);
}
