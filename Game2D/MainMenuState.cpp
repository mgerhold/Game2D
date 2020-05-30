#include "MainMenuState.h"
#include <iostream>

MainMenuState::MainMenuState(StateStack* stateStack)
	: State(stateStack) {
	AppContext context = getContext();
	getContext().textureHolder.load(TextureID::ButtonNormal, "textures/button_normal.png");
	getContext().textureHolder.load(TextureID::ButtonActive, "textures/button_active.png");
	getContext().textureHolder.load(TextureID::ButtonSelected, "textures/button_selected.png");
	getContext().textureHolder.load(TextureID::Checkerboard, "textures/checkerboard.jpg");
	getContext().textureHolder.get(TextureID::Checkerboard).setTextureWrap(true);	
	mBackground.setTexture(getContext().textureHolder.get(TextureID::MainMenuBackground));
	mBackground.setTiling(50, 50);
	mBackground.centerOrigin();

	mLogo.setTexture(getContext().textureHolder.get(TextureID::Logo));
	mLogo.centerOrigin();

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

MainMenuState::~MainMenuState() {
	getContext().textureHolder.unload(TextureID::Checkerboard);
}

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
	return true;
}

void MainMenuState::draw(const Window& window) const {
	window.draw(mBackground);
	window.draw(mGUIContainer);
	window.draw(mLogo);
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
