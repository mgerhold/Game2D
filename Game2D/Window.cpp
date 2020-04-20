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
	if (!sGlfwAndGladInitialized) {
		int result = GLFW_TRUE;
		try {
			result = glfwInit();
		}
		catch (...) {
			try {
				std::exception_ptr eptr = std::current_exception();
				if (eptr)
					std::rethrow_exception(eptr);
			} catch (const std::exception& e) {
				std::cout << "EXCEPTION: " << e.what() << "\n";
			}			
		}
		if (result != GLFW_TRUE)
			throw std::runtime_error("Failed to initialize GLFW!");
	}

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

	// center window
	centerWindow(mWindowPtr, getBestMonitor(mWindowPtr));

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

	// TODO: Refactor!
	mDefaultShader.loadFromFile("shaders/default.vert", Shader::Type::Vertex);
	mDefaultShader.loadFromFile("shaders/default.frag", Shader::Type::Fragment);
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
	glm::ivec2 result(0);
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
	GLCALL(glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, mClearColor.a));
	GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
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

bool Window::isMouseButtonPressed(MouseButton mouseButton) const {
	return mEventHandler.isMouseButtonPressed(mouseButton);
}

glm::vec2 Window::getMousePosition() const {
	return mEventHandler.getMousePosition();
}

void Window::setTitle(const std::string& title) {
	glfwSetWindowTitle(mWindowPtr, title.c_str());
}

void Window::makeContextCurrent() const {
	getContext();
}

void Window::draw(const Drawable& drawable, const Camera& camera) const {
	RenderStates states;	
	states.transform.setMatrix(mProjectionMatrix * camera.getMatrix());
	states.shader = &mDefaultShader;
	draw(drawable, states);
}

void Window::draw(const Drawable& drawable, RenderStates states) const {
	makeContextCurrent();
	drawable.draw(*this, states);
}

glm::vec2 Window::windowToWorldCoords(glm::vec2 windowCoords, const Camera& camera) const {
	windowCoords = glm::vec2(windowCoords.x * 2.f / getSize().x - 1.f, windowCoords.y * 2.f / getSize().y - 1.f);
	glm::vec4 homogeneous = glm::inverse(camera.getMatrix()) * glm::inverse(mProjectionMatrix) * glm::vec4(windowCoords.x, windowCoords.y, 0.f, 1.f);
	return glm::vec2(homogeneous.x, homogeneous.y);
}

glm::vec2 Window::worldToWindowCoords(glm::vec2 worldCoords, const Camera& camera) const {
	glm::vec4 homogeneous = mProjectionMatrix * camera.getMatrix() * glm::vec4(worldCoords.x, worldCoords.y, 0.f, 1.f);
	return glm::vec2(homogeneous.x, homogeneous.y);
}

void Window::processEvents() {
	// TODO: move this outside of the window class and into an application class
	glfwPollEvents();
	EventHandler::updateJoysticks();
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

void Window::centerWindow(GLFWwindow* window, GLFWmonitor* monitor) const {
	// taken from https://vallentin.dev/2014/02/07/glfw-center-window
	if (!monitor)
		return;

	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	if (!mode)
		return;

	int monitorX, monitorY;
	glfwGetMonitorPos(monitor, &monitorX, &monitorY);

	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);

	glfwSetWindowPos(window,
		monitorX + (mode->width - windowWidth) / 2,
		monitorY + (mode->height - windowHeight) / 2);
}

GLFWmonitor* Window::getBestMonitor(GLFWwindow* window) const {
	// taken from https://vallentin.dev/2014/02/07/glfw-center-window
	int monitorCount;
	GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);

	if (!monitors)
		return NULL;

	int windowX, windowY, windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	glfwGetWindowPos(window, &windowX, &windowY);

	GLFWmonitor* bestMonitor = NULL;
	int bestArea = 0;

	for (int i = 0; i < monitorCount; ++i) {
		GLFWmonitor* monitor = monitors[i];

		int monitorX, monitorY;
		glfwGetMonitorPos(monitor, &monitorX, &monitorY);

		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		if (!mode)
			continue;

		int areaMinX = std::max(windowX, monitorX);
		int areaMinY = std::max(windowY, monitorY);

		int areaMaxX = std::min(windowX + windowWidth, monitorX + mode->width);
		int areaMaxY = std::min(windowY + windowHeight, monitorY + mode->height);

		int area = (areaMaxX - areaMinX) * (areaMaxY - areaMinY);

		if (area > bestArea) {
			bestArea = area;
			bestMonitor = monitor;
		}
	}

	return bestMonitor;
}

void Window::swapBuffers() const {
	makeContextCurrent();
	if (mWindowPtr)
		glfwSwapBuffers(mWindowPtr);
	else
		throw std::runtime_error("Cannot swap buffers of invalid window!");
}
