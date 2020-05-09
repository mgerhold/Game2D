#include "SpriteRenderer.h"
#include <iostream>

SpriteRenderer::SpriteRenderer(const Sprite& sprite)
	: mSprite(sprite)
{}

Sprite& SpriteRenderer::getSprite() {
	return mSprite;
}

void SpriteRenderer::onDraw(const Window& window, RenderStates states) const {
	window.draw(mSprite, states);
}

glm::ivec2 SpriteRenderer::determineSize() const {
	return mSprite.getSize();
}
