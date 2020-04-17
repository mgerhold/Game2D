#pragma once

#include <fmod/fmod.hpp>
#include <memory>

class AudioSystem {
public:
										~AudioSystem();

	static AudioSystem*					getInstance();
	FMOD::System*						getFMODSystem() const;

	static void							checkFMODError(FMOD_RESULT result);
	static void							update();
private:
										AudioSystem();
private:
	static std::unique_ptr<AudioSystem>	sInstance;
	FMOD::System*						mFMODSystem;
};