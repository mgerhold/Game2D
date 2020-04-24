#pragma once
#include "State.h"
#include <vector>
#include <functional>
#include <map>
#include "AppContext.h"

class StateStack {
public:
												StateStack(AppContext context);
	void										push(StateID state);
	void										pop();
	void										clear();
	void										update(Time dt);
	void										draw(const Window& window) const;
	void										handleEvent(Event e);
	AppContext									getContext() const;
	template<typename T>
	void										registerState(StateID state);
	bool										isEmpty() const;

private:
	enum class PendingChangeType {
		Push,
		Pop,
		Clear,
	};

	struct PendingChange {
		PendingChangeType type;
		StateID state;
	};

private:
	using StateFactoryFunc = std::function<State::Ptr()>;

private:
	void										applyPendingChanges();
	State::Ptr									createState(StateID state) const;

private:
	std::vector<State::Ptr>						mStates;
	std::vector<PendingChange>					mPendingChanges;
	std::map<StateID, StateFactoryFunc> mStateFactories;
	AppContext									mContext;
};

template<typename T>
inline void StateStack::registerState(StateID state) {
	mStateFactories[state] = [this]() -> State::Ptr {
		return std::make_unique<T>(this);
	};
}