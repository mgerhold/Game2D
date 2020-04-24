#include "State.h"
#include "StateStack.h"

State::State(StateStack* stateStack)
	: mStateStack(stateStack)
{}

void State::requestStackPop() {
	mStateStack->pop();
}

void State::requestStackClear() {
	mStateStack->clear();
}

void State::requestStackPush(StateID state) {
	mStateStack->push(state);
}

AppContext State::getContext() const {
	return mStateStack->getContext();
}
