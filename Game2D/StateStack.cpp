#include "StateStack.h"
#include "RenderStates.h"

StateStack::StateStack(AppContext context)
	: mContext(context)
{ }

void StateStack::push(StateID state) {
	mPendingChanges.push_back({ PendingChangeType::Push, state });
	if (isEmpty())
		applyPendingChanges();
}

void StateStack::pop() {
	mPendingChanges.push_back({ PendingChangeType::Pop, StateID::None });
}

void StateStack::clear() {
	mPendingChanges.push_back({ PendingChangeType::Clear, StateID::None });
}

void StateStack::update(Time dt) {
	for (auto iter = mStates.rbegin(); iter != mStates.rend(); ++iter) {
		if (!(*iter)->update(dt))
			break;
	}
	applyPendingChanges();
}

void StateStack::draw(const Window& window) const {
	for (const auto& state : mStates)
		state->draw(window);
}

void StateStack::handleEvent(Event e) {
	for (auto iter = mStates.rbegin(); iter != mStates.rend(); ++iter) {
		if (!(*iter)->handleEvent(e))
			break;
	}
}

AppContext StateStack::getContext() const {
	return mContext;
}

bool StateStack::isEmpty() const {
	return mStates.size() == 0;
}

void StateStack::applyPendingChanges() {
	for (const auto& change : mPendingChanges) {
		switch (change.type) {
			case PendingChangeType::Clear:
				mStates.clear();
				break;
			case PendingChangeType::Pop:
				mStates.pop_back();
				break;
			case PendingChangeType::Push:
				mStates.push_back(std::move(createState(change.state)));
				break;
		}
	}
	mPendingChanges.clear();
}

State::Ptr StateStack::createState(StateID state) const {
	auto find = mStateFactories.find(state);
	assert(find != mStateFactories.end());
	return (find->second)(); // invoke factory method
}
