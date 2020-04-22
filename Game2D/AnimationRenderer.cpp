#include "AnimationRenderer.h"

Animation& AnimationRenderer::getAnimation() {
	return mAnimation;
}



void AnimationRenderer::onUpdate(Time dt) {
	mAnimation.update();
}

void AnimationRenderer::onDraw(const Window& window, RenderStates states) const {
	window.draw(mAnimation, states);
}
