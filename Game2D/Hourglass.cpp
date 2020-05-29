#include "Hourglass.h"
#include "AppContext.h"
#include "PlayerController.h"
#include "RigidBody.h"
#include <iostream>

Hourglass::Hourglass(const AppContext& context, const EntityContainer& entityContainer)
	: mContext(context)
	, mEntityContainer(entityContainer)
{}

void Hourglass::awake() {
	mClock.restart();	

	auto entityList = mEntityContainer.getEntitiesWithComponent<PlayerController>();
	mPlayer = entityList.front();

	mAnimationRunning.setTexture(mContext.textureHolder.get(TextureID::HourglassRunning));
	mAnimationRunning.generateAnimationStates(4, 2, mTimeLimit / (4.f * 2.f));
	mAnimationRunning.setHold(true);

	mAnimationTurning.setTexture(mContext.textureHolder.get(TextureID::HourglassTurning));
	mAnimationTurning.generateAnimationStates(2, 2, Time::seconds(0.1f));
	mAnimationTurning.setHold(true);
	mAnimationTurning.setAnimationEndCallback([this]() {
			std::cout << "Animation ended\n";
			mTurning = false;
			mAnimationRunning.reset();
		});

	mSound.setSoundBuffer(mContext.soundBufferHolder.get(SoundID::HourglassFX));
}

void Hourglass::update(Time dt) {
	if (mTurning)
		mAnimationTurning.update();
	else
		mAnimationRunning.update();

	if (mClock.getElapsedTime() >= mTimeLimit) {
		// time has elapsed
		mSound.play();
		mClock.setElapsedTime(mClock.getElapsedTime() - mTimeLimit);
		auto rb = mPlayer->getComponent<RigidBody>();
		rb->setGravity(-rb->getGravity());

		mTurning = !mTurning;
		mAnimationTurning.reset();
	}

	auto position = glm::ivec2(
		mContext.window.getSize().x / 2 - mAnimationRunning.getWidth() - 20,
		-mContext.window.getSize().y / 2 + 20
	);

	mAnimationRunning.setPosition(position);
	mAnimationTurning.setPosition(position);
}

void Hourglass::draw(const Window& window, RenderStates states) const {
	//window.draw(mText, states);
	window.draw(mTurning ? mAnimationTurning : mAnimationRunning, states);
}
