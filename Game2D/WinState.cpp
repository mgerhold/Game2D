#include "WinState.h"

WinState::WinState(StateStack* stateStack)
	: State(stateStack)
{
	mBackground.setTexture(getContext().textureHolder.get(TextureID::MainMenuBackground));
	mBackground.setTiling(20, 20);
	mBackground.centerOrigin();

	mText1.setFont(getContext().fontHolder.get(FontID::Default), 40);
	mText1.setString("Congratulations, you've won the game!");
	mText1.centerOrigin();
	mText1.setPosition(0.f, static_cast<float>(mText1.getHeight()));

	mText2.setFont(getContext().fontHolder.get(FontID::Default), 40);
	mText2.setString("Press any key to return to the main menu.");
	mText2.centerOrigin();
	mText2.setPosition(0.f, -static_cast<float>(mText1.getHeight()));
}

bool WinState::update(Time dt) {
	mBackground.setPosition(
		std::sin(0.1f * mClock.getElapsedTime().asSeconds()) * 50.f,
		std::sin(0.15f * (mClock.getElapsedTime().asSeconds() + 2.f)) * 40.f
	);
	return false;
}

void WinState::draw(const Window& window) const {
	window.draw(mBackground);
	window.draw(mText1);
	window.draw(mText2);
}

bool WinState::handleEvent(Event e) {
	if (e.type == Event::Type::KeyPress) {
		requestStackClear();
		requestStackPush(StateID::MainMenu);
	}
	return false;
}
