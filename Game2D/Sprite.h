#pragma once

#include <memory>
#include "Drawable.h"
#include "Transformable.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Rect.h"
#include "Color.h"

class Sprite
	: public Drawable
	, public Transformable
{
public:
												Sprite();
	void										setTexture(const Texture& texture);
	void										setDefaultTextureRect();
	void										setTextureRect(const IntRect& rect);
	void										setTiling(int x, int y);
	void										setColor(const Color& color);
	virtual glm::ivec2							getSize() const override;

private:
	void										updateVertexArray() const;
private:
	virtual void								draw(const Window& window, RenderStates states) const override;
	const Texture*								mTexture;
	IntRect										mTextureRect;
	mutable std::shared_ptr<VertexArray>		mVertexArray;
	mutable bool								mVertexArrayNeedsUpdate;
	Color										mColor;
};