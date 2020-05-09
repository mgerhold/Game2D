#include "Rect.h"
#include <algorithm>

template<typename T>
Rect<T>::Rect()
	: Rect(0, 0, 0, 0) {}

template<typename T>
Rect<T>::Rect(T left, T bottom, T right, T top)
	: left(left), bottom(bottom), right(right), top(top) {}

template<typename T>
inline bool Rect<T>::isInside(T x, T y) const {
	return (x >= left && x <= right && y >= bottom && y <= top);
}

template<typename T>
inline bool Rect<T>::intersects(const Rect<T>& other) const {
	return (left <= other.right && right >= other.left && top >= other.bottom && bottom <= other.top);
}

template<typename T>
inline Rect<T> Rect<T>::getIntersection(const Rect<T>& other) const {
	return {
		std::max(left, other.left),
		std::max(bottom, other.bottom),
		std::min(right, other.right),
		std::min(top, other.top)
	};
}
