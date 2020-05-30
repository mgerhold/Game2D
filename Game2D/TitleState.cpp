#include "TitleState.h"
#include <iostream>

TitleState::TitleState(StateStack* stateStack)
	: State(stateStack)
{
	mText.setFont(getContext().fontHolder.get(FontID::Default), 48);
	mText.setString("Click or press a key to continue...");
	mText.centerOrigin();

	mFMODLogo.setTexture(getContext().textureHolder.get(TextureID::FMODLogo));
	mFMODLogo.setScale(glm::vec2(0.3f));
	mFMODLogo.setPosition(-getContext().window.getSize().x / 2.f + 20.f, -getContext().window.getSize().y / 2.f + 20.f);
}

bool TitleState::update(Time dt) {
	mText.setColor(Color(1.f, 1.f, 1.f, std::sin(4.f * mClock.getElapsedTime().asSeconds()) * 0.4f + 0.6f));
	mFMODLogo.setPosition(-getContext().window.getSize().x / 2.f + 20.f, -getContext().window.getSize().y / 2.f + 20.f);
	return true;
}

void TitleState::draw(const Window& window) const {	
	window.draw(mText);
	window.draw(mFMODLogo);
}

bool TitleState::handleEvent(Event e) {
	if (e.type == Event::Type::KeyPress || e.type == Event::Type::MouseButtonPress) {
		requestStackPop();
		requestStackPush(StateID::MainMenu);
	}
	return false;
}
