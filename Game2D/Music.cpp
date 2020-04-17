#include "Music.h"
#include <iostream>

Music::Music()
	: mOpened(false)
	, mSound(nullptr)
	, mChannel(nullptr)
	, mSystem(AudioSystem::getInstance()->getFMODSystem())
	, mVolume(1.f)
{}

Music::~Music() {
	if (isPlaying())
		stop();
	if (mSound)
		mSound->release();
}

void Music::openFromFile(const std::string& filename) {
	if (mOpened) {
		if (isPlaying())
			stop();
		if (mSound)
			mSound->release();
		mSound = nullptr;
	}
	AudioSystem::checkFMODError(mSystem->createSound(filename.c_str(), FMOD_CREATESTREAM, nullptr, &mSound));
	mOpened = true;
}

void Music::play(int loopCount) {
	if (isPlaying()) {
		std::cout << "Music is already playing.\n";
		return;
	}

	if (mOpened) {
		if (loopCount < -1)
			loopCount = -1;
		AudioSystem::checkFMODError(mSound->setMode(loopCount ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF));
		if (loopCount)
			AudioSystem::checkFMODError(mSound->setLoopCount(loopCount));

		AudioSystem::checkFMODError(mSystem->playSound(mSound, nullptr, false, &mChannel));
		AudioSystem::checkFMODError(mChannel->setVolume(mVolume));
	} else {
		std::cout << "Cannot play music since no file has been opened.\n";
	}
}

void Music::stop() {
	if (isPlaying())
		AudioSystem::checkFMODError(mChannel->stop());
}

void Music::setPaused(bool paused) {
	if (mOpened && mChannel)
		AudioSystem::checkFMODError(mChannel->setPaused(paused));
}

bool Music::getPaused() const {
	bool result;
	if (mOpened && mChannel) {
		AudioSystem::checkFMODError(mChannel->getPaused(&result));
		return result;
	}
	std::cout << "Warning: Trying to get pause state while no music has been loaded or no channel is valid!\n";
	return false;
}

void Music::setVolume(float volume) {
	mVolume = volume;
	if (mOpened && mChannel) {
		AudioSystem::checkFMODError(mChannel->setVolume(volume));
	}
}

float Music::getVolume() const {
	return mVolume;
}

bool Music::isPlaying() const {
	if (!mOpened || !mChannel)
		return false;
	FMOD_RESULT FMODResult;
	bool playing;	
	FMODResult = mChannel->isPlaying(&playing);
	if (FMODResult == FMOD_ERR_INVALID_HANDLE)
		return false;
	else
		AudioSystem::checkFMODError(FMODResult);
	return playing;
}
