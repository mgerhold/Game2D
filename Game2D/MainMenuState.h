#pragma once

#include "State.h"
#include "Sprite.h"
#include "Container.h"
#include "Button.h"
#include "Clock.h"
#include "Text.h"

class MainMenuState : public State {
public:
									MainMenuState(StateStack* stateStack);
	virtual							~MainMenuState();

	bool							update(Time dt) override;
	void							draw(const Window& window) const override;
	bool							handleEvent(Event e) override;

private:
	GUI::Container					mGUIContainer;
	std::shared_ptr<GUI::Button>	mTestButton;
	std::shared_ptr<GUI::Button>	mExitButton;
	Sprite							mBackground;
	Sprite							mLogo;
	Clock							mClock;
	Text							mText1, mText2, mText3;
};