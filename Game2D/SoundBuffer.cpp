#include "SoundBuffer.h"
#include <iostream>

SoundBuffer::SoundBuffer() noexcept
	: mSystem(AudioSystem::getInstance()->getFMODSystem())
	, mSound(nullptr)
{}

SoundBuffer::~SoundBuffer() {
	if (mSound)
		mSound->release();
}

SoundBuffer::SoundBuffer(SoundBuffer&& other) noexcept
	: mSystem(other.mSystem)
	, mSound(other.mSound)
{
	other.mSound = nullptr;
}

bool SoundBuffer::loadFromFile(const std::string& filename) {
	if (mSound) {
		mSound->release();
		mSound = nullptr;
	}
	try {
		AudioSystem::checkFMODError(mSystem->createSound(filename.c_str(), FMOD_DEFAULT, nullptr, &mSound));
		AudioSystem::checkFMODError(mSound->setMode(FMOD_LOOP_OFF));
		return true;
	} catch (std::exception& e) {
		std::cout << "Error loading sound buffer from file " << filename << ": " << e.what() << "\n";
		return false;
	}
}
