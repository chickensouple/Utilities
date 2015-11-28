#ifndef _CIRCULAR_BUFFER_DYNAMIC_HPP
#define _CIRCULAR_BUFFER_DYNAMIC_HPP

#include "CircularBufferBase.hpp"

template <class T, size_t N>
class CircularBufferDynamic : public CircularBufferBase<T, N> {
public:
	CircularBufferDynamic();

	CircularBufferDynamic(const CircularBufferBase<T, N>& other);

	~CircularBufferDynamic();
};

template <class T, size_t N>
CircularBufferDynamic<T, N>::CircularBufferDynamic() :
	CircularBufferBase<T, N>() {
	this->_arr = new T[N];
}

template <class T, size_t N>
CircularBufferDynamic<T, N>::CircularBufferDynamic(const CircularBufferBase<T, N>& other) :
	CircularBufferBase<T, N>(other) { }

template <class T, size_t N>
CircularBufferDynamic<T, N>::~CircularBufferDynamic() {
	delete[] this->_arr;
}

#endif /* _CIRCULAR_BUFFER_DYNAMIC_HPP */
