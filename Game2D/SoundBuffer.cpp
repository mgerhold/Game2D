#include "SoundBuffer.h"

SoundBuffer::SoundBuffer()
	: mSystem(AudioSystem::getInstance()->getFMODSystem())
	, mSound(nullptr)
{}

SoundBuffer::~SoundBuffer() {
	if (mSound)
		mSound->release();
}

void SoundBuffer::loadFromFile(const std::string& filename) {
	if (mSound) {
		mSound->release();
		mSound = nullptr;
	}
	AudioSystem::checkFMODError(mSystem->createSound(filename.c_str(), FMOD_DEFAULT, nullptr, &mSound));
	AudioSystem::checkFMODError(mSound->setMode(FMOD_LOOP_OFF));
}
