#pragma once

#include "Component.h"
#include <glm/vec2.hpp>

class Entity;

class RigidBody : public Component {
public:
	void		setMass(float mass);
	float		getMass() const;
	void		accelerate(glm::vec2 acceleration);
	glm::vec2	getVelocity() const;
	void		setVelocity(glm::vec2 velocity);
	void		setGravity(glm::vec2 gravity);
	glm::vec2	getGravity() const;
	void		setDrag(float drag);
	float		getDrag() const;

private:
	void		onAwake() override;
	void		onUpdate(Time dt) override;

private:
	float		mMass = 10.f;
	float		mDrag = 0.1f;
	glm::vec2	mVelocity = glm::vec2(0.f);
	Entity*		mEntity = nullptr;
	glm::vec2	mGravity = glm::vec2(0.f, -98.1f);
};