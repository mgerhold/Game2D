#pragma once

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform {
public:
							Transform(glm::mat4 matrix = glm::mat4(1.f));				
	glm::mat4				getMatrix() const;
	void					translate(float x, float y);
	void					translate(glm::vec2 translation);
	void					rotate(float radians);
	void					rotate(float radians, float centerX, float centerY);
	void					rotate(float radians, glm::vec2 center);
	void					scale(float x, float y);
	void					scale(glm::vec2 scaling);
	void					scale(float x, float y, float centerX, float centerY);
	void					scale(glm::vec2 scaling, glm::vec2 center);
	glm::vec2				transformPoint(glm::vec2 point) const;

	Transform				operator*(const Transform& rhs) const;
	Transform&				operator*=(const Transform& rhs);
public:
	static const glm::mat4	Identity;
private:
private:
	glm::mat4				mMatrix;
};