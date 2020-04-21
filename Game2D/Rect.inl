#include "Rect.h"
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
