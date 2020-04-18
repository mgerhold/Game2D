#pragma once

#include "AudioSystem.h"
#include "NonCopyable.h"
#include <string>

class SoundBuffer : private NonCopyable {
public:
					SoundBuffer() noexcept;
					~SoundBuffer();
					SoundBuffer(SoundBuffer&& other) noexcept;

	bool			loadFromFile(const std::string& filename);

private:
	FMOD::System*	mSystem;
	FMOD::Sound*	mSound;

	friend class Sound;
};