#include "AnimationController.h"
#include <algorithm>
#include <iostream>
#include "Entity.h"

void AnimationController::addAnimationState(const std::string& name, const Animation& animation) {
	mAnimations[name] = animation;
	if (mAnimations.size() == 1)
		setAnimation(name);
}

void AnimationController::setAnimation(const std::string& name) {
	auto find = mAnimations.find(name);
	if (find != mAnimations.end()) {
		mAnimationRenderer->setAnimation(find->second);
		mCurrentAnimation = &find->second;
		mCurrentAnimationName = &find->first;
	} else {
		std::cout << "ERROR (AnimationController::setAnimation): Could not find animation called " << name << "!\n";
	}
}

const std::string* AnimationController::getCurrentAnimationName() const {
	if (mCurrentAnimationName)
		return mCurrentAnimationName;
	return nullptr;
}

Animation* AnimationController::getCurrentAnimation() {
	if (mCurrentAnimation)
		return mCurrentAnimation;
	return nullptr;
}

void AnimationController::onAwake() {
	mAnimationRenderer = getEntity()->getComponent<AnimationRenderer>();
}
