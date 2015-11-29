#ifndef _CIRCULAR_BUFFER_STATIC_HPP
#define _CIRCULAR_BUFFER_STATIC_HPP

#include "CircularBufferBase.hpp"

/**
 * @brief Class for Circular Buffers that keep its contents on the stack
 * @tparam T Type the buffer holds
 * @tparam N Number of items in buffer
 */
template <class T, size_t N>
class CircularBufferStatic : public CircularBufferBase<T, N> {
public:
	CircularBufferStatic();

	CircularBufferStatic(const CircularBufferBase<T, N>& other);

	CircularBufferStatic(const CircularBufferStatic<T, N>& other);

	CircularBufferStatic<T, N>& operator=(CircularBufferBase<T, N>& other);

	CircularBufferStatic<T, N>& operator=(CircularBufferStatic<T, N>& other);
protected:
	T _staticArr[N];
};

template <class T, size_t N>
CircularBufferStatic<T, N>::CircularBufferStatic() :
	CircularBufferBase<T, N>() {
	this->_arr = _staticArr;		
}

template <class T, size_t N>
CircularBufferStatic<T, N>::CircularBufferStatic(const CircularBufferBase<T, N>& other) {
	this->_arr = _staticArr;
	this->copy(other);
}

template <class T, size_t N>
CircularBufferStatic<T, N>::CircularBufferStatic(const CircularBufferStatic<T, N>& other) {
	this->_arr = _staticArr;
	this->copy(other);
}

template <class T, size_t N>
CircularBufferStatic<T, N>& CircularBufferStatic<T, N>::operator=(CircularBufferBase<T, N>& other) {
	this->copy(other);
	return *this;
}

template <class T, size_t N>
CircularBufferStatic<T, N>& CircularBufferStatic<T, N>::operator=(CircularBufferStatic<T, N>& other) {
	this->copy(other);
	return *this;
}

#endif /* _CIRCULAR_BUFFER_STATIC_HPP */
