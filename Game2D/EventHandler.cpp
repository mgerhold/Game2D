#include "EventHandler.h"
#include <iostream>
#include <cassert>

EventHandler::EventHandler()
	: mKeyRepeat(true)
{}

void EventHandler::attachToWindow(GLFWwindow* window) {
	glfwSetWindowUserPointer(window, this);

	auto keyCallbackLambda = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		static_cast<EventHandler*>(glfwGetWindowUserPointer(window))->keyCallback(window, key, scancode, action, mods);
	};
	glfwSetKeyCallback(window, keyCallbackLambda);

	auto scrollCallbackLambda = [](GLFWwindow* window, double xoffset, double yoffset) {
		static_cast<EventHandler*>(glfwGetWindowUserPointer(window))->scrollCallback(window, xoffset, yoffset);
	};
	glfwSetScrollCallback(window, scrollCallbackLambda);
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

Key EventHandler::getKeyFromKeycode(int key) const {
	using T = std::underlying_type<Key>::type;
	for (T i = static_cast<T>(Key::Unknown); i <= static_cast<T>(Key::LastKey); i++) {
		if (i == key)
			return static_cast<Key>(i);
	}
	return Key::Unknown;
}
