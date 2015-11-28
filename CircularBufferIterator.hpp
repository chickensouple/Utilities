#ifndef _CIRCULAR_BUFFER_ITERATOR_HPP
#define _CIRCULAR_BUFFER_ITERATOR_HPP

template <class T, size_t N>
class CircularBufferIterator {
public:
	CircularBufferIterator();
	CircularBufferIterator(CircularBufferBase<T, N>* ptr);
	CircularBufferIterator(CircularBufferBase<T, N>* ptr, size_t idx);
	CircularBufferIterator(const CircularBufferIterator<T, N>& other);

	bool valid() const;

	const CircularBufferIterator& operator=(const CircularBufferIterator<T, N>& other);
	CircularBufferIterator<T, N>& operator++();
	CircularBufferIterator<T, N> operator++(int);
	CircularBufferIterator<T, N>& operator--();
	CircularBufferIterator<T, N> operator--(int);
	bool operator==(const CircularBufferIterator<T, N>& other) const;
	bool operator!=(const CircularBufferIterator<T, N>& other) const;
	bool operator>(const CircularBufferIterator<T, N>& other) const;
	bool operator>=(const CircularBufferIterator<T, N>& other) const;
	bool operator<(const CircularBufferIterator<T, N>& other) const;
	bool operator<=(const CircularBufferIterator<T, N>& other) const;
	T& operator*();

	template <class _T, size_t _N>
	friend CircularBufferIterator<_T, _N> operator+(const CircularBufferIterator<_T, _N>& it, int n);

	template <class _T, size_t _N>
	friend CircularBufferIterator<_T, _N> operator+(int n, const CircularBufferIterator<_T, _N>& it);

	template <class _T, size_t _N>
	friend CircularBufferIterator<_T, _N> operator-(const CircularBufferIterator<_T, _N>& it, int n);

	template <class _T, size_t _N>
	friend int operator-(const CircularBufferIterator<_T, _N>& it1, const CircularBufferIterator<_T, _N>& it2);

protected:
	CircularBufferBase<T, N>* _ptr;
	size_t _idx;
};

template <class T, size_t N>
CircularBufferIterator<T, N>::CircularBufferIterator() :
	_ptr(nullptr),
	_idx(0) { }

template <class T, size_t N>
CircularBufferIterator<T, N>::CircularBufferIterator(CircularBufferBase<T, N>* ptr) :
	_ptr(ptr),
	_idx(0) { }

template <class T, size_t N>
CircularBufferIterator<T, N>::CircularBufferIterator(CircularBufferBase<T, N>* ptr, size_t idx) :
	_ptr(ptr),
	_idx(idx) { }

template <class T, size_t N>
CircularBufferIterator<T, N>::CircularBufferIterator(const CircularBufferIterator& other) :
	_ptr(other._ptr),
	_idx(other._idx) { }

template <class T, size_t N>
bool CircularBufferIterator<T, N>::valid() const {
	return (_ptr->front - _ptr->back) >= (*this - _ptr->back);
}

template <class T, size_t N>
const CircularBufferIterator<T, N>& CircularBufferIterator<T, N>::operator=(const CircularBufferIterator<T, N>& other) {
	_ptr = other._ptr;
	_idx = other._idx;
}

template <class T, size_t N>
CircularBufferIterator<T, N>& CircularBufferIterator<T, N>::operator++() {
	(*this)++;
	return *this;
}

template <class T, size_t N>
CircularBufferIterator<T, N> CircularBufferIterator<T, N>::operator++(int) {
	CircularBufferIterator<T, N> tmp = *this;
	if (_idx == N - 1) {
		_idx = 0;
	} else if (*this != _ptr->_front) {
		_idx = _idx + 1;
	}
	return tmp;
}

template <class T, size_t N>
CircularBufferIterator<T, N>& CircularBufferIterator<T, N>::operator--() {
	(*this)--;
	return *this;
}

template <class T, size_t N>
CircularBufferIterator<T, N> CircularBufferIterator<T, N>::operator--(int) {
	CircularBufferIterator<T, N> tmp = *this;
	if (_idx == 0) {
		_idx = N - 1;
	} else if (*this != _ptr->_back) {
		_idx = _idx - 1;
	}
	return tmp;
}

template <class T, size_t N>
bool CircularBufferIterator<T, N>::operator==(const CircularBufferIterator& other) const {
	return (_ptr == other._ptr) && (_idx == other._idx);
}

template <class T, size_t N>
bool CircularBufferIterator<T, N>::operator!=(const CircularBufferIterator& other) const {
	return !(*this == other);
}

template <class T, size_t N>
bool CircularBufferIterator<T, N>::operator>(const CircularBufferIterator<T, N>& other) const {
	return (*this - other > 0);
}

template <class T, size_t N>
bool CircularBufferIterator<T, N>::operator>=(const CircularBufferIterator<T, N>& other) const {
	return !(*this < other);
}

template <class T, size_t N>
bool CircularBufferIterator<T, N>::operator<(const CircularBufferIterator<T, N>& other) const {
	return (*this - other < 0);
}

template <class T, size_t N>
bool CircularBufferIterator<T, N>::operator<=(const CircularBufferIterator<T, N>& other) const {
	return !(*this > other);
}

template <class T, size_t N>
T& CircularBufferIterator<T, N>::operator*() {
	return _ptr->_arr[_idx];
}

template <class _T, size_t _N>
CircularBufferIterator<_T, _N> operator+(const CircularBufferIterator<_T, _N>& it, int n) {

	if (n < 0) {
		return it - (-n);
	}

	if (n >= it._ptr->num()) {
		n = it._ptr->num() - 1;
	}

	CircularBufferIterator<_T, _N> ret(it);
	ret._idx += n;
	if (ret._idx >= _N) {
		ret._idx -= _N;
	}
	return ret;
}

template <class _T, size_t _N>
CircularBufferIterator<_T, _N> operator+(int n, const CircularBufferIterator<_T, _N>& it) {
	return it + n;
}

template <class _T, size_t _N>
CircularBufferIterator<_T, _N> operator-(const CircularBufferIterator<_T, _N>& it, int n) {

	if (n < 0) {
		return it + (-n);
	}

	if (n >= it._ptr->num()) {
		n = it._ptr->num() - 1;
	}

	CircularBufferIterator<_T, _N> ret(it);
	if (ret._idx < n) {
		// since we are using unsigned, don't subtract or it will underflow
		ret._idx += _N;
	}
	ret._idx -= n;
	return ret;
}

template <class _T, size_t _N>
int operator-(const CircularBufferIterator<_T, _N>& it1, const CircularBufferIterator<_T, _N>& it2) {
	assert(it1->_ptr == it2->_ptr);

	int diff1;
	const CircularBufferIterator<_T, _N>& backIt = it1._ptr->_back;
	if (it1._idx < backIt._idx) {
		diff1 = it1._idx + (_N - 1 - backIt._idx);
	} else {
		diff1 = it1._idx - backIt._idx;
	}

	int diff2;
	if (it2._idx < backIt._idx) {
		diff2 = it2._idx + (_N - 1 - backIt._idx);
	} else {
		diff2 = it2._idx - backIt._idx;
	}

	return diff1 - diff2;
}

#endif /* _CIRCULAR_BUFFER_ITERATOR_HPP */
