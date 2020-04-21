#pragma once

template<typename T>
struct Rect {
	T left;
	T bottom;
	T right;
	T top;

	Rect();
	Rect(T left, T bottom, T right, T top);
	bool isInside(T x, T y) const;
};

using IntRect = Rect<int>;
using FloatRect = Rect<float>;

#include "Rect.inl"