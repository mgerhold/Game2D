#include "AudioSystem.h"
#include <Windows.h>
#include <string>
#include <fmod/fmod_errors.h>
#include <stdexcept>
#include <iostream>
#include "Sound.h"

std::unique_ptr<AudioSystem> AudioSystem::sInstance = nullptr;
std::list<Sound> AudioSystem::sSoundList = std::list<Sound>();

AudioSystem* AudioSystem::getInstance() {
	if (!sInstance)
		sInstance = std::unique_ptr<AudioSystem>(new AudioSystem); // "new" keyword because of private constructor
	return sInstance.get();
}

FMOD::System* AudioSystem::getFMODSystem() const {
	return mFMODSystem;
}

void AudioSystem::update() {
	// update step for FMOD
	FMOD::System* system = nullptr;
	if (sInstance)
		system = sInstance->getFMODSystem();
	if (system)
		system->update();

	// remove sounds that are not playing anymore
	sSoundList.remove_if([](const Sound& sound) -> bool {
		return !sound.isPlaying();
	});
}

Sound& AudioSystem::playSound(Sound sound) {
	sSoundList.push_back(sound);
	sSoundList.back().play();
	return sSoundList.back();
}

/*
	private methods
*/
AudioSystem::AudioSystem() {
	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

	//FMOD_RESULT result;
	checkFMODError(FMOD::System_Create(&mFMODSystem));

	unsigned int version;
	checkFMODError(mFMODSystem->getVersion(&version));
	if (version < FMOD_VERSION) {
		throw std::runtime_error("FMOD lib version doesn't match header version!");
	}

	//checkFMODError(mFMODSystem->setDSPBufferSize(512, 4));
	checkFMODError(mFMODSystem->init(512, FMOD_INIT_NORMAL, nullptr));

	//std::cout << "Audio system initialized!\n";
}

AudioSystem::~AudioSystem() {
	mFMODSystem->close();
	mFMODSystem->release();

	CoUninitialize();

	//std::cout << "Audio system has been shut down!\n";
}

void AudioSystem::checkFMODError(FMOD_RESULT result) {
	if (result != FMOD_OK) {
		const std::string msg = "FMOD error " + std::to_string(result) + ": " + FMOD_ErrorString(result);
		throw std::runtime_error(msg);
	}
}
