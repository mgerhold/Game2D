#pragma once

#include "SoundBuffer.h"

class Sound /*: private NonCopyable*/ {
public:
	Sound() noexcept;
	//Sound(Sound&& other) noexcept;
	~Sound();

	//Sound& operator=(Sound&& other) noexcept;

	void	setSoundBuffer(const SoundBuffer& soundBuffer);
	void	play();
	void	stop();
	void	setPaused(bool paused);
	bool	getPaused() const;
	void	setVolume(float volume);
	float	getVolume() const;
	bool	isPlaying() const;

private:
	const SoundBuffer* mSoundBuffer;
	FMOD::System*	mSystem;
	FMOD::Channel* mChannel;
	float	mVolume;
};