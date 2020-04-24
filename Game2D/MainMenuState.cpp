#include "MainMenuState.h"

MainMenuState::MainMenuState(StateStack* stateStack)
	: State(stateStack) {
	AppContext context = getContext();
	getContext().textureHolder.load(TextureID::ButtonNormal, "textures/button_normal.png");
	getContext().textureHolder.load(TextureID::ButtonActive, "textures/button_active.png");
	getContext().textureHolder.load(TextureID::ButtonSelected, "textures/button_selected.png");
	getContext().textureHolder.load(TextureID::Checkerboard, "textures/checkerboard.jpg");
	getContext().textureHolder.get(TextureID::Checkerboard).setTextureWrap(true);	
	mCheckerboard.setTexture(getContext().textureHolder.get(TextureID::Checkerboard));
	mCheckerboard.setTiling(5, 5);	
	mCheckerboard.centerOrigin();

	mExitButton = std::make_shared<GUI::Button>();
	mExitButton->setNormalTexture(getContext().textureHolder.get(TextureID::ButtonNormal));
	mExitButton->setSelectedTexture(getContext().textureHolder.get(TextureID::ButtonSelected));
	mExitButton->setActiveTexture(getContext().textureHolder.get(TextureID::ButtonActive));
	mExitButton->setString("Exit");
	mExitButton->setFont(getContext().fontHolder.get(FontID::Default), 20);
	mExitButton->centerOrigin();
	mExitButton->setCallbackFunc([this]() {
		requestStackClear();
	});

	mTestButton = std::make_shared<GUI::Button>();
	mTestButton->setNormalTexture(getContext().textureHolder.get(TextureID::ButtonNormal));
	mTestButton->setSelectedTexture(getContext().textureHolder.get(TextureID::ButtonSelected));
	mTestButton->setActiveTexture(getContext().textureHolder.get(TextureID::ButtonActive));
	mTestButton->setString("Test");
	mTestButton->setFont(getContext().fontHolder.get(FontID::Default), 20);
	mTestButton->centerOrigin();
	mTestButton->setPosition(0.f, mTestButton->getHeight() + 20.f);
	mTestButton->setCallbackFunc([]() {
		std::cout << "This is the test button\n";
	});

	mBackButton = std::make_shared<GUI::Button>();
	mBackButton->setNormalTexture(getContext().textureHolder.get(TextureID::ButtonNormal));
	mBackButton->setSelectedTexture(getContext().textureHolder.get(TextureID::ButtonSelected));
	mBackButton->setActiveTexture(getContext().textureHolder.get(TextureID::ButtonActive));
	mBackButton->setString("Back");
	mBackButton->setFont(getContext().fontHolder.get(FontID::Default), 20);
	mBackButton->centerOrigin();
	mBackButton->setPosition(0.f, 2.f * (mTestButton->getHeight() + 20.f));
	mBackButton->setCallbackFunc([this]() {
		requestStackPop();
		requestStackPush(StateID::Title);
	});

	mGUIContainer.pack(mExitButton);
	mGUIContainer.pack(mTestButton);
	mGUIContainer.pack(mBackButton);
}

MainMenuState::~MainMenuState() {
	getContext().textureHolder.unload(TextureID::Checkerboard);
}

bool MainMenuState::update(Time dt) {
	mCheckerboard.setRotation(mCheckerboard.getRotation() + glm::radians(40.f * dt.asSeconds()));
	return true;
}

void MainMenuState::draw(const Window& window) const {
	window.draw(mCheckerboard);
	window.draw(mGUIContainer);
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
