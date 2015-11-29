#ifndef _CIRCULAR_BUFFER_BASE_HPP
#define _CIRCULAR_BUFFER_BASE_HPP

#include <cassert>
#include <cstdint>
#include <cstring>

// define CIRCULAR_BUFFER_ITERATOR if iterators are being used
// #define CIRCULAR_BUFFER_ITERATOR

#ifdef CIRCULAR_BUFFER_ITERATOR
template <class T, size_t N, bool C>
class CircularBufferIterator;
#endif /* CIRCULAR_BUFFER_ITERATOR */

template <class T, size_t N>
class CircularBufferBase {

#ifdef CIRCULAR_BUFFER_ITERATOR
friend CircularBufferIterator<T, N, true>;
friend CircularBufferIterator<T, N, false>;
#endif /* CIRCULAR_BUFFER_ITERATOR */

public:
	void copy(const CircularBufferBase<T, N>& other);
	/**
	 * @brief Add a value to back of buffer
	 * @details value will be copied
	 * won't do anything if buffer is full
	 * @param val const reference to value
	 * @return true, if added successfully
	 */
	bool pushBack(const T& val);
	/**
	 * @brief Add a value to front of buffer
	 * @details value will be copied
	 * won't do anything if buffer is full
	 * @param val const reference to value
	 * @return true, if added successfully
	 */
	bool pushFront(const T& val);

	/**
	 * @brief Remove a value from back of buffer
	 * @details won't do anything if buffer is empty
	 * @return true, if removed successfully
	 */
	bool popBack();
	/**
	 * @brief Remove a value from front of buffer
	 * @details won't do anything if buffer is empty
	 * @return true, if removed successfully
	 */
	bool popFront();

	/**
	 * @brief Get reference to value at back of buffer
	 * @details if buffer is empty, behaviour is undefined
	 * @return reference to value at back of buffer
	 */
	T& readBack();
	/**
	 * @brief Get reference to value at front of buffer
	 * @details if buffer is empty, behaviour is undefined
	 * @return reference to value at front of buffer
	 */
	T& readFront();
	/**
	 * @brief Gets reference to a value offset from the back
	 * by idx
	 * @details an idx of 0 would be the same as readBack()
	 * has undefined behavior if idx >= num()
	 * @param idx offset from back of buffer, must be less than N
	 * @return const reference to value at sepcified index
	 */
	T& readBack(size_t idx);
	/**
	 * @brief Gets reference to a value offset from front by idx
	 * @details an idx of 0 would be the same as readFront()
	 * has undefined behavior if idx >= num()
	 * @param idx offset from back of buffer, must be less than N
	 * @return const reference to value at sepcified index
	 */
	T& readFront(size_t idx);
	const T& readBack() const;
	const T& readFront() const;
	const T& readBack(size_t idx) const;
	const T& readFront(size_t idx) const;
	T readBackCopy() const;
	T readFrontCopy() const;
	T readBackCopy(size_t idx) const;
	T readFrontCopy(size_t idx) const;

	/**
	 * @brief Get number of elements in buffer
	 */
	size_t num() const;
	/**
	 * @brief Returns true if buffer is empty
	 */
	bool empty() const;
	/**
	 * @brief Returns true if buffer is full
	 */
	bool full() const;

	bool operator==(const CircularBufferBase<T, N>& other) const;
	bool operator!=(const CircularBufferBase<T, N>& other) const;

#ifdef CIRCULAR_BUFFER_ITERATOR
	CircularBufferIterator<T, N, false> begin();
	CircularBufferIterator<T, N, false> end();
	CircularBufferIterator<T, N, true> cbegin();
	CircularBufferIterator<T, N, true> cend();
#endif /* CIRCULAR_BUFFER_ITERATOR */

protected:
	/**
	 * @brief Ctor for CircularBufferBase
	 */
	CircularBufferBase();

	CircularBufferBase(const CircularBufferBase<T, N>& other) = delete;
	CircularBufferBase<T, N>& operator=(const CircularBufferBase<T, N>& other) = delete;
	CircularBufferBase(CircularBufferBase<T, N>&& other) = delete;
	CircularBufferBase<T, N>& operator=(CircularBufferBase<T, N>&& other) = delete;

	T* _arr;
	size_t _num;
	size_t _frontIdx;
	size_t _backIdx;

	inline static size_t incrementIdx(size_t idx);
	inline static size_t decrementIdx(size_t idx);
};

