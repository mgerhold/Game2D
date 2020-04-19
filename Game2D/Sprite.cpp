#include "Sprite.h"
#include <glm/vec2.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include "Utility.h"

Sprite::Sprite()
	: mVertexArray(PrimitiveType::Quads, DrawType::Dynamic)
	, mTexture(nullptr)
	, mVertexArrayNeedsUpdate(false)
	, mColor(Color(1.f, 1.f, 1.f, 1.f))
{}

void Sprite::setTexture(const Texture& texture) {
	SCOPED_PROFILER;
	mTexture = &texture;
	setDefaultTextureRect();
}

void Sprite::setDefaultTextureRect() {
	SCOPED_PROFILER;
	glm::ivec2 size = mTexture->getSize();
	setTextureRect(IntRect(0, 0, size.x, size.y));
}

void Sprite::setTextureRect(const IntRect& rect) {
	SCOPED_PROFILER;
	mTextureRect = rect;
	/*glm::ivec2 size = mTexture->getSize();
	if (mTextureRect.left < 0)
		mTextureRect.left = 0;
	if (mTextureRect.right > size.x)
		mTextureRect.right = size.x;
	if (mTextureRect.bottom < 0)
		mTextureRect.bottom = 0;
	if (mTextureRect.top > size.y)
		mTextureRect.top = size.y;*/
	mVertexArrayNeedsUpdate = true;
}

void Sprite::setTiling(int x, int y) {
	assert(x >= 1 && y >= 1);
	setTextureRect(IntRect(0, 0, x * getWidth(), y * getHeight()));
}

void Sprite::setColor(const Color& color) {
	SCOPED_PROFILER;
	mColor = color;
	mVertexArrayNeedsUpdate = true;
}

glm::ivec2 Sprite::getSize() const {
	SCOPED_PROFILER;
	//return mTexture->getSize();
	return glm::ivec2(mTextureRect.right - mTextureRect.left, mTextureRect.top - mTextureRect.bottom);
}

void Sprite::updateVertexArray() const {
	SCOPED_PROFILER;
	glm::ivec2 size = mTexture->getSize();
	float minU = static_cast<float>(mTextureRect.left) / static_cast<float>(size.x);
	float maxU = static_cast<float>(mTextureRect.right) / static_cast<float>(size.x);
	float minV = static_cast<float>(mTextureRect.bottom) / static_cast<float>(size.y);
	float maxV = static_cast<float>(mTextureRect.top) / static_cast<float>(size.y);
	mVertexArray.clear();
	mVertexArray.append(Vertex(glm::vec2(0.f, 0.f), mColor, glm::vec2(minU, minV)));
	mVertexArray.append(Vertex(glm::vec2(mTextureRect.right - mTextureRect.left, 0.f), mColor, glm::vec2(maxU, minV)));
	mVertexArray.append(Vertex(glm::vec2(mTextureRect.right - mTextureRect.left, mTextureRect.top - mTextureRect.bottom), mColor, glm::vec2(maxU, maxV)));
	mVertexArray.append(Vertex(glm::vec2(0.f, mTextureRect.top - mTextureRect.bottom), mColor, glm::vec2(minU, maxV)));
	mVertexArrayNeedsUpdate = false;
}

void Sprite::draw(const Window& window, RenderStates states) const {
	SCOPED_PROFILER;
	if (mVertexArrayNeedsUpdate)
		updateVertexArray();
	states.texture = mTexture;
	states.transform *= getTransform();
	window.draw(mVertexArray, states);
}
