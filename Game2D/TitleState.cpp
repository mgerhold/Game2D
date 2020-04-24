#include "TitleState.h"
#include <iostream>

TitleState::TitleState(StateStack* stateStack)
	: State(stateStack)
{
	mText.setFont(getContext().fontHolder.get(FontID::Default), 48);
	mText.setString("Click or press a key to continue...");
	mText.centerOrigin();
}

bool TitleState::update(Time dt) {
	mText.setColor(Color(1.f, 1.f, 1.f, std::sin(8.f * mClock.getElapsedTime().asSeconds()) * 0.4f + 0.6f));
	return true;
}

void TitleState::draw(const Window& window) const {	
	window.draw(mText);
}

bool TitleState::handleEvent(Event e) {
	if (e.type == Event::Type::KeyPress || e.type == Event::Type::MouseButtonPress) {
		requestStackPop();
		requestStackPush(StateID::MainMenu);
	}
	return false;
}
