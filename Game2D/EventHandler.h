#pragma once

#include "Event.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <queue>
#include <set>
#include <glm/vec2.hpp>
#include <string>
#include <array>
#include "GamepadState.h"

class EventHandler {
public:
															EventHandler();
	void													attachToWindow(GLFWwindow* window);
	bool													hasEvent() const;
	Event													pollEvent();
	void													setKeyRepeat(bool repeat);
	bool													isKeyPressed(Key key) const;
	bool													isMouseButtonPressed(MouseButton mouseButton) const;
	glm::vec2												getMousePosition() const;
	static bool												isJoystickPresent(Joystick joystick);
	static bool												joystickIsGamepad(Joystick joystick);
	static std::string										getJoystickName(Joystick joystick);
	static std::string										getGamepadName(Joystick joystick);
	static void												updateJoysticks();
	static float											getGamepadAxis(Joystick gamepad, GamepadAxis axis);
	static bool												isGamepadButtonPressed(Joystick gamepad, GamepadButton button);

private:
	void													keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void													scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void													mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void													mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
	Key														getKeyFromKeycode(int key) const;

	static void												joystickConnectedCallback(int joystick, int event);
	static void												updateConnectedJoysticks();
	static void												updateConnectedGamepads();
	static GamepadState										getGamepadState(Joystick gamepad);

private:
	GLFWwindow*												mWindow;
	std::queue<Event>										mEventQueue;
	std::set<Key>											mPressedKeys;
	std::set<MouseButton>									mPressedMouseButtons;
	bool													mKeyRepeat;

	static bool												sConnectedJoysticksInitialized;
	static std::array<bool,
		static_cast<size_t>(Joystick::LastJoystick) + 1>	sConnectedJoysticks;
	static bool												sConnectedGamepadsInitialized;
	static std::array<bool,
		static_cast<size_t>(Joystick::LastJoystick) + 1>	sConnectedGamepads;
	static EventHandler*									sJoystickEventReceiver;
	static std::array<GamepadState,
		static_cast<size_t>(Joystick::LastJoystick) + 1>	sGamepadStates;
};

