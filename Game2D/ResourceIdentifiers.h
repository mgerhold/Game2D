#pragma once

#include "ResourceHolder.h"
#include "Texture.h"
#include "Font.h"
#include "SoundBuffer.h"

enum class TextureID {
	FMODLogo,
	ButtonNormal,
	ButtonSelected,
	ButtonActive,
	Tileset,
	ArrowLeft1,
	ArrowLeft2,
	ArrowRight1,
	ArrowRight2,
	Button2Normal,
	Button2Selected,
	Button2Active,
	PlayerIdle,
	PlayerIdleReversed,
	PlayerRun,
	PlayerRunReversed,
	PlayerJump,
	PlayerJumpReversed,
	Background,
	HourglassRunning,
	HourglassTurning,
	MainMenuBackground,
	Logo,
};

enum class SoundID {
	HourglassFX,
	PlayerJump,
	PlayerFall,
};

enum class FontID {
	Default,
};

using TextureHolder = ResourceHolder<TextureID, Texture>;
using SoundBufferHolder = ResourceHolder<SoundID, SoundBuffer>;
using FontHolder = ResourceHolder<FontID, Font>;