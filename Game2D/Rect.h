#pragma once

template<typename T>
struct Rect {
	T left;
	T bottom;
	T right;
	T top;

	Rect();
	Rect(T left, T bottom, T right, T top);
};

using IntRect = Rect<int>;
using FloatRect = Rect<float>;

#include "Rect.inl"

/*struct IntRect {
	int left;
	int bottom;
	int right;
	int top;

	IntRect();
	IntRect(int left, int bottom, int right, int top);
};*/