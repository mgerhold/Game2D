#include "EventHandler.h"
#include <iostream>
#include <cassert>

EventHandler* EventHandler::sJoystickEventReceiver = nullptr;
bool EventHandler::sConnectedJoysticksInitialized = false;
bool EventHandler::sConnectedGamepadsInitialized = false;
std::array<bool, static_cast<int>(Joystick::LastJoystick) + 1> EventHandler::sConnectedJoysticks = {
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
};
std::array<bool, static_cast<int>(Joystick::LastJoystick) + 1> EventHandler::sConnectedGamepads = {
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
};
std::array<GamepadState, static_cast<size_t>(Joystick::LastJoystick) + 1> EventHandler::sGamepadStates = {
	GamepadState(),
	GamepadState(),
	GamepadState(),
	GamepadState(),
	GamepadState(),
	GamepadState(),
	GamepadState(),
	GamepadState(),
	GamepadState(),
	GamepadState(),
	GamepadState(),
	GamepadState(),
	GamepadState(),
	GamepadState(),
	GamepadState(),
	GamepadState(),
};

EventHandler::EventHandler()
	: mKeyRepeat(true)
	, mWindow(nullptr)
{}

void EventHandler::attachToWindow(GLFWwindow* window) {
	mWindow = window;

	glfwSetWindowUserPointer(window, this);

	auto keyCallbackLambda = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		static_cast<EventHandler*>(glfwGetWindowUserPointer(window))->keyCallback(window, key, scancode, action, mods);
	};
	glfwSetKeyCallback(window, keyCallbackLambda);

	auto scrollCallbackLambda = [](GLFWwindow* window, double xoffset, double yoffset) {
		static_cast<EventHandler*>(glfwGetWindowUserPointer(window))->scrollCallback(window, xoffset, yoffset);
	};
	glfwSetScrollCallback(window, scrollCallbackLambda);

	auto mouseButtonCallbackLambda = [](GLFWwindow* window, int button, int action, int mods) {
		static_cast<EventHandler*>(glfwGetWindowUserPointer(window))->mouseButtonCallback(window, button, action, mods);
	};
	glfwSetMouseButtonCallback(window, mouseButtonCallbackLambda);

	auto mouseMoveCallbackLambda = [](GLFWwindow* window, double xpos, double ypos) {
		static_cast<EventHandler*>(glfwGetWindowUserPointer(window))->mouseMoveCallback(window, xpos, ypos);
	};
	glfwSetCursorPosCallback(window, mouseMoveCallbackLambda);

	auto joystickConnectedCallbackLambda = [](int joystick, int event) {
		EventHandler::joystickConnectedCallback(joystick, event);
	};
	glfwSetJoystickCallback(joystickConnectedCallbackLambda);

	if (!sJoystickEventReceiver)
		sJoystickEventReceiver = this;
}

bool EventHandler::hasEvent() const {
	return mEventQueue.size() > 0;
}

Event EventHandler::pollEvent() {
	if (!hasEvent()) {
		Event e;
		e.type = Event::Type::None;
		return e;
	}
	Event e = mEventQueue.front();
	mEventQueue.pop();
	return e;
}

void EventHandler::setKeyRepeat(bool repeat) {
	mKeyRepeat = repeat;
}

bool EventHandler::isKeyPressed(Key key) const {
	auto find = mPressedKeys.find(key);
	return find != mPressedKeys.end();
}

bool EventHandler::isMouseButtonPressed(MouseButton mouseButton) const {
	auto find = mPressedMouseButtons.find(mouseButton);
	return find != mPressedMouseButtons.end();
}

glm::vec2 EventHandler::getMousePosition() const {
	int width, height;
	glfwGetFramebufferSize(mWindow, &width, &height);
	double x, y;
	glfwGetCursorPos(mWindow, &x, &y);
	return glm::vec2(static_cast<float>(x), static_cast<float>(height - y - 1));
}

bool EventHandler::isJoystickPresent(Joystick joystick) {
	return (glfwJoystickPresent(static_cast<int>(joystick)) == GLFW_TRUE);
}

