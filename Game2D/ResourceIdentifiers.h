#pragma once

#include "ResourceHolder.h"
#include "Texture.h"
#include "Font.h"
#include "SoundBuffer.h"

enum class TextureID {
	Eagle,
	Raptor,
	FMODLogo,
	Checkerboard,
	Explosion,
	Guybrush,
	ButtonNormal,
	ButtonSelected,
	ButtonActive,
	Tilemap,
	TileSelection,
	ArrowLeft1,
	ArrowLeft2,
	ArrowRight1,
	ArrowRight2,
};

enum class SoundID {
	Kick,
	Snare,
	Hats,
	Crash,
	Crow,
};

enum class FontID {
	Default,
};

using TextureHolder = ResourceHolder<TextureID, Texture>;
using SoundBufferHolder = ResourceHolder<SoundID, SoundBuffer>;
using FontHolder = ResourceHolder<FontID, Font>;