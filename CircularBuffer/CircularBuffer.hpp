#ifndef _CIRCULAR_BUFFER_HPP
#define _CIRCULAR_BUFFER_HPP

#include <cassert>
#include <cstdint>
#include <cstring>

namespace Alectryon {

// define CIRCULAR_BUFFER_ITERATOR if iterators are being used
// #define CIRCULAR_BUFFER_ITERATOR

#ifdef CIRCULAR_BUFFER_ITERATOR
template <class T, bool C>
class CircularBufferIterator;
#endif /* CIRCULAR_BUFFER_ITERATOR */

template <class T>
class CircularBuffer {

#ifdef CIRCULAR_BUFFER_ITERATOR
friend CircularBufferIterator<T, true>;
friend CircularBufferIterator<T, false>;
#endif /* CIRCULAR_BUFFER_ITERATOR */

public:
	CircularBuffer(size_t capacity);

	CircularBuffer(const CircularBuffer<T>& other);

	CircularBuffer(CircularBuffer<T>&& other);

	CircularBuffer<T>& operator=(const CircularBuffer<T>& other);

	CircularBuffer<T>& operator=(CircularBuffer<T>&& other);

	~CircularBuffer();

	/**
	 * @brief Copies data from other to this one
	 * @details doesn't change the capacity of current buffer
	 * if other has more data than the capacity of current buffer
	 * it will only copy capacity() number of elements.
	 * @param other buffer to copy from
	 */
	void copy(const CircularBuffer<T>& other);

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
	 * @brief Clears the buffer of all elements
	 */
	void clear();

	/**
	 * @brief Returns capacity of buffer 
	 * @details the maximum number of elements this buffer can hold
	 */
	size_t capacity() const;
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

	bool operator==(const CircularBuffer<T>& other) const;
	bool operator!=(const CircularBuffer<T>& other) const;

#ifdef CIRCULAR_BUFFER_ITERATOR
	CircularBufferIterator<T, false> begin();
	CircularBufferIterator<T, false> end();
	CircularBufferIterator<T, true> cbegin();
	CircularBufferIterator<T, true> cend();
#endif /* CIRCULAR_BUFFER_ITERATOR */

protected:
	T* _arr;
	size_t _num;
	size_t _frontIdx;
	size_t _backIdx;
	size_t _capacity;

