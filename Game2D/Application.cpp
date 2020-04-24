#include "Application.h"
#include "RenderStates.h"
#include "Clock.h"
#include "TitleState.h"
#include "MainMenuState.h"
#include <iostream>

Application::Application()
	: mWindow(
		1024,
		768,
		APPLICATION_TITLE,
		4,		// multisampling
		false,	// vertical sync
		true,	// resizeable
		true,	// decorated
		false	// center cursor
	)
	, mContext(mTextureHolder, mFontHolder, mSoundBufferHolder, mWindow)
	, mStateStack(mContext) {

	mWindow.setClearColor(Color(0.4f, 0.4f, 0.4f, 1.0f));
	mWindow.setKeyRepeat(false);

	mShader.loadFromFile("shaders/default.vert", Shader::Type::Vertex);
	mShader.loadFromFile("shaders/default.frag", Shader::Type::Fragment);

	mRenderStates.shader = &mShader;
	mRenderStates.transform = Transform::Identity;

	mFontHolder.load(FontID::Default, "fonts/arial.ttf");

	registerStates();
	mStateStack.push(StateID::Title);
}

void Application::run() {
	// main loop
	Clock clock;
	Clock fpsClock;
	Time lastElapsed = clock.getElapsedTime();
	Time currentTime;
	int updateFrames = 0;
	int renderFrames = 0;

	while (!mWindow.shouldClose()) {
		while ((currentTime = clock.getElapsedTime()) >= lastElapsed + Time::seconds(1.f) / mUpdatesPerSecond) {
			Time dt = currentTime - lastElapsed;
			lastElapsed = currentTime;
			updateFrames++;

			// update
			mStateStack.update(dt);			

			// process events
			Window::processEvents();
			while (mWindow.hasEvent()) {
				Event e = mWindow.pollEvent();

				mStateStack.handleEvent(e);
			}
		}

		// render
		mWindow.setClearColor(Color(0.3f, 0.2f, 0.7f));
		mWindow.clear();
		mStateStack.draw(mWindow);
		mWindow.swapBuffers();
		renderFrames++;

		// exit on empty stack
		if (mStateStack.isEmpty())
			mWindow.setWindowShouldClose(true);

		if (fpsClock.getElapsedTime() >= Time::seconds(1.f)) {
			Time fpsClockElapsed = fpsClock.getElapsedTime();
			fpsClock.setElapsedTime(fpsClock.getElapsedTime() - Time::seconds(1.f));
			float fps = renderFrames == 0 ? 0.f : renderFrames / fpsClockElapsed.asSeconds();
			float ups = updateFrames == 0 ? 0.f : updateFrames / fpsClockElapsed.asSeconds();
			mWindow.setTitle(
				APPLICATION_TITLE
				+ " ("s+ std::to_string(fps)
				+ " fps, delta = "s
				+ std::to_string(fpsClockElapsed.asSeconds() * 1000.f / renderFrames)
				+ " us | "s
				+ std::to_string(ups) + " ups, delta = "s
				+ std::to_string(fpsClockElapsed.asSeconds() * 1000.f / updateFrames)
				+ " us)"s
			);
			renderFrames = updateFrames = 0;
		}
	}
}

void Application::registerStates() {
	mStateStack.registerState<TitleState>(StateID::Title);
	mStateStack.registerState<MainMenuState>(StateID::MainMenu);
}