template <class T, size_t N>
void CircularBufferBase<T, N>::copy(const CircularBufferBase<T, N>& other) {
	_num = other._num;
	_frontIdx = other._frontIdx;
	_backIdx = other._backIdx;

	size_t spaceUntilEnd = N - _backIdx;
	if (spaceUntilEnd < _num) {
		for (size_t idx = _backIdx; idx < N; idx++) {
			_arr[idx] = other._arr[idx];
		}
		size_t spaceLeft = _num - spaceUntilEnd;
		for (size_t idx = 0; idx < spaceLeft; idx++) {
			_arr[idx] = other._arr[idx];
		}
	} else {
		for (size_t idx = _backIdx; idx < _backIdx + _num; idx++) {
			_arr[idx] = other._arr[idx];
		}
	}
}

template <class T, size_t N>
bool CircularBufferBase<T, N>::pushBack(const T& val) {
	if (_num >= N) {
		return false;
	}

	_backIdx = decrementIdx(_backIdx);
	_arr[_backIdx] = val;
	_num++;
	return true;
}

template <class T, size_t N>
bool CircularBufferBase<T, N>::pushFront(const T& val) {
	if (_num >= N) {
		return false;
	}

	_arr[_frontIdx] = val;
	_frontIdx = incrementIdx(_frontIdx);
	_num++;
	return true;
}

template <class T, size_t N>
bool CircularBufferBase<T, N>::popBack() {
	if (_num == 0) {
		return false;
	}

	_backIdx = incrementIdx(_backIdx);
	_num--;
	return true;
}

template <class T, size_t N>
bool CircularBufferBase<T, N>::popFront() {
	if (_num == 0) {
		return false;
	}

	_frontIdx = decrementIdx(_frontIdx);
	_num--;
	return true;
}

template <class T, size_t N>
T& CircularBufferBase<T, N>::readBack() {
	return _arr[_backIdx];
}

template <class T, size_t N>
T& CircularBufferBase<T, N>::readFront() {
	size_t idx = decrementIdx(_frontIdx);
	return _arr[idx];
}

template <class T, size_t N>
T& CircularBufferBase<T, N>::readBack(size_t idx) {
	if (idx >= N) {
		idx = N - 1;
	}
	idx = _backIdx + idx;
	if (idx >= N) {
		idx -= N;
	}
	return _arr[idx];
}

template <class T, size_t N>
T& CircularBufferBase<T, N>::readFront(size_t idx) {
	if (idx >= N) {
		idx = N - 1;
	}

	if (_frontIdx < idx) {
		// since we are using unsigned, don't subtract or it will underflow
		idx = (_frontIdx + N) - idx;
	} else {
		idx = _frontIdx - idx;
	}
	idx = decrementIdx(idx);
	return _arr[idx];
}

template <class T, size_t N>
const T& CircularBufferBase<T, N>::readBack() const {
	return readBack();
}

template <class T, size_t N>
const T& CircularBufferBase<T, N>::readFront() const {
	return readFront();
}

template <class T, size_t N>
const T& CircularBufferBase<T, N>::readBack(size_t idx) const {
	return readBack(idx);
}

template <class T, size_t N>
const T& CircularBufferBase<T, N>::readFront(size_t idx) const {
	return readFront(idx);
}

template <class T, size_t N>
T CircularBufferBase<T, N>::readBackCopy() const {
	return readBack();
}

template <class T, size_t N>
T CircularBufferBase<T, N>::readFrontCopy() const {
	return readFront();
}

template <class T, size_t N>
T CircularBufferBase<T, N>::readBackCopy(size_t idx) const {
	return readBack(idx);
}

template <class T, size_t N>
T CircularBufferBase<T, N>::readFrontCopy(size_t idx) const {
	return readFront(idx);
}

template <class T, size_t N>
size_t CircularBufferBase<T, N>::num() const {
	return _num;
}

template <class T, size_t N>
bool CircularBufferBase<T, N>::empty() const {
	return _num == 0;
}

template <class T, size_t N>
bool CircularBufferBase<T, N>::full() const {
	return _num == N;
}

template <class T, size_t N>
bool CircularBufferBase<T, N>::operator==(const CircularBufferBase<T, N>& other) const {
	if (_num != other._num) return false;

	size_t idx1 = _backIdx;
	size_t idx2 = other._backIdx;
	for (int i = 0; i < _num; i++) {
		if (_arr[idx1] != other._arr[idx2]) {
			return false;
		}

		idx1 = incrementIdx(idx1);
		idx2 = incrementIdx(idx2);
	}

	return true;
}

template <class T, size_t N>
bool CircularBufferBase<T, N>::operator!=(const CircularBufferBase<T, N>& other) const {
	return !(*this == other);
}

#ifdef CIRCULAR_BUFFER_ITERATOR
template <class T, size_t N>
CircularBufferIterator<T, N, false> CircularBufferBase<T, N>::begin() {
	return CircularBufferIterator<T, N, false>(this, _backIdx, CircularBufferIterator<T, N, false>::BACK);
}

