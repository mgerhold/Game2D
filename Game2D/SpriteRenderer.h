#pragma once

#include "Component.h"
#include "Sprite.h"

class SpriteRenderer : public Component {
public:
	Sprite&			getSprite();

private:
	virtual void	onDraw(const Window& window, RenderStates states) const override;

private:
	Sprite			mSprite;
};