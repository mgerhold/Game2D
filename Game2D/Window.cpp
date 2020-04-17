#include "Window.h"
#include <stdexcept>
#include "Drawable.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include "Utility.h"

bool					Window::sGlfwAndGladInitialized = false;
const Window*			Window::sWindowWithContext = nullptr;
std::vector<Window*>	Window::sWindowList = std::vector<Window*>();

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glfwMakeContextCurrent(window);
	GLCALL(glViewport(0, 0, width, height));
	// recalculate projection matrix for the resized window
	for (const auto& ptr : Window::sWindowList) {
		if (ptr->mWindowPtr == window)
			ptr->calculateProjectionMatrix();
	}
}

Window::Window(
	int width,
	int height,
	const std::string& title,
	int multisamplingSamples,
	bool verticalSync,
	bool resizeable,
	bool decorated,
	bool centerCursor)
	: mWindowPtr(nullptr)
	, mInitialWidth(width)
	, mInitialHeight(height)
	, mProjectionMatrix(glm::mat4())
{
	// initialize GLFW if not done already
	if (!sGlfwAndGladInitialized && !glfwInit())
		throw std::runtime_error("Failed to initialize GLFW!");

	// set window hints before creating the window
	if (multisamplingSamples > 0)
		glfwWindowHint(GLFW_SAMPLES, multisamplingSamples);
	glfwWindowHint(GLFW_RESIZABLE, static_cast<int>(resizeable));
	glfwWindowHint(GLFW_DECORATED, static_cast<int>(decorated));
	glfwWindowHint(GLFW_CENTER_CURSOR, static_cast<int>(centerCursor));

	// create window
	mWindowPtr = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!mWindowPtr)
		throw std::runtime_error("Failed to create window!");

	// set context to current
	sWindowWithContext = this;
	glfwMakeContextCurrent(mWindowPtr);

	// load GLAD if not done already
	if (!sGlfwAndGladInitialized && !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("Failed to initialize GLAD!");
	}

	// enable OpenGL multisampling
	if (multisamplingSamples > 0)
		GLCALL(glEnable(GL_MULTISAMPLE));

	// enable blending
	GLCALL(glEnable(GL_BLEND));
	GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	
	// enable/disable VSync
	glfwSwapInterval(verticalSync ? 1 : 0);

	// set viewport and size change callback
	framebufferSizeCallback(mWindowPtr, width, height);
	glfwSetFramebufferSizeCallback(mWindowPtr, framebufferSizeCallback);

	// attach event handler
	mEventHandler.attachToWindow(mWindowPtr);

	// calculate Projection Matrix
	calculateProjectionMatrix();

	// add this instance to the window list
	Window::sWindowList.push_back(this);

	sGlfwAndGladInitialized = true;
}

Window::~Window() {
	if (mWindowPtr)
		glfwDestroyWindow(mWindowPtr);
	// remove this from instance list
	Window::sWindowList.erase(std::remove_if(Window::sWindowList.begin(), sWindowList.end(),
		[this](Window* ptr) -> bool {
			return ptr == this;
		}),
		Window::sWindowList.end()
	);
}

bool Window::shouldClose() const {
	return mWindowPtr ? glfwWindowShouldClose(mWindowPtr) : true;
}

glm::ivec2 Window::getSize() const {
	glm::ivec2 result;
	if (mWindowPtr)
		glfwGetFramebufferSize(mWindowPtr, &result.x, &result.y);
	return result;
}

void Window::setWindowShouldClose(bool shouldClose) {
	if (mWindowPtr)
		glfwSetWindowShouldClose(mWindowPtr, shouldClose);
}

void Window::clear() const {
	makeContextCurrent();
	glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, mClearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::setClearColor(Color color) {
	mClearColor = color;
}

bool Window::hasEvent() const {
	return mEventHandler.hasEvent();
}

Event Window::pollEvent() {
	return mEventHandler.pollEvent();
}

void Window::setKeyRepeat(bool repeat) {
	mEventHandler.setKeyRepeat(repeat);
}

bool Window::isKeyPressed(Key key) const {
	return mEventHandler.isKeyPressed(key);
}

void Window::setTitle(const std::string& title) {
	glfwSetWindowTitle(mWindowPtr, title.c_str());
}

void Window::makeContextCurrent() const {
	getContext();
}

void Window::draw(const Drawable& drawable, RenderStates states) const {
	makeContextCurrent();
	states.shader->use();
	states.shader->setUniformMat4fv("uProjectionMatrix", mProjectionMatrix);
	drawable.draw(*this, states);
}

void Window::processEvents() {
	glfwPollEvents();
}

void Window::getContext() const {
	if (sWindowWithContext != this) {
		glfwMakeContextCurrent(mWindowPtr);
		sWindowWithContext = this;
	}
	glfwMakeContextCurrent(mWindowPtr);
}

void Window::calculateProjectionMatrix() {
	int width, height;
	glfwGetFramebufferSize(mWindowPtr, &width, &height);
	mProjectionMatrix = glm::ortho(
		-static_cast<GLfloat>(width / 2),
		static_cast<GLfloat>(width / 2),
		-static_cast<GLfloat>(height / 2),
		static_cast<GLfloat>(height / 2)
	);
}

void Window::swapBuffers() const {
	makeContextCurrent();
	if (mWindowPtr)
		glfwSwapBuffers(mWindowPtr);
	else
		throw std::runtime_error("Cannot swap buffers of invalid window!");
}
