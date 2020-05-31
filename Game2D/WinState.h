#pragma once

#include "State.h"
#include "Sprite.h"
#include "Text.h"
#include "Clock.h"

class WinState : public State {
public:
	WinState(StateStack* stateStack);

	bool	update(Time dt) override;
	void	draw(const Window& window) const override;
	bool	handleEvent(Event e) override;

private:
	Sprite	mBackground;
	Text	mText1;
	Text	mText2;
	Clock	mClock;
};