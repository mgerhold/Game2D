#include "Texture.h"
#include "Utility.h"

Texture::Texture()
	: mTexture(0)
	, mLoaded(false)
	, mWidth(0)
	, mHeight(0)
{}

Texture::Texture(Texture && other) noexcept
	: mTexture(other.mTexture)
	, mLoaded(other.mLoaded)
	, mWidth(other.mWidth)
	, mHeight(other.mHeight)
{
	other.mTexture = 0;
	other.mLoaded = false;
	other.mWidth = 0;
	other.mHeight = 0;
}

Texture::~Texture() {
	if (mLoaded) {
		GLCALL(glDeleteTextures(1, &mTexture));
	}
	mLoaded = false;
}

bool Texture::loadFromFile(const std::string& filename) {
	Image image;
	if (!image.loadFromFile(filename))
		return false;
	if (!loadFromImage(image))
		return false;
	return true;
}

bool Texture::loadFromImage(const Image& image) {
	mLoaded = true;
	GLCALL(glGenTextures(1, &mTexture));
	bind();
	GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.mWidth, image.mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.mData));
	GLCALL(glGenerateMipmap(GL_TEXTURE_2D));
	setTextureFiltering(Filtering::Linear);
	setTextureWrap(false);
	mWidth = image.mWidth;
	mHeight = image.mHeight;
	return true;
}

bool Texture::loadFromMemory(int width, int height, const unsigned char* data) {
	mLoaded = true;
	mWidth = width;
	mHeight = height;
	GLCALL(glGenTextures(1, &mTexture));
	bind();
	GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
	GLCALL(glGenerateMipmap(GL_TEXTURE_2D));
	setTextureFiltering(Filtering::Linear);
	setTextureWrap(false);
	return true;
}

void Texture::bind() const {
	if (mLoaded)
		GLCALL(glBindTexture(GL_TEXTURE_2D, mTexture));
}

void Texture::setTextureFiltering(Filtering filtering) const {
	if (mLoaded) {
		bind();
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering == Filtering::Linear ? GL_LINEAR : GL_NEAREST));
	}
}

void Texture::setTextureWrap(bool wrap) {
	if (mLoaded) {
		bind();
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP_TO_EDGE));
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP_TO_EDGE));
	}
}

glm::ivec2 Texture::getSize() const {
	return glm::ivec2(mWidth, mHeight);
}

void Texture::unbind() {
	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}