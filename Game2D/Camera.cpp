#include "Camera.h"

Camera::Camera() noexcept
	: mNeedsRecalculation(false)
	, mMatrix(glm::mat4(1.f))
	, mPosition(glm::vec2(0.f))
	, mZoom(1.f)
	, mRotation(0.f)
{}

void Camera::setPosition(glm::vec2 position) {
	mPosition = position;
	mNeedsRecalculation = true;
}

glm::vec2 Camera::getPosition() const {
	return mPosition;
}

void Camera::setZoom(float zoom) {
	mZoom = zoom;
	mNeedsRecalculation = true;
}

float Camera::getZoom() const {
	return mZoom;
}

glm::mat4 Camera::getMatrix() const {
	if (mNeedsRecalculation)
		calculateMatrix();
	return mMatrix;
}

void Camera::move(glm::vec2 translation) {
	setPosition(getPosition() + translation);
}

void Camera::zoom(float factor) {
	setZoom(getZoom() * factor);
}

void Camera::setRotation(float radians) {
	mRotation = radians;
	mNeedsRecalculation = true;
}

float Camera::getRotation() const {
	return mRotation;
}

void Camera::rotate(float radians) {
	setRotation(getRotation() + radians);
}

void Camera::calculateMatrix() const {
	mMatrix = glm::mat4(1.f);
	mMatrix = glm::scale(mMatrix, glm::vec3(mZoom, mZoom, 1.f));
	mMatrix = glm::rotate(mMatrix, mRotation, glm::vec3(0.f, 0.f, 1.f));
	mMatrix = glm::translate(mMatrix, glm::vec3(-mPosition.x, -mPosition.y, 0.f));
	mNeedsRecalculation = false;
}
