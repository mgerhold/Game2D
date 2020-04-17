#include "Font.h"
#include <stdexcept>
#include <iostream>
#include <memory>

Font::Font()
	: mFaceLoaded(false)
	, mFTLibrary()
	, mFace()
{
	SCOPED_PROFILER;
	FT_Error error = FT_Init_FreeType(&mFTLibrary);
	if (error) {
		throw std::runtime_error("Could not create Freetype Library Object!");
	}
}

Font::~Font() {
	SCOPED_PROFILER;
	if (mFaceLoaded)
		FT_Done_Face(mFace);
	FT_Done_FreeType(mFTLibrary);
}

bool Font::loadFromFile(const std::string& filename) {
	SCOPED_PROFILER;
	FT_Error error = FT_New_Face(mFTLibrary, filename.c_str(), 0, &mFace);
	if (error == FT_Err_Unknown_File_Format) {
		std::cout << "Freetype Error: Unknown File Format! (" << filename << ")\n";
		return false;
	} else if (error) {
		std::cout << "Freetype Error: Error while loading file " << filename << "\n";
		return false;
	}

	mFaceLoaded = true;
	return true;
}

size_t Font::getNumberOfGlyphs() const {
	SCOPED_PROFILER;
	return mFaceLoaded ? mFace->num_glyphs : 0;
}

const Texture& Font::getTexture(unsigned int fontHeight) const {
	SCOPED_PROFILER;
	auto find = mCharacterAtlases.find(fontHeight);
	if (find == mCharacterAtlases.end())
		throw std::runtime_error("Could not find texture atlas for font specified font height.");
	return find->second.texture;
}

Font::CharacterMetrics Font::getCharacterMetrics(unsigned int fontHeight, unsigned int keycode) const {
	SCOPED_PROFILER;
	auto find = mCharacterAtlases.find(fontHeight);
	if (find == mCharacterAtlases.end())
		throw std::runtime_error("Could not return character metrics - no texture atlas for this font height found!");
	if (find->second.characterMetrics.size() < keycode)
		throw std::runtime_error("This font does not contain that many characters!");
	return find->second.characterMetrics[keycode];
}

IntRect Font::getTextureRect(unsigned int fontHeight, unsigned int keycode) const {
	SCOPED_PROFILER;
	auto find = mCharacterAtlases.find(fontHeight);
	if (find == mCharacterAtlases.end())
		throw std::runtime_error("Could not get texture rect - no texture atlas for this font height found!");
	if (find->second.characterMetrics.size() < keycode)
		throw std::runtime_error("This font does not contain that many characters!");
	const auto metrics = find->second.characterMetrics[keycode];

	IntRect result;
	unsigned int x = keycode % CHARACTERS_PER_ROW;
	unsigned int y = keycode / (CHARACTERS_PER_ATLAS / CHARACTERS_PER_ROW);
	unsigned int characterWidth = static_cast<unsigned int>(find->second.texture.getSize().x) / CHARACTERS_PER_ROW;
	unsigned int characterHeight = static_cast<unsigned int>(find->second.texture.getSize().y) / (CHARACTERS_PER_ATLAS / CHARACTERS_PER_ROW);
	unsigned texX = characterWidth * x;
	unsigned texY = characterHeight * y;
	result.left = texX;
	result.bottom = texY;
	result.right = texX + metrics.width;
	result.top = texY + metrics.height;
	return result;
}

void Font::generateCharacterAtlas(unsigned int fontHeight) {
	SCOPED_PROFILER;
	if (!mFaceLoaded) {
		std::cout << "Error: Cannot generate character atlas since no font face has been loaded\n";
		return;
	}

	FT_Set_Pixel_Sizes(mFace, 0, fontHeight);
	// find maximum dimensions for character
	unsigned int maxCharWidth = 0;
	unsigned int maxCharHeight = 0;
	for (unsigned long i = 0; i < CHARACTERS_PER_ATLAS; i++) {
		if (FT_Load_Char(mFace, i, FT_LOAD_BITMAP_METRICS_ONLY/*FT_LOAD_RENDER*/)) {
			std::cout << "Error: Could not load character '" << static_cast<char>(i) << "'\n";			
			continue;
		}
		if (mFace->glyph->bitmap.width > maxCharWidth)
			maxCharWidth = mFace->glyph->bitmap.width;
		if (mFace->glyph->bitmap.rows > maxCharHeight)
			maxCharHeight = mFace->glyph->bitmap.rows;
	}
	//std::cout << "Maximum dimensions: " << maxCharWidth << ", " << maxCharHeight << "\n";

	// generate texture buffer and character atlas
	CharacterAtlas atlas; // automatically initializes characterMetrics vector to needed size
	unsigned int textureWidth = maxCharWidth * CHARACTERS_PER_ROW;
	unsigned int textureHeight = maxCharHeight * CHARACTERS_PER_ATLAS / CHARACTERS_PER_ROW;
	const unsigned long long dataSize = static_cast<unsigned long long>(textureWidth)
		* static_cast<unsigned long long>(textureHeight)
		* static_cast<unsigned long long>(BYTES_PER_PIXEL);
	auto buffer = std::make_unique<unsigned char[]>(dataSize);

	// clear whole buffer to Color(0, 0, 0, 0)
	memset(buffer.get(), 0, dataSize);
	for (unsigned long i = 0; i < CHARACTERS_PER_ATLAS; i++) {
		if (FT_Load_Char(mFace, i, FT_LOAD_RENDER)) {
			std::cout << "Error: Could not load bitmap for character '" << (isprint(i) ? static_cast<char>(i) : ' ') << "'\n";
			continue;
		}
		// read character metrics
		const unsigned int glyphWidth = mFace->glyph->bitmap.width;
		const unsigned int glyphHeight = mFace->glyph->bitmap.rows;
		atlas.characterMetrics[i].width = glyphWidth;
		atlas.characterMetrics[i].height = glyphHeight;
		atlas.characterMetrics[i].bearingX = mFace->glyph->bitmap_left;
		atlas.characterMetrics[i].bearingY = mFace->glyph->bitmap_top;
		atlas.characterMetrics[i].advance = mFace->glyph->advance.x;
		//std::cout << "Dimensions for character " << i << "('" << (isprint(i) ? static_cast<char>(i) : ' ') << "'): " << atlas.characterMetrics[i].width << ", " << atlas.characterMetrics[i].height << "\n";
		const unsigned int originX = (i % CHARACTERS_PER_ROW) * maxCharWidth;
		const unsigned int originY = (i / CHARACTERS_PER_ROW) * maxCharHeight;
		for (unsigned int x = 0; x < glyphWidth; x++) {
			for (unsigned int y = 0; y < glyphHeight; y++) {
				unsigned long long position =
					(static_cast<unsigned long long>(originX) + static_cast<unsigned long long>(x)
						+ (static_cast<unsigned long long>(originY)
						+ static_cast<unsigned long long>(y)) * static_cast<unsigned long long>(textureWidth)
					) * 4ull;
				buffer[position] = 255;
				buffer[position + 1] = 255;
				buffer[position + 2] = 255;
				buffer[position + 3] = mFace->glyph->bitmap.buffer[x + (glyphHeight - y - 1) * glyphWidth];
			}
		}
	}
	atlas.texture.loadFromMemory(textureWidth, textureHeight, buffer.get());
	mCharacterAtlases.emplace(std::make_pair(fontHeight, std::move(atlas)));
}
