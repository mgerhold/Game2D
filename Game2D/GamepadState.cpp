#include "GamepadState.h"

bool operator==(const GamepadState& lhs, const GamepadState& rhs) {
	if (lhs.axes.size() != rhs.axes.size())
		return false;

	for (size_t i = 0; i < lhs.axes.size(); i++) {
		if (lhs.axes.at(static_cast<GamepadAxis>(i)) != rhs.axes.at(static_cast<GamepadAxis>(i)))
			return false;
	}

	using T = std::underlying_type<GamepadButton>::type;
	for (T i = 0; i <= static_cast<T>(GamepadButton::LastButton); i++) {
		if (lhs.buttons.at(static_cast<GamepadButton>(i)) != rhs.buttons.at(static_cast<GamepadButton>(i)))
			return false;
	}
	return true;
}