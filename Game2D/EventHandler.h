#pragma once

#include "Event.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <queue>
#include <set>

class EventHandler {
public:
						EventHandler();
	void				attachToWindow(GLFWwindow* window);
	bool				hasEvent() const;
	Event				pollEvent();
	void				setKeyRepeat(bool repeat);
	bool				isKeyPressed(Key key) const;
private:
	void				keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void				scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	Key					getKeyFromKeycode(int key) const;
private:
	std::queue<Event>	mEventQueue;
	std::set<Key>		mPressedKeys;
	bool				mKeyRepeat;
};

