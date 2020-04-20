#include "Animation.h"
#include <iostream>

Animation::Animation()
	: mTexture(nullptr)
	, mIsLooping(false)
	, mCurrentStateIndex(0)
	, mIsPlaying(false)
{}

glm::ivec2 Animation::getSize() const {
	if (mAnimationStates.size() == 0)
		return glm::ivec2(0, 0);
	assert(mCurrentStateIndex < mAnimationStates.size());
	return glm::ivec2(
		mAnimationStates[mCurrentStateIndex].textureRect.right - mAnimationStates[mCurrentStateIndex].textureRect.left,
		mAnimationStates[mCurrentStateIndex].textureRect.top - mAnimationStates[mCurrentStateIndex].textureRect.bottom
	);
}

void Animation::setTexture(const Texture& texture) {
	mTexture = &texture;
	mSprite.setTexture(texture);
	mIsPlaying = true;
}

void Animation::update() {
	if (mIsPlaying) {
		size_t newStateIndex = calculateCurrentStateIndex();
		if (newStateIndex != mCurrentStateIndex) {
			if (newStateIndex == 0 && !mIsLooping)
				mIsPlaying = false;
			mCurrentStateIndex = newStateIndex;
			mSprite.setTextureRect(mAnimationStates[mCurrentStateIndex].textureRect);
		}
	}
}

void Animation::generateAnimationStates(int spritesPerRow, int spritesPerColumn, Time duration, bool topToBottom) {
	mAnimationStates.clear();
	mCurrentStateIndex = 0;
	auto texSize = mTexture->getSize();
	glm::ivec2 spriteSize = glm::ivec2(texSize.x / spritesPerRow, texSize.y / spritesPerColumn);
	for (int y = 0; y < spritesPerColumn; y++) {
		for (int x = 0; x < spritesPerRow; x++) {
			IntRect rect;
			if (topToBottom) {
				rect = IntRect(
					x * spriteSize.x,
					(spritesPerColumn - y - 1) * spriteSize.y,
					x * spriteSize.x + spriteSize.x,
					(spritesPerColumn - y) * spriteSize.y
				);
			} else {
				rect = IntRect(
					x * spriteSize.x,
					y * spriteSize.y,
					x * spriteSize.x + spriteSize.x,
					y * spriteSize.y + spriteSize.y
				);
			}
			AnimationState state;
			state.textureRect = rect;
			state.duration = duration;
			mAnimationStates.push_back(state);
		}
	}
	mSprite.setTextureRect(mAnimationStates.front().textureRect);
}

bool Animation::isPlaying() const {
	return mIsPlaying;
}

void Animation::setLooping(bool isLooping) {
	mIsLooping = isLooping;
}

void Animation::draw(const Window& window, RenderStates states) const {
	if (mIsPlaying) {
		states.transform *= getTransform();
		window.draw(mSprite, states);
	}
}

size_t Animation::calculateCurrentStateIndex() {
	if (mAnimationStates.size() == 0)
		return 0;

	Time totalDuration = getDurationSum();
	while (mClock.getElapsedTime() >= totalDuration)
		mClock.setElapsedTime(mClock.getElapsedTime() - totalDuration);
	size_t currentStateIndex = 0;
	Time elapsed = mClock.getElapsedTime();	
	for (size_t i = 0; i < mAnimationStates.size(); i++) {
		if (elapsed >= mAnimationStates[i].duration) {
			elapsed -= mAnimationStates[i].duration;
			currentStateIndex++;
		} else
			break;
	}
	return currentStateIndex;
}

Time Animation::getDurationSum() const {
	Time result;
	for (const auto& animationState : mAnimationStates) {
		result += animationState.duration;
	}
	return result;
}
