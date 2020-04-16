#include "Color.h"
#include <algorithm>

Color::Color()
	: Color(1.0f, 1.0f, 1.0f, 1.0f)
{}

Color::Color(float r, float g, float b, float a)
	: r(r)
	, g(g)
	, b(b)
	, a(a)
{
	clampColors();
}

void Color::clampColors() {
	r = std::clamp(r, 0.0f, 1.0f);
	g = std::clamp(g, 0.0f, 1.0f);
	b = std::clamp(b, 0.0f, 1.0f);
	a = std::clamp(a, 0.0f, 1.0f);
}
