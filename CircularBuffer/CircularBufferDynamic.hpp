#ifndef _CIRCULAR_BUFFER_DYNAMIC_HPP
#define _CIRCULAR_BUFFER_DYNAMIC_HPP

#include "CircularBufferBase.hpp"

/**
 * @brief Class for Circular Buffers that keeps its contents in the heap
 * @tparam T Type the buffer holds
 * @tparam N Number of items in buffer
 */
template <class T, size_t N>
class CircularBufferDynamic : public CircularBufferBase<T, N> {
public:
	CircularBufferDynamic();

	~CircularBufferDynamic();

	CircularBufferDynamic(const CircularBufferBase<T, N>& other);

	CircularBufferDynamic(const CircularBufferDynamic<T, N>& other);

	CircularBufferDynamic<T, N>& operator=(CircularBufferBase<T, N>& other);

	CircularBufferDynamic<T, N>& operator=(CircularBufferDynamic<T, N>& other);
};

template <class T, size_t N>
CircularBufferDynamic<T, N>::CircularBufferDynamic() :
	CircularBufferBase<T, N>() {
	this->_arr = new T[N];
}

template <class T, size_t N>
CircularBufferDynamic<T, N>::~CircularBufferDynamic() {
	delete[] this->_arr;
}

template <class T, size_t N>
CircularBufferDynamic<T, N>::CircularBufferDynamic(const CircularBufferBase<T, N>& other) { 
	this->_arr = new T[N];
	this->copy(other);
}

template <class T, size_t N>
CircularBufferDynamic<T, N>::CircularBufferDynamic(const CircularBufferDynamic<T, N>& other) { 
	this->_arr = new T[N];
	this->copy(other);
}
template <class T, size_t N>
CircularBufferDynamic<T, N>& CircularBufferDynamic<T, N>::operator=(CircularBufferBase<T, N>& other) {
	this->copy(other);
	return *this;
}

template <class T, size_t N>
CircularBufferDynamic<T, N>& CircularBufferDynamic<T, N>::operator=(CircularBufferDynamic<T, N>& other) {
	this->copy(other);
	return *this;
}

#endif /* _CIRCULAR_BUFFER_DYNAMIC_HPP */
