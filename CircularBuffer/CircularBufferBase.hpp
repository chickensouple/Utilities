#ifndef _CIRCULAR_BUFFER_BASE_HPP
#define _CIRCULAR_BUFFER_BASE_HPP

#include <cassert>
#include <cstdint>
#include <cstring>

// define CIRCULAR_BUFFER_ITERATOR if iterators are being used
// #define CIRCULAR_BUFFER_ITERATOR

#ifdef CIRCULAR_BUFFER_ITERATOR
template <class T, bool C>
class CircularBufferIterator;
#endif /* CIRCULAR_BUFFER_ITERATOR */

template <class T>
class CircularBufferBase {

#ifdef CIRCULAR_BUFFER_ITERATOR
friend CircularBufferIterator<T, true>;
friend CircularBufferIterator<T, false>;
#endif /* CIRCULAR_BUFFER_ITERATOR */

public:
	void copy(const CircularBufferBase<T>& other);
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
	 * @param idx offset from back of buffer, must be less than capacity()
	 * @return const reference to value at sepcified index
	 */
	T& readBack(size_t idx);
	/**
	 * @brief Gets reference to a value offset from front by idx
	 * @details an idx of 0 would be the same as readFront()
	 * has undefined behavior if idx >= num()
	 * @param idx offset from back of buffer, must be less than _capacity
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

	bool operator==(const CircularBufferBase<T>& other) const;
	bool operator!=(const CircularBufferBase<T>& other) const;

#ifdef CIRCULAR_BUFFER_ITERATOR
	CircularBufferIterator<T, false> begin();
	CircularBufferIterator<T, false> end();
	CircularBufferIterator<T, true> cbegin();
	CircularBufferIterator<T, true> cend();
#endif /* CIRCULAR_BUFFER_ITERATOR */

protected:
	/**
	 * @brief Ctor for CircularBufferBase
	 */
	CircularBufferBase(size_t capacity);

	CircularBufferBase(const CircularBufferBase<T>& other) = delete;
	CircularBufferBase<T>& operator=(const CircularBufferBase<T>& other) = delete;
	CircularBufferBase(CircularBufferBase<T>&& other) = delete;
	CircularBufferBase<T>& operator=(CircularBufferBase<T>&& other) = delete;

	T* _arr;
	size_t _num;
	size_t _frontIdx;
	size_t _backIdx;
	size_t _capacity;

	inline size_t incrementIdx(size_t idx);
	inline size_t decrementIdx(size_t idx);
};

template <class T>
void CircularBufferBase<T>::copy(const CircularBufferBase<T>& other) {
	_num = other._num;
	if (_num > _capacity) {
		_num = _capacity;
	}

	size_t idx;
	for (idx = 0; idx < _num; idx++) {
		_arr[idx] = other._arr[idx];
	}
	_backIdx = 0;
	_frontIdx = idx;
	// take care of boundary case that _num == _capacity
	if (_frontIdx >= _capacity) {
		_frontIdx = 0;
	}
}

template <class T>
bool CircularBufferBase<T>::pushBack(const T& val) {
	if (_num >= _capacity) {
		return false;
	}

	_backIdx = decrementIdx(_backIdx);
	_arr[_backIdx] = val;
	_num++;
	return true;
}

template <class T>
bool CircularBufferBase<T>::pushFront(const T& val) {
	if (_num >= _capacity) {
		return false;
	}

	_arr[_frontIdx] = val;
	_frontIdx = incrementIdx(_frontIdx);
	_num++;
	return true;
}

template <class T>
bool CircularBufferBase<T>::popBack() {
	if (_num == 0) {
		return false;
	}

	_backIdx = incrementIdx(_backIdx);
	_num--;
	return true;
}

template <class T>
bool CircularBufferBase<T>::popFront() {
	if (_num == 0) {
		return false;
	}

	_frontIdx = decrementIdx(_frontIdx);
	_num--;
	return true;
}

template <class T>
T& CircularBufferBase<T>::readBack() {
	return _arr[_backIdx];
}

template <class T>
T& CircularBufferBase<T>::readFront() {
	size_t idx = decrementIdx(_frontIdx);
	return _arr[idx];
}

template <class T>
T& CircularBufferBase<T>::readBack(size_t idx) {
	if (idx >= _capacity) {
		idx = _capacity - 1;
	}
	idx = _backIdx + idx;
	if (idx >= _capacity) {
		idx -= _capacity;
	}
	return _arr[idx];
}

template <class T>
T& CircularBufferBase<T>::readFront(size_t idx) {
	if (idx >= _capacity) {
		idx = _capacity - 1;
	}

	if (_frontIdx < idx) {
		// since we are using unsigned, don't subtract or it will underflow
		idx = (_frontIdx + _capacity) - idx;
	} else {
		idx = _frontIdx - idx;
	}
	idx = decrementIdx(idx);
	return _arr[idx];
}

template <class T>
const T& CircularBufferBase<T>::readBack() const {
	return readBack();
}

