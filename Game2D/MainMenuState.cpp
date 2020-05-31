#include "MainMenuState.h"
#include <iostream>

MainMenuState::MainMenuState(StateStack* stateStack)
	: State(stateStack) {
	AppContext context = getContext();
	getContext().textureHolder.load(TextureID::ButtonNormal, "textures/button_normal.png");
	getContext().textureHolder.load(TextureID::ButtonActive, "textures/button_active.png");
	getContext().textureHolder.load(TextureID::ButtonSelected, "textures/button_selected.png");
	mBackground.setTexture(getContext().textureHolder.get(TextureID::MainMenuBackground));
	mBackground.setTiling(50, 50);
	mBackground.centerOrigin();

	mLogo.setTexture(getContext().textureHolder.get(TextureID::Logo));
	mLogo.centerOrigin();

	mText1.setFont(getContext().fontHolder.get(FontID::Default), 16);
	mText1.setString("game design and graphics: Kevin H.");
	mText1.centerOrigin();

	mText2.setFont(getContext().fontHolder.get(FontID::Default), 16);
	mText2.setString("programming and audio: Michael G.");
	mText2.centerOrigin();

	mText3.setFont(getContext().fontHolder.get(FontID::Default), 16);
	mText3.setString("voice acting: Julia W.");
	mText3.centerOrigin();

	mTestButton = std::make_shared<GUI::Button>();
	mTestButton->setNormalTexture(getContext().textureHolder.get(TextureID::ButtonNormal));
	mTestButton->setSelectedTexture(getContext().textureHolder.get(TextureID::ButtonSelected));
	mTestButton->setActiveTexture(getContext().textureHolder.get(TextureID::ButtonActive));
	mTestButton->setString("Play");
	mTestButton->setFont(getContext().fontHolder.get(FontID::Default), 20);
	mTestButton->setPosition(-mTestButton->getWidth() / 2.f, 0.f);
	mTestButton->setCallbackFunc([this]() {
		requestStackClear();
		requestStackPush(StateID::Game);
	});

	mExitButton = std::make_shared<GUI::Button>();
	mExitButton->setNormalTexture(getContext().textureHolder.get(TextureID::ButtonNormal));
	mExitButton->setSelectedTexture(getContext().textureHolder.get(TextureID::ButtonSelected));
	mExitButton->setActiveTexture(getContext().textureHolder.get(TextureID::ButtonActive));
	mExitButton->setString("Exit");
	mExitButton->setFont(getContext().fontHolder.get(FontID::Default), 20);
	mExitButton->setPosition(-mExitButton->getWidth() / 2.f, -mExitButton->getHeight() - 20.f);
	mExitButton->setCallbackFunc([this]() {
		requestStackClear();
	});

	mGUIContainer.pack(mExitButton);
	mGUIContainer.pack(mTestButton);
}

MainMenuState::~MainMenuState() { }

bool MainMenuState::update(Time dt) {
	mLogo.setPosition(0.f, getContext().window.getSize().y / 4.f);
	mLogo.setScale(
		std::sin(mClock.getElapsedTime().asSeconds()) * 0.05f + 0.65f,
		-std::sin(mClock.getElapsedTime().asSeconds()) * 0.05f + 0.65f
	);

	mBackground.setPosition(
		std::sin(0.1f * mClock.getElapsedTime().asSeconds()) * 50.f,
		std::sin(0.15f * (mClock.getElapsedTime().asSeconds() + 2.f)) * 40.f
	);

	mText1.setPosition(0.f, -getContext().window.getSize().y / 2.f + 6.f * mText1.getHeight());
	mText2.setPosition(0.f, -getContext().window.getSize().y / 2.f + 4.f * mText1.getHeight());
	mText3.setPosition(0.f, -getContext().window.getSize().y / 2.f + 2.f * mText1.getHeight());
	return true;
}

void MainMenuState::draw(const Window& window) const {
	window.draw(mBackground);
	window.draw(mGUIContainer);
	window.draw(mLogo);
	window.draw(mText1);
	window.draw(mText2);
	window.draw(mText3);
}

bool MainMenuState::handleEvent(Event e) {
	mGUIContainer.handleEvent(e, getContext().window);
	switch (e.type) {
		case Event::Type::KeyPress:
			switch (e.key) {
				case Key::Escape:
					requestStackClear();
					break;
			}
			break;
	}
	return false;
}
