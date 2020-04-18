#pragma once

#include <fmod/fmod.hpp>
#include <memory>
#include <list>

class Sound;

class AudioSystem {
public:
										~AudioSystem();

	static AudioSystem*					getInstance();
	FMOD::System*						getFMODSystem() const;

	static void							checkFMODError(FMOD_RESULT result);
	static void							update();
	static Sound&						playSound(Sound sound);
private:
										AudioSystem();
private:
	FMOD::System*						mFMODSystem;

	static std::unique_ptr<AudioSystem>	sInstance;
	static std::list<Sound>				sSoundList;
};