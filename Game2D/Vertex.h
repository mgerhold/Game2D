#pragma once

#include <glm/vec2.hpp>
#include "Color.h"

struct Vertex {
	glm::vec2	position = glm::vec2(0.f);
	Color		color = Color(1.0f, 1.0f, 1.0f);
	glm::vec2	texCoords = glm::vec2(0.f);

	Vertex();
	Vertex(const glm::vec2& position);
	Vertex(const glm::vec2& position, const Color& color);
	Vertex(const glm::vec2& position, const glm::vec2& texCoords);
	Vertex(const glm::vec2& position, const Color& color, const glm::vec2& texCoords);
};