template <class T, size_t N>
CircularBufferIterator<T, N, false> CircularBufferBase<T, N>::end() {
	return CircularBufferIterator<T, N, false>(this, _frontIdx, CircularBufferIterator<T, N, false>::FRONT);
}

template <class T, size_t N>
CircularBufferIterator<T, N, true> CircularBufferBase<T, N>::cbegin() {
	return CircularBufferIterator<T, N, true>(this, _backIdx, CircularBufferIterator<T, N, true>::BACK);
}

template <class T, size_t N>
CircularBufferIterator<T, N, true> CircularBufferBase<T, N>::cend() {
	return CircularBufferIterator<T, N, true>(this, _frontIdx, CircularBufferIterator<T, N, true>::FRONT);
}
#endif /* CIRCULAR_BUFFER_ITERATOR */

template <class T, size_t N>
CircularBufferBase<T, N>::CircularBufferBase() :
	_arr(nullptr),
	_num(0),
	_frontIdx(0),
	_backIdx(0) { }

template <class T, size_t N>
inline size_t CircularBufferBase<T, N>::incrementIdx(size_t idx) {
	if (idx == N - 1) {
		return 0;
	} else {
		return idx + 1;
	}
}

template <class T, size_t N>
inline size_t CircularBufferBase<T, N>::decrementIdx(size_t idx) {
	if (idx == 0) {
		return N - 1;
	} else {
		return idx - 1;
	}
}

#ifdef CIRCULAR_BUFFER_ITERATOR

template <class T, size_t N, bool C>
class CircularBufferIterator {
private:
	typedef typename std::conditional<C, const T&, T&>::type ReferenceType;
public:
	enum IteratorState { BACK, FRONT, MIDDLE };
	CircularBufferIterator();
	CircularBufferIterator(CircularBufferBase<T, N>* ptr);
	CircularBufferIterator(CircularBufferBase<T, N>* ptr, size_t idx, IteratorState state);
	CircularBufferIterator(const CircularBufferIterator<T, N, C>& other);

	bool valid() const;

	const CircularBufferIterator<T, N, C>& operator=(const CircularBufferIterator<T, N, C>& other);
	CircularBufferIterator<T, N, C>& operator++();
	CircularBufferIterator<T, N, C> operator++(int);
	CircularBufferIterator<T, N, C>& operator--();
	CircularBufferIterator<T, N, C> operator--(int);
	bool operator==(const CircularBufferIterator<T, N, C>& other) const;
	bool operator!=(const CircularBufferIterator<T, N, C>& other) const;
	bool operator>(const CircularBufferIterator<T, N, C>& other) const;
	bool operator>=(const CircularBufferIterator<T, N, C>& other) const;
	bool operator<(const CircularBufferIterator<T, N, C>& other) const;
	bool operator<=(const CircularBufferIterator<T, N, C>& other) const;
	ReferenceType operator*();
	ReferenceType operator[](size_t idx);
	CircularBufferIterator<T, N, C> operator+(int n);
	CircularBufferIterator<T, N, C> operator-(int n);
	int operator-(const CircularBufferIterator<T, N, C>& other) const;

protected:
	CircularBufferBase<T, N>* _ptr;
	size_t _idx;
	IteratorState _state;
};

template <class T, size_t N, bool C>
CircularBufferIterator<T, N, C>::CircularBufferIterator() :
	_ptr(nullptr),
	_idx(0),
	_state(BACK) { }

template <class T, size_t N, bool C>
CircularBufferIterator<T, N, C>::CircularBufferIterator(CircularBufferBase<T, N>* ptr) :
	_ptr(ptr),
	_idx(0),
	_state(BACK) { }

template <class T, size_t N, bool C>
CircularBufferIterator<T, N, C>::CircularBufferIterator(CircularBufferBase<T, N>* ptr, size_t idx, IteratorState state) :
	_ptr(ptr),
	_idx(idx),
	_state(state) { }

template <class T, size_t N, bool C>
CircularBufferIterator<T, N, C>::CircularBufferIterator(const CircularBufferIterator<T, N, C>& other) :
	_ptr(other._ptr),
	_idx(other._idx),
	_state(other._state) { }

template <class T, size_t N, bool C>
bool CircularBufferIterator<T, N, C>::valid() const {
	return (_ptr->front - _ptr->back) >= (*this - _ptr->back);
}

template <class T, size_t N, bool C>
const CircularBufferIterator<T, N, C>& CircularBufferIterator<T, N, C>::operator=(const CircularBufferIterator<T, N, C>& other) {
	_ptr = other._ptr;
	_idx = other._idx;
}

template <class T, size_t N, bool C>
CircularBufferIterator<T, N, C>& CircularBufferIterator<T, N, C>::operator++() {
	(*this)++;
	return *this;
}

