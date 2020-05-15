#include "PlayerController.h"
#include "RigidBody.h"
#include "Entity.h"
#include "BoxCollider.h"
#include "Tilemap.h"
#include "Window.h"
#include "AnimationRenderer.h"
#include "AnimationController.h"
#include <iostream>
#include <string>

using namespace std::literals::string_literals;

bool PlayerController::handleEvent(Event e) {
	switch (e.type) {
		case Event::Type::KeyPress:
			switch (e.key) {
				case Key::Space:
					if (mIsTouchingMap)
						mRigidBody->accelerate(glm::vec2(0.f, 300.f));
					return true;
			}
			break;
	}
	return false;
}

void PlayerController::onAwake() {
	mRigidBody = getEntity()->getComponent<RigidBody>();
	mWindow = &(getEntity()->getContext().window);
	mAnimationRenderer = getEntity()->getComponent<AnimationRenderer>();
	mAnimationController = getEntity()->getComponent<AnimationController>();
	setupAnimations();
	getEntity()->setOrigin(mAnimationRenderer->getAnimation().getSize().x / 2.f, 0.f);

	auto collider = getEntity()->getComponent<BoxCollider>();

	collider->registerCallback(Collider::CallbackType::Enter, [this] (CollisionInfo info) {
		auto tilemap = info.other->getComponent<Tilemap>();
		if (tilemap) {
			mIsTouchingMap = true;
			if (*mAnimationController->getCurrentAnimationName() != "idle"s)
				mAnimationController->setAnimation("idle");
			std::cout << "Touching\n";
		}
	});

	collider->registerCallback(Collider::CallbackType::Leave, [this](CollisionInfo info) {
		auto tilemap = info.other->getComponent<Tilemap>();
		if (tilemap) {
			mIsTouchingMap = false;
			if (*mAnimationController->getCurrentAnimationName() != "jump"s)
				mAnimationController->setAnimation("jump");
			std::cout << "Not touching\n";
		}
	});
}

void PlayerController::onUpdate(Time dt) {
	if (mWindow->isKeyPressed(Key::Right)) {
		mRigidBody->setVelocity(glm::vec2(80.f, mRigidBody->getVelocity().y));
		if (mIsTouchingMap && *mAnimationController->getCurrentAnimationName() != "run"s) {
			mAnimationController->setAnimation("run");
		}
	}
	if (mWindow->isKeyPressed(Key::Left)) {
		mRigidBody->setVelocity(glm::vec2(-80.f, mRigidBody->getVelocity().y));
		if (mIsTouchingMap && *mAnimationController->getCurrentAnimationName() != "run"s) {
			mAnimationController->setAnimation("run");
		}
	}
	if (!mWindow->isKeyPressed(Key::Left) && !mWindow->isKeyPressed(Key::Right)) {
		mRigidBody->setVelocity(glm::vec2(0.f, mRigidBody->getVelocity().y));
		if (mIsTouchingMap && *mAnimationController->getCurrentAnimationName() != "idle"s) {
			mAnimationController->setAnimation("idle");
		}
	}

	if (mRigidBody->getVelocity().x > 0.f && mAnimationScale.x != 1.f) {
		mAnimationScale = glm::vec2(1.f, 1.f);
	} else if (mRigidBody->getVelocity().x < 0.f && mAnimationScale.x != -1.f) {
		mAnimationScale = glm::vec2(-1.f, 1.f);
	}
	if (mAnimationRenderer->getAnimation().getScale() != mAnimationScale)
		mAnimationRenderer->getAnimation().setScale(mAnimationScale);
}

void PlayerController::setupAnimations() {
	Animation idleAnimation;
	idleAnimation.setTexture(getEntity()->getContext().textureHolder.get(TextureID::PlayerIdle));
	idleAnimation.generateAnimationStates(4, 1, Time::seconds(0.2f));
	idleAnimation.setLooping(true);
	idleAnimation.setOrigin(idleAnimation.getSize().x / 2.f, 0.f);
	mAnimationController->addAnimationState("idle", idleAnimation);

	Animation runAnimation;
	runAnimation.setTexture(getEntity()->getContext().textureHolder.get(TextureID::PlayerRun));
	runAnimation.generateAnimationStates(4, 1, Time::seconds(0.1f));
	runAnimation.setLooping(true);
	runAnimation.setOrigin(runAnimation.getSize().x / 2.f, 0.f);
	mAnimationController->addAnimationState("run", runAnimation);

	Animation jumpAnimation;
	jumpAnimation.setTexture(getEntity()->getContext().textureHolder.get(TextureID::PlayerJump));
	jumpAnimation.generateAnimationStates(1, 1, Time::seconds(1.f));
	jumpAnimation.setLooping(false);
	jumpAnimation.setHold(true);
	jumpAnimation.setOrigin(runAnimation.getSize().x / 2.f, 0.f);
	mAnimationController->addAnimationState("jump", jumpAnimation);
}