	inline size_t incrementIdx(size_t idx) const;
	inline size_t decrementIdx(size_t idx) const;
};

template <class T>
CircularBuffer<T>::CircularBuffer(size_t capacity) :
	_arr(nullptr),
	_num(0),
	_frontIdx(0),
	_backIdx(0),
	_capacity(capacity) {
	
	_arr = new T[_capacity];		
}

template <class T>
CircularBuffer<T>::CircularBuffer(const CircularBuffer<T>& other) :
	_capacity(other._capacity){
	_arr = new T[_capacity];
	copy(other);
}

template <class T>
CircularBuffer<T>::CircularBuffer(CircularBuffer<T>&& other) :
	_arr(other._arr),
	_num(other._num),
	_frontIdx(other._frontIdx),
	_backIdx(other._backIdx),
	_capacity(other._capacity) {

	other._arr = nullptr;
}

template <class T>
CircularBuffer<T>& CircularBuffer<T>::operator=(const CircularBuffer<T>& other) {
	if (_capacity != other._capacity) {
		_capacity = other._capacity;
		delete[] _arr;
		_arr = new T[_capacity];
	}
	copy(other);
	return *this;
}

template <class T>
CircularBuffer<T>& CircularBuffer<T>::operator=(CircularBuffer<T>&& other) {
	_num = other._num;
	_frontIdx = other._frontIdx;
	_backIdx = other._backIdx;
	_capacity = other._capacity;

	T* tempPtr = _arr;
	_arr = other._arr;
	other._arr = tempPtr;
	return *this;
}

template <class T>
CircularBuffer<T>::~CircularBuffer() {
	if (_arr != nullptr) {
		delete[] _arr;
	}
}

template <class T>
void CircularBuffer<T>::copy(const CircularBuffer<T>& other) {
	_num = other._num;
	if (_num > _capacity) {
		_num = _capacity;
	}
	_backIdx = 0;

	size_t thisIdx = _backIdx;
	size_t otherIdx = other._backIdx;
	for (size_t i = 0; i < _num; i++) {
		_arr[thisIdx] = other._arr[otherIdx];
		thisIdx = this->incrementIdx(thisIdx);
		otherIdx = other.incrementIdx(otherIdx);
	}
	_frontIdx = thisIdx;
	if (_frontIdx >= _capacity) {
		_frontIdx = 0;
	}
}

template <class T>
bool CircularBuffer<T>::pushBack(const T& val) {
	if (_num >= _capacity) {
		return false;
	}

	_backIdx = decrementIdx(_backIdx);
	_arr[_backIdx] = val;
	_num++;
	return true;
}

template <class T>
bool CircularBuffer<T>::pushFront(const T& val) {
	if (_num >= _capacity) {
		return false;
	}

	_arr[_frontIdx] = val;
	_frontIdx = incrementIdx(_frontIdx);
	_num++;
	return true;
}

template <class T>
bool CircularBuffer<T>::popBack() {
	if (_num == 0) {
		return false;
	}

	_backIdx = incrementIdx(_backIdx);
	_num--;
	return true;
}

template <class T>
bool CircularBuffer<T>::popFront() {
	if (_num == 0) {
		return false;
	}

	_frontIdx = decrementIdx(_frontIdx);
	_num--;
	return true;
}

template <class T>
T& CircularBuffer<T>::readBack() {
	return _arr[_backIdx];
}

template <class T>
T& CircularBuffer<T>::readFront() {
	size_t idx = decrementIdx(_frontIdx);
	return _arr[idx];
}

template <class T>
T& CircularBuffer<T>::readBack(size_t idx) {
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
T& CircularBuffer<T>::readFront(size_t idx) {
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
const T& CircularBuffer<T>::readBack() const {
	return readBack();
}

template <class T>
const T& CircularBuffer<T>::readFront() const {
	return readFront();
}

template <class T>
const T& CircularBuffer<T>::readBack(size_t idx) const {
	return readBack(idx);
}

template <class T>
const T& CircularBuffer<T>::readFront(size_t idx) const {
	return readFront(idx);
}

template <class T>
T CircularBuffer<T>::readBackCopy() const {
	return readBack();
}

template <class T>
T CircularBuffer<T>::readFrontCopy() const {
	return readFront();
}

template <class T>
T CircularBuffer<T>::readBackCopy(size_t idx) const {
	return readBack(idx);
}

template <class T>
T CircularBuffer<T>::readFrontCopy(size_t idx) const {
	return readFront(idx);
}

template <class T>
void CircularBuffer<T>::clear() {
	_num = 0;
	_frontIdx = _backIdx = 0;
}

template <class T>
size_t CircularBuffer<T>::capacity() const {
	return _capacity;
}

template <class T>
size_t CircularBuffer<T>::num() const {
	return _num;
}

template <class T>
bool CircularBuffer<T>::empty() const {
	return _num == 0;
}

template <class T>
bool CircularBuffer<T>::full() const {
	return _num == _capacity;
}

template <class T>
bool CircularBuffer<T>::operator==(const CircularBuffer<T>& other) const {
	if (_num != other._num) return false;

	size_t idx1 = _backIdx;
	size_t idx2 = other._backIdx;
	for (size_t i = 0; i < _num; i++) {
		if (_arr[idx1] != other._arr[idx2]) {
			return false;
		}

		idx1 = incrementIdx(idx1);
		idx2 = incrementIdx(idx2);
	}

	return true;
}

template <class T>
bool CircularBuffer<T>::operator!=(const CircularBuffer<T>& other) const {
	return !(*this == other);
}

#ifdef CIRCULAR_BUFFER_ITERATOR
template <class T>
CircularBufferIterator<T, false> CircularBuffer<T>::begin() {
	return CircularBufferIterator<T, false>(this, _backIdx, CircularBufferIterator<T, false>::BACK);
}

template <class T>
CircularBufferIterator<T, false> CircularBuffer<T>::end() {
	return CircularBufferIterator<T, false>(this, _frontIdx, CircularBufferIterator<T, false>::FRONT);
}

template <class T>
CircularBufferIterator<T, true> CircularBuffer<T>::cbegin() {
	return CircularBufferIterator<T, true>(this, _backIdx, CircularBufferIterator<T, true>::BACK);
}

template <class T>
CircularBufferIterator<T, true> CircularBuffer<T>::cend() {
	return CircularBufferIterator<T, true>(this, _frontIdx, CircularBufferIterator<T, true>::FRONT);
}
#endif /* CIRCULAR_BUFFER_ITERATOR */

template <class T>
inline size_t CircularBuffer<T>::incrementIdx(size_t idx) const {
	if (idx == _capacity - 1) {
		return 0;
	} else {
		return idx + 1;
	}
}

template <class T>
inline size_t CircularBuffer<T>::decrementIdx(size_t idx) const {
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
	CircularBufferIterator(CircularBuffer<T>* ptr);
	CircularBufferIterator(CircularBuffer<T>* ptr, size_t idx, IteratorState state);
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
	CircularBuffer<T>* _ptr;
	size_t _idx;
	IteratorState _state;
};

template <class T, bool C>
CircularBufferIterator<T, C>::CircularBufferIterator() :
	_ptr(nullptr),
	_idx(0),
	_state(BACK) { }

template <class T, bool C>
CircularBufferIterator<T, C>::CircularBufferIterator(CircularBuffer<T>* ptr) :
	_ptr(ptr),
	_idx(0),
	_state(BACK) { }

template <class T, bool C>
CircularBufferIterator<T, C>::CircularBufferIterator(CircularBuffer<T>* ptr, size_t idx, IteratorState state) :
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
		if (_idx == _ptr->_capacity - 1) {
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
			_idx = _ptr->_capacity - 1;
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
	if (ret._idx >= _ptr->_capacity) {
		ret._idx -= _ptr->_capacity;
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
		ret._idx += _ptr->_capacity;
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
		diff1 = _ptr->_capacity;
	} else {
		if (_idx < backIdx) {
			diff1 = _idx + (_ptr->_capacity - backIdx);
		} else {
			diff1 = _idx - backIdx;
		}
	}

	int diff2;
	if (other._state == FRONT) {
		diff2 = _ptr->_capacity;
	} else {
		if (other._idx < backIdx) {
			diff2 = other._idx + (_ptr->_capacity - backIdx);
		} else {
			diff2 = other._idx - backIdx;
		}
	}

	return diff1 - diff2;
}

#endif /* CIRCULAR_BUFFER_ITERATOR */

}

#endif /* _CIRCULAR_BUFFER_HPP */
