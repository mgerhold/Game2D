#pragma once

#include <memory>
#include "Time.h"
#include "Window.h"
#include "StateIdentifiers.h"
#include "RenderStates.h"
#include "AppContext.h"

class StateStack;

class State {
public:
	using Ptr = std::unique_ptr<State>;

public:
					State(StateStack* stateStack);
	virtual			~State() = default;

	virtual bool	update(Time dt) = 0;
	virtual void	draw(const Window& window) const = 0;
	virtual bool	handleEvent(Event e) = 0;

protected:
	void			requestStackPop();
	void			requestStackClear();
	void			requestStackPush(StateID state);
	AppContext		getContext() const;

private:
	StateStack* mStateStack = nullptr;
};