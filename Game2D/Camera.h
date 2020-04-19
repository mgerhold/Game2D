#pragma once

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
						Camera() noexcept;
	void				setPosition(glm::vec2 position);
	glm::vec2			getPosition() const;
	void				setZoom(float zoom);
	float				getZoom() const;
	glm::mat4			getMatrix() const;
	void				move(glm::vec2 translation);
	void				zoom(float factor);
	void				setRotation(float radians);
	float				getRotation() const;
	void				rotate(float radians);

private:
	void				calculateMatrix() const;

private:
	glm::vec2			mPosition;
	float				mZoom;
	float				mRotation;
	mutable bool		mNeedsRecalculation;
	mutable glm::mat4	mMatrix;
};