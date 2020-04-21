#include "Transformable.h"
#include "Utility.h"

Transformable::Transformable()
	: mOrigin(glm::vec2(0.0f))
	, mTransform(Transform())
	, mPosition(glm::vec2(0.f))
	, mNeedsRecalculation(false)
	, mRotation(0.f)
	, mScale(glm::vec2(1.f, 1.f))
{}

void Transformable::setOrigin(glm::vec2 origin) {
	mOrigin = origin;
	mNeedsRecalculation = true;
}

void Transformable::setOrigin(float x, float y) {
	mOrigin = glm::vec2(x, y);
	mNeedsRecalculation = true;
}

glm::vec2 Transformable::getOrigin() const {
	return mOrigin;
}

void Transformable::centerOrigin() {
	setOrigin(static_cast<float>(getWidth()) / 2.f, static_cast<float>(getHeight()) / 2.f);
}

void Transformable::setPosition(glm::vec2 position) {
	mPosition = position;
	mNeedsRecalculation = true;
}

void Transformable::setPosition(float x, float y) {
	mPosition = glm::vec2(x, y);
	mNeedsRecalculation = true;
}

glm::vec2 Transformable::getPosition() const {
	return mPosition;
}

Transform Transformable::getTransform() const {
	if (mNeedsRecalculation)
		recalculateTransform();
	return mTransform;
}

void Transformable::setRotation(float radians) {
	mRotation = radians;
	mNeedsRecalculation = true;
}

float Transformable::getRotation() const {
	return mRotation;
}

void Transformable::setScale(glm::vec2 scale) {
	mScale = scale;
	mNeedsRecalculation = true;
}

void Transformable::setScale(float x, float y) {
	setScale(glm::vec2(x, y));
}

glm::vec2 Transformable::getScale() const {
	return mScale;
}

int Transformable::getWidth() const {
	return getSize().x;
}

int Transformable::getHeight() const {
	return getSize().y;
}

glm::ivec2 Transformable::getSize() const {
	return glm::ivec2(0, 0);
}

FloatRect Transformable::getLocalBounds() const {
	return FloatRect(
		mPosition.x - mOrigin.x,
		mPosition.y - mOrigin.y,
		mPosition.x + getWidth() - mOrigin.x,
		mPosition.y + getHeight() - mOrigin.y
	);
}

void Transformable::recalculateTransform() const {
	SCOPED_PROFILER;
	mTransform = Transform::Identity;
	mTransform.translate(mPosition);
	mTransform.rotate(mRotation);
	mTransform.scale(mScale);
	mTransform.translate(-mOrigin);
}
