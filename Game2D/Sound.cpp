#include "Sound.h"
#include <iostream>

Sound::Sound() noexcept
	: mSoundBuffer(nullptr)
	, mChannel(nullptr)
	, mVolume(1.f)
	, mSystem(AudioSystem::getInstance()->getFMODSystem())
{}

/*Sound::Sound(Sound && other) noexcept
	: mSoundBuffer(other.mSoundBuffer)
	, mChannel(other.mChannel)
	, mVolume(other.mVolume)
	, mSystem(other.mSystem)
{}

Sound& Sound::operator=(Sound&& other) noexcept {
	this->mSoundBuffer = other.mSoundBuffer;
	this->mChannel = other.mChannel;
	this->mVolume = other.mVolume;
	this->mSystem = other.mSystem;
	return *this;
}*/

Sound::~Sound() {
	if (isPlaying())
		stop();
}

void Sound::setSoundBuffer(const SoundBuffer& soundBuffer) {
	if (isPlaying())
		stop();
	mSoundBuffer = &soundBuffer;
}

void Sound::play() {
	if (!mSoundBuffer) {
		std::cout << "Warning: Cannot play sound since no SoundBuffer has been attached.\n";
		return;
	}
	if (!mSoundBuffer->mSound) {
		std::cout << "Warning: Cannot play sound since the attached SoundBuffer has no valid sound loaded.\n";
		return;
	}
	if (isPlaying())
		stop();
	AudioSystem::checkFMODError(mSystem->playSound(mSoundBuffer->mSound, nullptr, false, &mChannel));
	setVolume(mVolume);
}

void Sound::stop() {
	if (mChannel)
		AudioSystem::checkFMODError(mChannel->stop());
}

void Sound::setPaused(bool paused) {
	if (mChannel) {
		AudioSystem::checkFMODError(mChannel->setPaused(paused));
	}
}

bool Sound::getPaused() const {
	bool result;
	if (mChannel) {
		AudioSystem::checkFMODError(mChannel->getPaused(&result));
		return result;
	}
	std::cout << "Warning: Trying to get pause state while channel is invalid.\n";
	return false;
}

void Sound::setVolume(float volume) {
	mVolume = volume;
	if (isPlaying())
		AudioSystem::checkFMODError(mChannel->setVolume(mVolume));
}

float Sound::getVolume() const {
	return mVolume;
}

bool Sound::isPlaying() const {
	if (mChannel) {
		bool result;
		FMOD_RESULT FMODResult = mChannel->isPlaying(&result);
		if (FMODResult != FMOD_OK)
			return false;
		return result;
	}
	return false;
}
