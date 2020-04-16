#include "Text.h"
#include <iostream>
#include "Utility.h"

Text::Text()
	: mFont(nullptr)
	, mVertexArray(PrimitiveType::Quads, DrawType::Dynamic)
	, mString("")
	, mFontHeight(48)
	, mBounds()
	, mSize()
	, mColor(Color(1.f, 1.f, 1.f, 1.f))
	, mTexture(nullptr)
{}

void Text::setFont(Font& font, unsigned int fontHeight) {
	SCOPED_PROFILER;
	mFontHeight = fontHeight;
	mFont = &font;
	mFont->generateCharacterAtlas(fontHeight);
	mTexture = &mFont->getTexture(fontHeight);
	updateVertexArray();
}

void Text::setString(const std::string& string) {
	SCOPED_PROFILER;
	mString = string;
	updateVertexArray();
}

FloatRect Text::getLocalBounds() const {
	return mBounds;
}

glm::ivec2 Text::getSize() const {
	return mSize;
}

void Text::setColor(Color color) {
	mColor = color;
	updateVertexArray();
}

void Text::updateVertexArray() {
	SCOPED_PROFILER;
	if (!mFont)
		return;

	mVertexArray.clear();
	if (mString == "")
		return;

	//Color color = Color(1.f, 1.f, 1.f, 1.f);
	float xOffset = 0.f;
	float maxBearingY = 0.f;
	float minY = 0.f;
	int maxHeight = 0;
	//float lastBearingX = 0.f;
	//const float firstBearingX = mFont->getCharacterMetrics(mFontHeight, static_cast<unsigned int>(mString[0])).bearingX;
	const auto texSize = mFont->getTexture(mFontHeight).getSize();
	for (const auto& character : mString) {
		//std::cout << "Updating vertex array for character " << character << "\n";
		const auto metrics = mFont->getCharacterMetrics(mFontHeight, static_cast<unsigned int>(character));
		const auto texRect = mFont->getTextureRect(mFontHeight, static_cast<unsigned int>(character));
		/*std::cout << "\twidth: " << metrics.width << ", height: " << metrics.height << ", advance: " << static_cast<float>(metrics.advance) / 64.f << "\n";
		std::cout << "\tbearingY: " << metrics.bearingY << "\n";
		std::cout << "\t(texRect)left: " << texRect.left << ", bottom: " << texRect.bottom << ", right: " << texRect.right << ", top: " << texRect.top << "\n";*/
		mVertexArray.append(
			Vertex(
				glm::vec2(xOffset + metrics.bearingX, static_cast<float>(metrics.bearingY) - static_cast<float>(metrics.height)),
				mColor,
				glm::vec2(static_cast<float>(texRect.left) / texSize.x, static_cast<float>(texRect.bottom) / texSize.y)
			));
		mVertexArray.append(
			Vertex(
				glm::vec2(xOffset + metrics.bearingX + metrics.width, static_cast<float>(metrics.bearingY) - static_cast<float>(metrics.height)),
				mColor,
				glm::vec2(static_cast<float>(texRect.right) / texSize.x,	static_cast<float>(texRect.bottom) / texSize.y)
			));
		mVertexArray.append(
			Vertex(
				glm::vec2(xOffset + metrics.bearingX + metrics.width, static_cast<float>(metrics.bearingY)),
				mColor,
				glm::vec2(static_cast<float>(texRect.right) / texSize.x, static_cast<float>(texRect.top) / texSize.y)
			));
		mVertexArray.append(
			Vertex(
				glm::vec2(xOffset + metrics.bearingX, static_cast<float>(metrics.bearingY)),
				mColor,
				glm::vec2(static_cast<float>(texRect.left) / texSize.x,	static_cast<float>(texRect.top) / texSize.y)
			));
		xOffset += static_cast<float>(metrics.advance) / 64.f;
		if (metrics.bearingY > maxBearingY)
			maxBearingY = static_cast<float>(metrics.bearingY);
		if (static_cast<float>(metrics.bearingY) - static_cast<float>(metrics.height) < minY)
			minY = static_cast<float>(metrics.bearingY) - static_cast<float>(metrics.height);
		if (static_cast<int>(metrics.height) > maxHeight)
			maxHeight = static_cast<int>(metrics.height);
		//lastBearingX = static_cast<float>(metrics.bearingX);
	}
	mBounds.left = 0.f;
	mBounds.right = xOffset;
	mBounds.bottom = minY;
	mBounds.top = maxBearingY;
	mSize = glm::ivec2(static_cast<int>(xOffset), maxHeight);
}

void Text::draw(const Window& window, RenderStates states) const {
	SCOPED_PROFILER;
	if (!mFont)
		return;
	states.transform *= getTransform();
	states.texture = mTexture;
	window.draw(mVertexArray, states);
}
