#ifndef _CIRCULAR_BUFFER_DYNAMIC_HPP
#define _CIRCULAR_BUFFER_DYNAMIC_HPP

#include "CircularBufferBase.hpp"

/**
 * @brief Class for Circular Buffers that keeps its contents in the heap
 * @tparam T Type the buffer holds
 * @tparam N Number of items in buffer
 */
template <class T>
class CircularBufferDynamic : public CircularBufferBase<T> {
public:
	CircularBufferDynamic(size_t capacity);

	~CircularBufferDynamic();

	CircularBufferDynamic(const CircularBufferBase<T>& other);

	CircularBufferDynamic(const CircularBufferDynamic<T>& other);

	CircularBufferDynamic(CircularBufferDynamic<T>&& other);

	CircularBufferDynamic<T>& operator=(CircularBufferBase<T>& other);

	CircularBufferDynamic<T>& operator=(CircularBufferDynamic<T>& other);

	CircularBufferDynamic<T>& operator=(CircularBufferDynamic<T>&& other);
};

template <class T>
CircularBufferDynamic<T>::CircularBufferDynamic(size_t capacity) :
	CircularBufferBase<T>(capacity) {
	this->_arr = new T[capacity];
}

template <class T>
CircularBufferDynamic<T>::~CircularBufferDynamic() {
	delete[] this->_arr;
}

template <class T>
CircularBufferDynamic<T>::CircularBufferDynamic(const CircularBufferBase<T>& other) { 
	this->_arr = new T[other._capacity];
	this->copy(other);
}

template <class T>
CircularBufferDynamic<T>::CircularBufferDynamic(const CircularBufferDynamic<T>& other) { 
	this->_arr = new T[other._capacity];
	this->copy(other);
}

template <class T>
CircularBufferDynamic<T>::CircularBufferDynamic(CircularBufferDynamic<T>&& other) {
	this->_num = other._num;
	this->_frontIdx = other._frontIdx;
	this->_backIdx = other._backIdx;
	this->_arr = other._arr;
}

template <class T>
CircularBufferDynamic<T>& CircularBufferDynamic<T>::operator=(CircularBufferBase<T>& other) {
	this->copy(other);
	return *this;
}

template <class T>
CircularBufferDynamic<T>& CircularBufferDynamic<T>::operator=(CircularBufferDynamic<T>& other) {
	this->copy(other);
	return *this;
}

template <class T>
CircularBufferDynamic<T>& CircularBufferDynamic<T>::operator=(CircularBufferDynamic<T>&& other) {
	this->_num = other._num;
	this->_frontIdx = other._frontIdx;
	this->_backIdx = other._backIdx;
	this->_arr = other._arr;
}

#endif /* _CIRCULAR_BUFFER_DYNAMIC_HPP */
