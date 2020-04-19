#pragma once

#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum class GamepadButton {
	// TOTO: Use glfw constants
	A = 0,
	B = 1,
	X = 2,
	Y = 3,
	LeftBumper = 4,
	RightBumper = 5,
	Back = 6,
	Start = 7,
	Guide = 8,
	LeftThumb = 9,
	RightThumb = 10,
	DPadUp = 11,
	DPadRight = 12,
	DPadDown = 13,
	DPadLeft = 14,
	Cross = A,
	Circle = B,
	Square = X,
	Triangle = Y,
	LastButton = DPadLeft,
};

enum class GamepadAxis {
	LeftX = GLFW_GAMEPAD_AXIS_LEFT_X,
	LeftY = GLFW_GAMEPAD_AXIS_LEFT_Y,
	RightX = GLFW_GAMEPAD_AXIS_RIGHT_X,
	RightY = GLFW_GAMEPAD_AXIS_RIGHT_Y,
	LeftTrigger = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,
	RightTrigger = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER,
	LastAxis = RightTrigger,
};

struct GamepadAxisChange {
	GamepadAxis axis;
	float value = 0.f;
};

struct GamepadState {
	std::map<GamepadAxis, float> axes = {
		{ GamepadAxis::LeftX, 0.f },
		{ GamepadAxis::LeftY, 0.f },
		{ GamepadAxis::RightX, 0.f },
		{ GamepadAxis::RightY, 0.f },
		{ GamepadAxis::LeftTrigger, 0.f },
		{ GamepadAxis::RightTrigger, 0.f },
	};
	std::map<GamepadButton, bool> buttons = {
		{ GamepadButton::A, false },
		{ GamepadButton::B, false },
		{ GamepadButton::X, false },
		{ GamepadButton::Y, false },
		{ GamepadButton::LeftBumper, false },
		{ GamepadButton::RightBumper, false },
		{ GamepadButton::Back, false },
		{ GamepadButton::Start, false },
		{ GamepadButton::Guide, false },
		{ GamepadButton::LeftThumb, false },
		{ GamepadButton::RightThumb, false },
		{ GamepadButton::DPadUp, false },
		{ GamepadButton::DPadRight, false },
		{ GamepadButton::DPadDown, false },
		{ GamepadButton::DPadLeft, false },
	};
};

bool operator==(const GamepadState& lhs, const GamepadState& rhs);