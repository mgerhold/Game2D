#pragma once

#include "Component.h"
#include "Animation.h"
#include "ComponentWithSize.h"

class AnimationRenderer
	: public ComponentWithSize
{
public:
	Animation&		getAnimation();
	void			setAnimation(const Animation& animation);

private:
	virtual void	onUpdate(Time dt) override;
	virtual void	onDraw(const Window& window, RenderStates states) const override;
	glm::ivec2		determineSize() const override;

private:
	Animation		mAnimation;
};