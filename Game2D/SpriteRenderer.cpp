#include "SpriteRenderer.h"
#include <iostream>

Sprite& SpriteRenderer::getSprite() {
	return mSprite;
}

void SpriteRenderer::onDraw(const Window& window, RenderStates states) const {
	window.draw(mSprite, states);
}
