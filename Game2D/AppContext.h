#pragma once

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"

struct AppContext {
	TextureHolder& textureHolder;
	FontHolder& fontHolder;
	SoundBufferHolder& soundBufferHolder;
	const Window& window;

	AppContext(TextureHolder& textureHolder, FontHolder& fontHolder, SoundBufferHolder& soundBufferHolder, const Window& window)
		: textureHolder(textureHolder)
		, fontHolder(fontHolder)
		, soundBufferHolder(soundBufferHolder)
		, window(window)
	{}

};