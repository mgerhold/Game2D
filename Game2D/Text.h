#pragma once

#include "Font.h"
#include "VertexArray.h"
#include "Drawable.h"
#include "Transformable.h"
#include "Color.h"
#include "Rect.h"

class Text
	: public Drawable
	, public Transformable
{
public:
						Text();
	void				setFont(Font& font, unsigned int fontHeight);
	void				setString(const std::string& string);
	FloatRect			getLocalBounds() const;
	virtual glm::ivec2	getSize() const override;
	void				setColor(Color color);

private:
	void				updateVertexArray();
	virtual void		draw(const Window& window, RenderStates states) const;

private:
	Font*				mFont;
	VertexArray			mVertexArray;
	std::string			mString;
	unsigned int		mFontHeight;
	FloatRect			mBounds;
	glm::ivec2			mSize;
	Color				mColor;
	const Texture*		mTexture;
};