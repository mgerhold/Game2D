#pragma once

#include "Window.h"
#include "StateStack.h"
#include "ResourceIdentifiers.h"
#include "AppContext.h"

using namespace std::literals::string_literals;

#define APPLICATION_TITLE "Space Cat!"s

class Application {
public:
	Application();

	void					run();

private:
	void					registerStates();

private:
	// order of these members is important!
	Window					mWindow;
	TextureHolder			mTextureHolder;
	SoundBufferHolder		mSoundBufferHolder;
	FontHolder				mFontHolder;
	AppContext				mContext;
	constexpr static float	mUpdatesPerSecond = 120.f;
	StateStack				mStateStack;
	Shader					mShader;
	RenderStates			mRenderStates;
};