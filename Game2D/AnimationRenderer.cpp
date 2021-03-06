#include "AnimationRenderer.h"

Animation& AnimationRenderer::getAnimation() {
	return mAnimation;
}

void AnimationRenderer::setAnimation(const Animation& animation) {
	mAnimation = animation;
}



void AnimationRenderer::onUpdate(Time dt) {
	mAnimation.update();
}

void AnimationRenderer::onDraw(const Window& window, RenderStates states) const {
	window.draw(mAnimation, states);
}

glm::ivec2 AnimationRenderer::determineSize() const {
	return mAnimation.getSize();
}