template <class T>
const T& CircularBufferBase<T>::readFront() const {
	return readFront();
}

template <class T>
const T& CircularBufferBase<T>::readBack(size_t idx) const {
	return readBack(idx);
}

template <class T>
const T& CircularBufferBase<T>::readFront(size_t idx) const {
	return readFront(idx);
}

template <class T>
T CircularBufferBase<T>::readBackCopy() const {
	return readBack();
}

template <class T>
T CircularBufferBase<T>::readFrontCopy() const {
	return readFront();
}

template <class T>
T CircularBufferBase<T>::readBackCopy(size_t idx) const {
	return readBack(idx);
}

template <class T>
T CircularBufferBase<T>::readFrontCopy(size_t idx) const {
	return readFront(idx);
}

template <class T>
size_t CircularBufferBase<T>::num() const {
	return _num;
}

template <class T>
bool CircularBufferBase<T>::empty() const {
	return _num == 0;
}

template <class T>
bool CircularBufferBase<T>::full() const {
	return _num == _capacity;
}

template <class T>
bool CircularBufferBase<T>::operator==(const CircularBufferBase<T>& other) const {
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

template <class T>
bool CircularBufferBase<T>::operator!=(const CircularBufferBase<T>& other) const {
	return !(*this == other);
}

#ifdef CIRCULAR_BUFFER_ITERATOR
template <class T>
CircularBufferIterator<T, false> CircularBufferBase<T>::begin() {
	return CircularBufferIterator<T, false>(this, _backIdx, CircularBufferIterator<T, false>::BACK);
}

template <class T>
CircularBufferIterator<T, false> CircularBufferBase<T>::end() {
	return CircularBufferIterator<T, false>(this, _frontIdx, CircularBufferIterator<T, false>::FRONT);
}

template <class T>
CircularBufferIterator<T, true> CircularBufferBase<T>::cbegin() {
	return CircularBufferIterator<T, true>(this, _backIdx, CircularBufferIterator<T, true>::BACK);
}

template <class T>
CircularBufferIterator<T, true> CircularBufferBase<T>::cend() {
	return CircularBufferIterator<T, true>(this, _frontIdx, CircularBufferIterator<T, true>::FRONT);
}
#endif /* CIRCULAR_BUFFER_ITERATOR */

template <class T>
CircularBufferBase<T>::CircularBufferBase(size_t capacity) :
	_arr(nullptr),
	_num(0),
	_frontIdx(0),
	_backIdx(0),
	_capacity(capacity) { }

template <class T>
inline size_t CircularBufferBase<T>::incrementIdx(size_t idx) {
	if (idx == _capacity - 1) {
		return 0;
	} else {
		return idx + 1;
	}
}

template <class T>
inline size_t CircularBufferBase<T>::decrementIdx(size_t idx) {
	if (idx == 0) {
		return _capacity - 1;
	} else {
		return idx - 1;
	}
}

#ifdef CIRCULAR_BUFFER_ITERATOR

template <class T, bool C>
class CircularBufferIterator {
private:
	typedef typename std::conditional<C, const T&, T&>::type ReferenceType;
public:
	enum IteratorState { BACK, FRONT, MIDDLE };
	CircularBufferIterator();
	CircularBufferIterator(CircularBufferBase<T>* ptr);
	CircularBufferIterator(CircularBufferBase<T>* ptr, size_t idx, IteratorState state);
	CircularBufferIterator(const CircularBufferIterator<T, C>& other);

	bool valid() const;

	const CircularBufferIterator<T, C>& operator=(const CircularBufferIterator<T, C>& other);
	CircularBufferIterator<T, C>& operator++();
	CircularBufferIterator<T, C> operator++(int);
	CircularBufferIterator<T, C>& operator--();
	CircularBufferIterator<T, C> operator--(int);
	bool operator==(const CircularBufferIterator<T, C>& other) const;
	bool operator!=(const CircularBufferIterator<T, C>& other) const;
	bool operator>(const CircularBufferIterator<T, C>& other) const;
	bool operator>=(const CircularBufferIterator<T, C>& other) const;
	bool operator<(const CircularBufferIterator<T, C>& other) const;
	bool operator<=(const CircularBufferIterator<T, C>& other) const;
	ReferenceType operator*();
	ReferenceType operator[](size_t idx);
	CircularBufferIterator<T, C> operator+(int n);
	CircularBufferIterator<T, C> operator-(int n);
	int operator-(const CircularBufferIterator<T, C>& other) const;

protected:
	CircularBufferBase<T>* _ptr;
	size_t _idx;
	IteratorState _state;
};

template <class T, bool C>
CircularBufferIterator<T, C>::CircularBufferIterator() :
	_ptr(nullptr),
	_idx(0),
	_state(BACK) { }

template <class T, bool C>
CircularBufferIterator<T, C>::CircularBufferIterator(CircularBufferBase<T>* ptr) :
	_ptr(ptr),
	_idx(0),
	_state(BACK) { }

template <class T, bool C>
CircularBufferIterator<T, C>::CircularBufferIterator(CircularBufferBase<T>* ptr, size_t idx, IteratorState state) :
	_ptr(ptr),
	_idx(idx),
	_state(state) { }

template <class T, bool C>
CircularBufferIterator<T, C>::CircularBufferIterator(const CircularBufferIterator<T, C>& other) :
	_ptr(other._ptr),
	_idx(other._idx),
	_state(other._state) { }

template <class T, bool C>
bool CircularBufferIterator<T, C>::valid() const {
	return (_ptr->front - _ptr->back) >= (*this - _ptr->back);
}

template <class T, bool C>
const CircularBufferIterator<T, C>& CircularBufferIterator<T, C>::operator=(const CircularBufferIterator<T, C>& other) {
	_ptr = other._ptr;
	_idx = other._idx;
}

template <class T, bool C>
CircularBufferIterator<T, C>& CircularBufferIterator<T, C>::operator++() {
	(*this)++;
	return *this;
}

template <class T, bool C>
CircularBufferIterator<T, C> CircularBufferIterator<T, C>::operator++(int) {
	CircularBufferIterator<T, C> tmp = *this;

	if (_idx != _ptr->_frontIdx || _state == BACK) {
		if (_idx == _capacity - 1) {
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

template <class T, bool C>
CircularBufferIterator<T, C>& CircularBufferIterator<T, C>::operator--() {
	(*this)--;
	return *this;
}

template <class T, bool C>
CircularBufferIterator<T, C> CircularBufferIterator<T, C>::operator--(int) {
	CircularBufferIterator<T, C> tmp = *this;

	if (_idx != _ptr->_backIdx || _state == FRONT) {
		if (_idx == 0) {
			_idx = _capacity - 1;
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

template <class T, bool C>
bool CircularBufferIterator<T, C>::operator==(const CircularBufferIterator& other) const {
	return (_ptr == other._ptr) && (_idx == other._idx) && (_state == other._state);
}

template <class T, bool C>
bool CircularBufferIterator<T, C>::operator!=(const CircularBufferIterator& other) const {
	return !(*this == other);
}

template <class T, bool C>
bool CircularBufferIterator<T, C>::operator>(const CircularBufferIterator<T, C>& other) const {
	return (*this - other > 0);
}

template <class T, bool C>
bool CircularBufferIterator<T, C>::operator>=(const CircularBufferIterator<T, C>& other) const {
	return !(*this < other);
}

template <class T, bool C>
bool CircularBufferIterator<T, C>::operator<(const CircularBufferIterator<T, C>& other) const {
	return (*this - other < 0);
}

template <class T, bool C>
bool CircularBufferIterator<T, C>::operator<=(const CircularBufferIterator<T, C>& other) const {
	return !(*this > other);
}

template <class T, bool C>
typename CircularBufferIterator<T, C>::ReferenceType CircularBufferIterator<T, C>::operator*() {
	return _ptr->_arr[_idx];
}

template <class T, bool C>
typename CircularBufferIterator<T, C>::ReferenceType CircularBufferIterator<T, C>::operator[](size_t idx) {
	CircularBufferIterator<T, C> temp = *this + idx;
	return *temp;
}

template <class T, bool C>
CircularBufferIterator<T, C> CircularBufferIterator<T, C>::operator+(int n) {
	if (n < 0) {
		return *this - (-n);
	}

	if (n >= _ptr->_num) {
		n = _ptr->_num - 1;
	}

	CircularBufferIterator<T, C> ret(*this);
	ret._idx += n;
	if (ret._idx >= _capacity) {
		ret._idx -= _capacity;
	}
	return ret;
}

template <class T, bool C>
CircularBufferIterator<T, C> CircularBufferIterator<T, C>::operator-(int n) {
	if (n < 0) {
		return *this + (-n);
	}

	if (n > _ptr->_num) {
		n = _ptr->_num;
	}
	CircularBufferIterator<T, C> ret(*this);
	if (ret._idx < n) {
		// since we are using unsigned, don't subtract or it will underflow
		ret._idx += _capacity;
	}
	ret._idx -= n;
	return ret;
}

template <class T, bool C>
int CircularBufferIterator<T, C>::operator-(const CircularBufferIterator<T, C>& other) const {
	assert(_ptr == other._ptr);

	int diff1;
	size_t backIdx = _ptr->_backIdx;
	if (_state == FRONT) {
		diff1 = _capacity;
	} else {
		if (_idx < backIdx) {
			diff1 = _idx + (_capacity - backIdx);
		} else {
			diff1 = _idx - backIdx;
		}
	}

	int diff2;
	if (other._state == FRONT) {
		diff2 = _capacity;
	} else {
		if (other._idx < backIdx) {
			diff2 = other._idx + (_capacity - backIdx);
		} else {
			diff2 = other._idx - backIdx;
		}
	}

	return diff1 - diff2;
}

#endif /* CIRCULAR_BUFFER_ITERATOR */

#endif /* _CIRCULAR_BUFFER_BASE_HPP */
