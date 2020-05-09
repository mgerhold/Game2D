#include "PlayerController.h"
#include "RigidBody.h"
#include "Entity.h"
#include "BoxCollider.h"
#include "Tilemap.h"
#include "Window.h"
#include <iostream>

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

	auto collider = getEntity()->getComponent<BoxCollider>();

	collider->registerCallback(Collider::CallbackType::Enter, [this] (CollisionInfo info) {
		auto tilemap = info.other->getComponent<Tilemap>();
		if (tilemap) {
			mIsTouchingMap = true;
			std::cout << "Touching\n";
		}
	});

	collider->registerCallback(Collider::CallbackType::Leave, [this](CollisionInfo info) {
		auto tilemap = info.other->getComponent<Tilemap>();
		if (tilemap) {
			mIsTouchingMap = false;
			std::cout << "Not touching\n";
		}
	});
}

void PlayerController::onUpdate(Time dt) {
	if (mWindow->isKeyPressed(Key::Right))
		mRigidBody->setVelocity(glm::vec2(80.f, mRigidBody->getVelocity().y));
	if (mWindow->isKeyPressed(Key::Left))
		mRigidBody->setVelocity(glm::vec2(-80.f, mRigidBody->getVelocity().y));
	if (!mWindow->isKeyPressed(Key::Left) && !mWindow->isKeyPressed(Key::Right))
		mRigidBody->setVelocity(glm::vec2(0.f, mRigidBody->getVelocity().y));
}
