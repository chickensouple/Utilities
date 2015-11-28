#ifndef _CIRCULAR_BUFFER_STATIC_HPP
#define _CIRCULAR_BUFFER_STATIC_HPP

#include "CircularBufferBase.hpp"

template <class T, size_t N>
class CircularBufferStatic : public CircularBufferBase<T, N> {
public:
	CircularBufferStatic();

	CircularBufferStatic(const CircularBufferBase<T, N>& other);

protected:
	T _staticArr[N];
};

template <class T, size_t N>
CircularBufferStatic<T, N>::CircularBufferStatic() :
	CircularBufferBase<T, N>() {
	this->_arr = _staticArr;		
}

template <class T, size_t N>
CircularBufferStatic<T, N>::CircularBufferStatic(const CircularBufferBase<T, N>& other) :
	CircularBufferBase<T, N>(other) { }

#endif /* _CIRCULAR_BUFFER_STATIC_HPP */
