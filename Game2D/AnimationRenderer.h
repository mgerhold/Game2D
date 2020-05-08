#pragma once

#include "Component.h"
#include "Animation.h"

class AnimationRenderer : public Component {
public:
	Animation&		getAnimation();
	void			setAnimation(const Animation& animation);

private:
	virtual void	onUpdate(Time dt) override;
	virtual void	onDraw(const Window& window, RenderStates states) const override;

private:
	Animation		mAnimation;
};