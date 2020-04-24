#pragma once

#include "State.h"
#include "Text.h"
#include "Clock.h"

class TitleState : public State {
public:
			TitleState(StateStack* stateStack);

	bool	update(Time dt) override;
	void	draw(const Window& window) const override;
	bool	handleEvent(Event e) override;

private:
	Text	mText;
	Clock	mClock;
};