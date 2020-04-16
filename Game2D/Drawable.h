#pragma once

#include "RenderStates.h"
#include "Window.h"

class Drawable {
private:
	virtual void draw(const Window& window, RenderStates states) const = 0;

	friend class Window;
};