bool EventHandler::joystickIsGamepad(Joystick joystick) {
	return (glfwJoystickIsGamepad(static_cast<int>(joystick)) == GLFW_TRUE);
}

std::string EventHandler::getJoystickName(Joystick joystick) {
	return std::string(glfwGetJoystickName(static_cast<int>(joystick)));
}

std::string EventHandler::getGamepadName(Joystick joystick) {
	return std::string(glfwGetGamepadName(static_cast<int>(joystick)));
}

void EventHandler::updateJoysticks() {
	if (!sConnectedJoysticksInitialized)
		updateConnectedJoysticks();
	if (!sConnectedGamepadsInitialized)
		updateConnectedGamepads();
	for (size_t i = 0; i < sConnectedGamepads.size(); i++) {
		if (sConnectedGamepads[i]) {
			auto newState = getGamepadState(static_cast<Joystick>(i));
			if (newState != sGamepadStates[i] && sJoystickEventReceiver) { // state has changed
				// create button events
				for (size_t j = 0; j <= static_cast<size_t>(GamepadButton::LastButton); j++) {
					if (newState.buttons.at(static_cast<GamepadButton>(j)) && !sGamepadStates[i].buttons.at(static_cast<GamepadButton>(j))) {
						// button pressed
						Event e;
						e.type = Event::Type::GamepadButtonPress;
						e.joystick = static_cast<Joystick>(i);
						e.gamepadButton = static_cast<GamepadButton>(j);
						sJoystickEventReceiver->mEventQueue.push(e);
					} else if (!newState.buttons.at(static_cast<GamepadButton>(j)) && sGamepadStates[i].buttons.at(static_cast<GamepadButton>(j))) {
						// button released
						Event e;
						e.type = Event::Type::GamepadButtonRelease;
						e.joystick = static_cast<Joystick>(i);
						e.gamepadButton = static_cast<GamepadButton>(j);
						sJoystickEventReceiver->mEventQueue.push(e);
					}
				}
				// create axes events
				for (size_t j = 0; j <= static_cast<size_t>(GamepadAxis::LastAxis); j++) {
					if (newState.axes.at(static_cast<GamepadAxis>(j)) != sGamepadStates[i].axes.at(static_cast<GamepadAxis>(j))) {
						// axis has changed
						Event e;
						e.type = Event::Type::GamepadAxisChange;
						e.joystick = static_cast<Joystick>(i);
						e.gamepadAxisChange.axis = static_cast<GamepadAxis>(j);
						e.gamepadAxisChange.value = newState.axes.at(static_cast<GamepadAxis>(j));
						sJoystickEventReceiver->mEventQueue.push(e);
					}
				}
			}
			sGamepadStates[i] = newState;
		}
	}
}

float EventHandler::getGamepadAxis(Joystick gamepad, GamepadAxis axis) {
	return sGamepadStates[static_cast<size_t>(gamepad)].axes.at(axis);
}

bool EventHandler::isGamepadButtonPressed(Joystick gamepad, GamepadButton button) {
	return sGamepadStates[static_cast<size_t>(gamepad)].buttons.at(button);
}

void EventHandler::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Event e;	
	e.type = Event::Type::None;
	if (action == GLFW_PRESS || (action == GLFW_REPEAT && mKeyRepeat))
		e.type = Event::Type::KeyPress;
	else if (action == GLFW_RELEASE)
		e.type = Event::Type::KeyRelease;

	if (e.type != Event::Type::None) {
		e.key = getKeyFromKeycode(key);
		mEventQueue.push(e);
		if (e.type == Event::Type::KeyPress)
			mPressedKeys.insert(e.key);
		else {
			auto find = mPressedKeys.find(e.key);
			assert(find != mPressedKeys.end());
			mPressedKeys.erase(find);
		}
	}
}

void EventHandler::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	Event e;
	e.type = Event::Type::MouseScroll;
	e.mouseScrollDelta.x = xoffset;
	e.mouseScrollDelta.y = yoffset;
	mEventQueue.push(e);
}

