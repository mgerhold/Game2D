#include "RigidBody.h"
#include "Entity.h"
#include <iostream>

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