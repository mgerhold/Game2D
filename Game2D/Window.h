#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <string>
#include "NonCopyable.h"
#include "Color.h"
#include "EventHandler.h"
#include <vector>

class Drawable;
struct RenderStates;

class Window : private NonCopyable
{
public:
								Window(
									int width,
									int height,
									const std::string& title,
									int multisamplingSamples = 2,
									bool verticalSync = true,
									bool resizeable = true,
									bool decorated = true,
									bool centerCursor = false
								);
								~Window();
	bool						shouldClose() const;
	glm::ivec2					getSize() const;
	void						setWindowShouldClose(bool shouldClose);
	void						swapBuffers() const;
	void						clear() const;
	void						setClearColor(Color color);
	bool						hasEvent() const;
	Event						pollEvent();
	void						setKeyRepeat(bool repeat);
	bool						isKeyPressed(Key key) const;
	void						setTitle(const std::string& title);
	void						makeContextCurrent() const;
	void						draw(const Drawable& drawable, RenderStates states) const;
	// static methods
	static void					processEvents();

private:
	void						getContext() const;
	void						calculateProjectionMatrix();

private:
	GLFWwindow*					mWindowPtr;
	Color						mClearColor;
	EventHandler				mEventHandler;
	int							mInitialWidth;
	int							mInitialHeight;
	glm::mat4					mProjectionMatrix;

	// static variables
	static bool					sGlfwAndGladInitialized;
	static const Window*		sWindowWithContext;
	static std::vector<Window*>	sWindowList;

	friend class EventHandler;
	friend void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};