template <class T, size_t N, bool C>
CircularBufferIterator<T, N, C> CircularBufferIterator<T, N, C>::operator++(int) {
	CircularBufferIterator<T, N, C> tmp = *this;

	if (_idx != _ptr->_frontIdx || _state == BACK) {
		if (_idx == N - 1) {
			_idx = 0;
		} else {
			_idx++;
		}

		if (_idx == _ptr->_frontIdx) {
			_state = FRONT; 
		} else {
			_state = MIDDLE;
		}
	}

	return tmp;
}

template <class T, size_t N, bool C>
CircularBufferIterator<T, N, C>& CircularBufferIterator<T, N, C>::operator--() {
	(*this)--;
	return *this;
}

template <class T, size_t N, bool C>
CircularBufferIterator<T, N, C> CircularBufferIterator<T, N, C>::operator--(int) {
	CircularBufferIterator<T, N, C> tmp = *this;

	if (_idx != _ptr->_backIdx || _state == FRONT) {
		if (_idx == 0) {
			_idx = N - 1;
		} else {
			_idx--;
		}

		if (_idx == _ptr->_backIdx) {
			_state = BACK;
		} else {
			_state = MIDDLE;
		}
	}

	return tmp;
}

template <class T, size_t N, bool C>
bool CircularBufferIterator<T, N, C>::operator==(const CircularBufferIterator& other) const {
	return (_ptr == other._ptr) && (_idx == other._idx) && (_state == other._state);
}

template <class T, size_t N, bool C>
bool CircularBufferIterator<T, N, C>::operator!=(const CircularBufferIterator& other) const {
	return !(*this == other);
}

template <class T, size_t N, bool C>
bool CircularBufferIterator<T, N, C>::operator>(const CircularBufferIterator<T, N, C>& other) const {
	return (*this - other > 0);
}

template <class T, size_t N, bool C>
bool CircularBufferIterator<T, N, C>::operator>=(const CircularBufferIterator<T, N, C>& other) const {
	return !(*this < other);
}

template <class T, size_t N, bool C>
bool CircularBufferIterator<T, N, C>::operator<(const CircularBufferIterator<T, N, C>& other) const {
	return (*this - other < 0);
}

template <class T, size_t N, bool C>
bool CircularBufferIterator<T, N, C>::operator<=(const CircularBufferIterator<T, N, C>& other) const {
	return !(*this > other);
}

template <class T, size_t N, bool C>
typename CircularBufferIterator<T, N, C>::ReferenceType CircularBufferIterator<T, N, C>::operator*() {
	return _ptr->_arr[_idx];
}

template <class T, size_t N, bool C>
typename CircularBufferIterator<T, N, C>::ReferenceType CircularBufferIterator<T, N, C>::operator[](size_t idx) {
	CircularBufferIterator<T, N, C> temp = *this + idx;
	return *temp;
}

template <class T, size_t N, bool C>
CircularBufferIterator<T, N, C> CircularBufferIterator<T, N, C>::operator+(int n) {
	if (n < 0) {
		return *this - (-n);
	}

	if (n >= _ptr->_num) {
		n = _ptr->_num - 1;
	}

	CircularBufferIterator<T, N, C> ret(*this);
	ret._idx += n;
	if (ret._idx >= N) {
		ret._idx -= N;
	}
	return ret;
}

template <class T, size_t N, bool C>
CircularBufferIterator<T, N, C> CircularBufferIterator<T, N, C>::operator-(int n) {
	if (n < 0) {
		return *this + (-n);
	}

	if (n > _ptr->_num) {
		n = _ptr->_num;
	}
	CircularBufferIterator<T, N, C> ret(*this);
	if (ret._idx < n) {
		// since we are using unsigned, don't subtract or it will underflow
		ret._idx += N;
	}
	ret._idx -= n;
	return ret;
}

template <class T, size_t N, bool C>
int CircularBufferIterator<T, N, C>::operator-(const CircularBufferIterator<T, N, C>& other) const {
	assert(_ptr == other._ptr);

	int diff1;
	size_t backIdx = _ptr->_backIdx;
	if (_state == FRONT) {
		diff1 = N;
	} else {
		if (_idx < backIdx) {
			diff1 = _idx + (N - backIdx);
		} else {
			diff1 = _idx - backIdx;
		}
	}

	int diff2;
	if (other._state == FRONT) {
		diff2 = N;
	} else {
		if (other._idx < backIdx) {
			diff2 = other._idx + (N - backIdx);
		} else {
			diff2 = other._idx - backIdx;
		}
	}

	return diff1 - diff2;
}

#endif /* CIRCULAR_BUFFER_ITERATOR */

#endif /* _CIRCULAR_BUFFER_BASE_HPP */
