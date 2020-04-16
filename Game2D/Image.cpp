#include "Image.h"

Image::Image()
	: mData(nullptr)
	, mWidth(0)
	, mHeight(0)
	, mNumberOfChannels(0)
{}

Image::~Image() {
	if (mData)
		stbi_image_free(mData);
	mData = nullptr;
}

bool Image::loadFromFile(const std::string& filename) {
	stbi_set_flip_vertically_on_load(true);
	mData = stbi_load(filename.c_str(), &mWidth, &mHeight, &mNumberOfChannels, 4);
	return mData != nullptr;
}
