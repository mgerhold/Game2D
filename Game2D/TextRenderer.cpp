#include "TextRenderer.h"

Text& TextRenderer::getText() {
	return mText;
}

void TextRenderer::onDraw(const Window& window, RenderStates states) const {
	window.draw(mText, states);
}
