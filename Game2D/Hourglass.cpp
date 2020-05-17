#include "Hourglass.h"
#include "AppContext.h"
#include "PlayerController.h"
#include "RigidBody.h"

Hourglass::Hourglass(const AppContext& context, const EntityContainer& entityContainer)
	: mContext(context)
	, mEntityContainer(entityContainer)
{}

void Hourglass::awake() {
	mText.setFont(mContext.fontHolder.get(FontID::Default), 48);
	mText.setString("Test");
	mClock.restart();
	// TODO: Remove magic number
	mTimeLimit = Time::seconds(5.f);

	auto entityList = mEntityContainer.getEntitiesWithComponent<PlayerController>();
	mPlayer = entityList.front();
}

void Hourglass::update(Time dt) {
	if (mClock.getElapsedTime() >= mTimeLimit) {
		mClock.setElapsedTime(mClock.getElapsedTime() - mTimeLimit);
		auto rb = mPlayer->getComponent<RigidBody>();
		rb->setGravity(-rb->getGravity());
	}
	mText.setString(std::to_string((mTimeLimit - mClock.getElapsedTime()).asSeconds()) + " s");	
	auto position = glm::ivec2(
		mContext.window.getSize().x / 2 - mText.getWidth() - 20,
		-mContext.window.getSize().y / 2 + 20
	);
	if (static_cast<glm::ivec2>(mText.getPosition()) != position)
		mText.setPosition(position);
}

void Hourglass::draw(const Window& window, RenderStates states) const {
	window.draw(mText, states);
}
