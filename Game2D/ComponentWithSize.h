#pragma once

#include <glm/vec2.hpp>
#include "Component.h"

class ComponentWithSize : public Component {
private:
	virtual glm::ivec2 determineSize() const = 0;

	friend class Entity;
};