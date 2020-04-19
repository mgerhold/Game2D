#include "Transform.h"

const glm::mat4 Transform::Identity = glm::mat4(1.f);

Transform::Transform(glm::mat4 matrix)
	: mMatrix(matrix)
{}

glm::mat4 Transform::getMatrix() const {
	return mMatrix;
}

void Transform::setMatrix(glm::mat4 matrix) {
	mMatrix = matrix;
}

void Transform::translate(float x, float y) {
	mMatrix = glm::translate(mMatrix, glm::vec3(x, y, 0.f));
}

void Transform::translate(glm::vec2 translation) {
	translate(translation.x, translation.y);
}

void Transform::rotate(float radians) {
	mMatrix = glm::rotate(mMatrix, radians, glm::vec3(0.f, 0.f, 1.f));
}

void Transform::rotate(float radians, float centerX, float centerY) {
	translate(-centerX, -centerY);
	rotate(radians);
	translate(centerX, centerY);
}

void Transform::rotate(float radians, glm::vec2 center) {
	rotate(radians, center.x, center.y);
}

void Transform::scale(float x, float y) {
	mMatrix = glm::scale(mMatrix, glm::vec3(x, y, 0.f));
}

void Transform::scale(glm::vec2 scaling) {
	scale(scaling.x, scaling.y);
}

void Transform::scale(float x, float y, float centerX, float centerY) {
	translate(-centerX, -centerY);
	scale(x, y);
	translate(centerX, centerY);
}

void Transform::scale(glm::vec2 scaling, glm::vec2 center) {
	scale(scaling.x, scaling.y, center.x, center.y);
}

glm::vec2 Transform::transformPoint(glm::vec2 point) const {
	glm::vec4 temp = glm::vec4(point.x, point.y, 0.f, 1.f);
	temp = getMatrix() * temp;
	return glm::vec2(temp.x, temp.y);
}

Transform Transform::operator*(const Transform& rhs) const {
	return Transform(getMatrix() * rhs.getMatrix());
}

Transform& Transform::operator*=(const Transform& rhs) {
	*this = *this * rhs;
	return *this;
}
