#include "RigidBody.h"
#include "Entity.h"
#include <iostream>
#include "Collider.h"

void RigidBody::setMass(float mass) {
	mMass = mass;
}

float RigidBody::getMass() const {
	return mMass;
}

void RigidBody::accelerate(glm::vec2 acceleration) {
	mVelocity += acceleration;
}

glm::vec2 RigidBody::getVelocity() const {
	return mVelocity;
}

void RigidBody::setVelocity(glm::vec2 velocity) {
	mVelocity = velocity;
}

void RigidBody::setGravity(glm::vec2 gravity) {
	mGravity = gravity;
}

glm::vec2 RigidBody::getGravity() const {
	return mGravity;
}

void RigidBody::setDrag(float drag) {}

float RigidBody::getDrag() const {
	return 0.0f;
}

void RigidBody::onAwake() {
	mEntity = getEntity();
	auto collider = mEntity->getComponent<Collider>();
	if (collider) {
		collider->registerCallback(Collider::CallbackType::Colliding, [this](CollisionInfo info) {
			if (mVelocity != glm::vec2(0.f)) {
				auto t = glm::vec2(0.f);
				if (info.penetration.x < info.penetration.y) {
					if (info.thisBounds.left == info.intersection.left) { // collision from the right side
						t = glm::vec2(info.penetration.x, 0.f);
						if (mVelocity.x <= 0.f)
							mVelocity.x = 0.f;
					} else { // collision from the left side
						t = glm::vec2(-info.penetration.x, 0.f);
						if (mVelocity.x >= 0.f)
							mVelocity.x = 0.f;
					}
				} else {
					if (info.thisBounds.bottom == info.intersection.bottom) { // collision from the top
						t = glm::vec2(0.f, info.penetration.y);
						if (mVelocity.y <= 0.f)
							mVelocity.y = 0.f;
					} else { // collision from the bottom
						t = glm::vec2(0.f, -info.penetration.y);
						if (mVelocity.y >= 0.f)
							mVelocity.y = 0.f;
					}
				}
				mEntity->setPosition(mEntity->getPosition() + t);
			}
		});
	}
}

void RigidBody::onUpdate(Time dt) {
	mVelocity += mGravity * dt.asSeconds();

	if (mMass > 0.f && (mVelocity.x != 0 || mVelocity.y != 0)) {
		glm::vec2 dragForce = -glm::normalize(mVelocity) * mDrag * glm::length(mVelocity) * glm::length(mVelocity);
		glm::vec2 dragAcc = dragForce / mMass;
		mVelocity += dragAcc * dt.asSeconds();
	}

	mEntity->setPosition(mEntity->getPosition() + mVelocity * dt.asSeconds());
	//std::cout << "Falling with vy = " << getVelocity().y << "\n";
}