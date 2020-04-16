#pragma once

#include <stb_image/stb_image.h>
#include <string>

class Image {
public:
					Image();
					~Image();

	bool			loadFromFile(const std::string& filename);
private:
	unsigned char*	mData;
	int				mWidth;
	int				mHeight;
	int				mNumberOfChannels;

	friend class Texture;
};