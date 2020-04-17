#pragma once

#include "AudioSystem.h"
#include <string>

class Music {
public:
	Music();
	~Music();

	void			openFromFile(const std::string& filename);
	void			play(int loopCount = 0);
	void			stop();
	void			setPaused(bool paused);
	bool			getPaused() const;
	void			setVolume(float volume = 1.f);
	float			getVolume() const;
	bool			isPlaying() const;
private:
	FMOD::System*	mSystem;
	FMOD::Sound*	mSound;
	FMOD::Channel*	mChannel;
	bool			mOpened;
	float			mVolume;
};