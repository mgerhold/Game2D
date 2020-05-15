#pragma once

#include "Component.h"
#include <string>
#include <map>
#include <optional>
#include "Animation.h"
#include "AnimationRenderer.h"

class AnimationController : public Component {
public:
	void								addAnimationState(const std::string& name, const Animation& animation);
	void								setAnimation(const std::string& name);
	const std::string*					getCurrentAnimationName() const;
	Animation*							getCurrentAnimation();

private:
	void								onAwake() override;

private:
	std::map<std::string, Animation>	mAnimations;
	Animation*							mCurrentAnimation = nullptr;
	const std::string*					mCurrentAnimationName = nullptr;
	AnimationRenderer*					mAnimationRenderer = nullptr;
};