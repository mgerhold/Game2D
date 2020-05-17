#pragma once

#include "Window.h"
#include "RenderStates.h"
#include "Drawable.h"
#include "Text.h"
#include "AppContext.h"
#include "Clock.h"
#include "EntityContainer.h"

class Entity;

class Hourglass : public Drawable {
public:
							Hourglass(const AppContext& context, const EntityContainer& entityContainer);
	void					awake();
	void					update(Time dt);

private:
	void					draw(const Window& window, RenderStates states) const override;

private:
	Text					mText;
	AppContext				mContext;
	Clock					mClock;
	Time					mTimeLimit;
	Entity*					mPlayer = nullptr;
	const EntityContainer&	mEntityContainer;
};