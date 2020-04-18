#pragma once

#include "AudioSystem.h"
#include "NonCopyable.h"
#include <string>

class SoundBuffer : private NonCopyable {
public:
					SoundBuffer();
					~SoundBuffer();

	void			loadFromFile(const std::string& filename);

private:
	FMOD::System*	mSystem;
	FMOD::Sound*	mSound;

	friend class Sound;
};