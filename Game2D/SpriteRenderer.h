#pragma once

#include "ComponentWithSize.h"
#include "Sprite.h"

class SpriteRenderer : public ComponentWithSize {
public:
					SpriteRenderer(const Sprite& sprite);
	Sprite&			getSprite();

private:
	virtual void	onDraw(const Window& window, RenderStates states) const override;
	glm::ivec2		determineSize() const override;

private:
	Sprite			mSprite;
};