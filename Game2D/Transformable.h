#pragma once

#include <glm/vec2.hpp>
#include "Transform.h"
#include "Rect.h"

class Transformable {
public:
						Transformable();
	void				setOrigin(glm::vec2 origin);
	void				setOrigin(float x, float y);
	glm::vec2			getOrigin() const;
	void				centerOrigin();
	void				setPosition(glm::vec2 position);
	void				setPosition(float x, float y);
	glm::vec2			getPosition() const;
	Transform			getTransform() const;
	void				setRotation(float radians);
	float				getRotation() const;
	void				setScale(glm::vec2 scale);
	void				setScale(float x, float y);
	glm::vec2			getScale() const;
	int					getWidth() const;
	int					getHeight() const;
	virtual glm::ivec2	getSize() const;
	FloatRect			getLocalBounds() const;
	virtual FloatRect	getWorldBounds() const;

private:
	void				recalculateTransform() const;

private:
	glm::vec2			mOrigin;
	glm::vec2			mPosition;
	float				mRotation; // in radians
	glm::vec2			mScale;
	mutable Transform	mTransform;
	mutable bool		mNeedsRecalculation;
};