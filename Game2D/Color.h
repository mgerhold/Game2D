#pragma once
class Color {
public:
	float r;
	float g;
	float b;
	float a;

	static const Color Black;
public:
	Color();
	Color(float r, float g, float b, float a = 1.0f);
private:
	void clampColors();
};