void EventHandler::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	Event e;
	e.mouseButton = static_cast<MouseButton>(button);
	auto position = getMousePosition();
	e.mousePosition.x = position.x;
	e.mousePosition.y = position.y;
	if (action == GLFW_PRESS) {
		e.type = Event::Type::MouseButtonPress;
		mPressedMouseButtons.insert(e.mouseButton);
	} else if (action == GLFW_RELEASE) {
		e.type = Event::Type::MouseButtonRelease;
		auto find = mPressedMouseButtons.find(e.mouseButton);
		assert(find != mPressedMouseButtons.end());
		mPressedMouseButtons.erase(find);
	}
	mEventQueue.push(e);
}

void EventHandler::mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
	Event e;
	e.type = Event::Type::MouseMove;
	e.mousePosition.x = static_cast<float>(xpos);
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	e.mousePosition.y = static_cast<float>(height - ypos - 1);
	mEventQueue.push(e);
}

void EventHandler::joystickConnectedCallback(int joystick, int event) {
	if (sJoystickEventReceiver) {
		Event e;
		e.joystick = static_cast<Joystick>(joystick);
		switch (event) {
			case GLFW_CONNECTED:
				e.type = Event::Type::JoystickConnected;
				break;
			case GLFW_DISCONNECTED:
				e.type = Event::Type::JoystickDisconnected;
				break;
		}
		sJoystickEventReceiver->mEventQueue.push(e);
	}
	if (!sConnectedJoysticksInitialized)
		updateConnectedJoysticks();
	if (!sConnectedGamepadsInitialized)
		updateConnectedGamepads();
	if (event == GLFW_CONNECTED) {
		sConnectedJoysticks[static_cast<size_t>(joystick)] = true;
		if (glfwJoystickIsGamepad(joystick))
			sConnectedGamepads[static_cast<size_t>(joystick)] = true;
	} else if (event == GLFW_DISCONNECTED) {
		sConnectedJoysticks[static_cast<size_t>(joystick)] = false;
		sConnectedGamepads[static_cast<size_t>(joystick)] = false;
	}
}

void EventHandler::updateConnectedJoysticks() {
	using T = std::underlying_type<Joystick>::type;
	for (T i = static_cast<T>(0); i <= static_cast<T>(Joystick::LastJoystick); i++) {
		sConnectedJoysticks[static_cast<size_t>(i)] = (glfwJoystickPresent(static_cast<int>(i)) == GLFW_TRUE);
	}
	sConnectedJoysticksInitialized = true;
}

void EventHandler::updateConnectedGamepads() {
	using T = std::underlying_type<Joystick>::type;
	for (T i = static_cast<T>(0); i <= static_cast<T>(Joystick::LastJoystick); i++) {
		sConnectedGamepads[static_cast<size_t>(i)] = (glfwJoystickIsGamepad(static_cast<int>(i)) == GLFW_TRUE);
	}
	sConnectedGamepadsInitialized = true;
}

GamepadState EventHandler::getGamepadState(Joystick gamepad) {
	GamepadState result;
	GLFWgamepadstate state;
	if (glfwGetGamepadState(static_cast<int>(gamepad), &state) == GLFW_TRUE) {
		for (size_t i = 0; i <= static_cast<size_t>(GamepadButton::LastButton); i++)
			result.buttons[static_cast<GamepadButton>(i)] = (state.buttons[i] == GLFW_PRESS);
		for (size_t i = 0; i <= static_cast<size_t>(GamepadAxis::LastAxis); i++)
			result.axes[static_cast<GamepadAxis>(i)] = state.axes[i];
	}
	return result;
}

Key EventHandler::getKeyFromKeycode(int key) const {
	using T = std::underlying_type<Key>::type;
	for (T i = static_cast<T>(Key::Unknown); i <= static_cast<T>(Key::LastKey); i++) {
		if (i == key)
			return static_cast<Key>(i);
	}
	return Key::Unknown;
}
