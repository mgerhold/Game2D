#include "Vertex.h"

Vertex::Vertex()
	: Vertex(glm::vec2(0.f), Color(1.f, 1.f, 1.f), glm::vec2(0.f))
{}

Vertex::Vertex(const glm::vec2& position)
	: Vertex(position, Color(1.f, 1.f, 1.f), glm::vec2(0.f))
{}

Vertex::Vertex(const glm::vec2& position, const Color& color)
	: Vertex(position, color, glm::vec2(0.f))
{}

Vertex::Vertex(const glm::vec2& position, const glm::vec2& texCoords)
	: Vertex(position, Color(1.f, 1.f, 1.f), texCoords)
{}

Vertex::Vertex(const glm::vec2& position, const Color& color, const glm::vec2& texCoords)
	: position(position)
	, color(color)
	, texCoords(texCoords)
{}
