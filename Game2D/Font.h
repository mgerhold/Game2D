#pragma once

#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <vector>
#include <map>
#include "Texture.h"
#include "Rect.h"
#include "Utility.h"

namespace {
	constexpr unsigned int CHARACTERS_PER_ATLAS = 256;
	constexpr unsigned int CHARACTERS_PER_ROW = 16; // sqrt(256)
	constexpr unsigned int BYTES_PER_PIXEL = 4; // RGBA (yes, that IS a waste)
}

class Font {
public:
	struct CharacterMetrics {
		unsigned int width = 0;
		unsigned int height = 0;
		unsigned int bearingX = 0;
		unsigned int bearingY = 0;
		unsigned int advance = 0;
	};

public:
											Font();
											Font(Font&& other) noexcept;
											~Font();
	bool									loadFromFile(const std::string& filename);
	size_t									getNumberOfGlyphs() const;
	const Texture&							getTexture(unsigned int fontHeight) const;
	CharacterMetrics						getCharacterMetrics(unsigned int fontHeight, unsigned int keycode) const;
	IntRect									getTextureRect(unsigned int fontHeight, unsigned int keycode) const;

private:
	struct CharacterAtlas {
		std::vector<CharacterMetrics> characterMetrics = std::vector<CharacterMetrics>(CHARACTERS_PER_ATLAS);
		Texture texture = Texture();
	};

private:
	void									generateCharacterAtlas(unsigned int fontHeight);

private:
	FT_Library								mFTLibrary;
	FT_Face									mFace;
	bool									mFaceLoaded;
	std::map<unsigned int, CharacterAtlas>	mCharacterAtlases;

	friend class Text;
};