#pragma once

#include "Transformable.h"
#include "Drawable.h"
#include <vector>
#include "Rect.h"
#include "Clock.h"
#include "Texture.h"
#include "Sprite.h"

class Animation 
	: public Transformable
	, public Drawable
{
public:
	Animation();

	virtual glm::ivec2	getSize() const override;
	void	setTexture(const Texture& texture);
	void	update();
	void	generateAnimationStates(int spritesPerRow, int spritesPerColumn, Time duration, bool topToBottom = true);
	bool	isPlaying() const;
	void	setLooping(bool isLooping);
	void	setHold(bool hold);

private:
	struct AnimationState {
		IntRect	textureRect;
		Time	duration;
	};

private:
	virtual void draw(const Window& window, RenderStates states) const;
	size_t	calculateCurrentStateIndex();
	Time	getDurationSum() const;

private:
	std::vector<AnimationState> mAnimationStates;
	const Texture*	mTexture;
	Clock	mClock;
	bool	mIsLooping;
	size_t	mCurrentStateIndex;
	Sprite	mSprite;
	bool	mIsPlaying;
	bool	mHold;
	bool	mIsHolding;
};