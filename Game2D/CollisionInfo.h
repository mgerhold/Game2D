#pragma once

#include "Rect.h"

class Entity;

struct CollisionInfo {
	FloatRect intersection;
	glm::vec2 penetration = glm::vec2(0.f);
	FloatRect thisBounds;
	FloatRect otherBounds;
	Entity* other = nullptr;
};