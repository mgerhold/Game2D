template<typename T>
Rect<T>::Rect()
	: Rect(0, 0, 0, 0) {}

template<typename T>
Rect<T>::Rect(T left, T bottom, T right, T top)
	: left(left), bottom(bottom), right(right), top(top) {}