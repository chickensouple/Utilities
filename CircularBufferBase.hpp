#ifndef _CIRCULAR_BUFFER_BASE_HPP
#define _CIRCULAR_BUFFER_BASE_HPP

#include <cassert>
#include <cstdint>
#include <cstring>

template <class T, size_t N>
class CircularBufferIterator;

template <class T, size_t N>
class CircularBufferBase {
friend CircularBufferIterator<T, N>;
public:
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

	CircularBufferIterator<T, N> begin() const;
	CircularBufferIterator<T, N> end() const;

	bool operator==(const CircularBufferBase<T, N>& other) const;
	bool operator!=(const CircularBufferBase<T, N>& other) const;
protected:
	/**
	 * @brief Ctor for CircularBufferBase
	 */
	CircularBufferBase();

	CircularBufferBase(const CircularBufferBase<T, N>& other);

	T* _arr;
	size_t _num;
	size_t _frontIdx;
	size_t _backIdx;

	static size_t incrementIdx(size_t idx);
	static size_t decrementIdx(size_t idx);	
};

template <class T, size_t N>
class CircularBufferIterator {
public:
	CircularBufferIterator();
	CircularBufferIterator(const CircularBufferBase<T, N>* ptr);
	CircularBufferIterator(const CircularBufferBase<T, N>* ptr, size_t idx);
	CircularBufferIterator(const CircularBufferIterator& other);

	const CircularBufferIterator& operator=(const CircularBufferIterator& other);
	CircularBufferIterator& operator++();
	CircularBufferIterator operator++(int);
	CircularBufferIterator& operator--();
	CircularBufferIterator operator--(int);
	bool operator==(const CircularBufferIterator& other);
	bool operator!=(const CircularBufferIterator& other);
	T& operator*();

	template <class _T, size_t _N>
	friend CircularBufferIterator<_T, _N> operator+(const CircularBufferIterator<_T, _N>& it, int n);

	template <class _T, size_t _N>
	friend CircularBufferIterator<_T, _N> operator-(const CircularBufferIterator<_T, _N>& it, int n);

private:
	CircularBufferBase<T, N>* _ptr;
	size_t _idx;
};

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
	size_t frontValIdx = decrementIdx(_frontIdx);
	return _arr[frontValIdx];
}

template <class T, size_t N>
T& CircularBufferBase<T, N>::readBack(size_t idx) {
	assert(idx < N);
	idx = _backIdx + idx;
	if (idx >= N) {
		idx -= N;
	}
	return _arr[idx];
}

template <class T, size_t N>
T& CircularBufferBase<T, N>::readFront(size_t idx) {
	assert(idx < N);
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
CircularBufferIterator<T, N> CircularBufferBase<T, N>::begin() const {
	return CircularBufferIterator<T, N>(this, _backIdx);
}

template <class T, size_t N>
CircularBufferIterator<T, N> CircularBufferBase<T, N>::end() const {
	return CircularBufferIterator<T, N>(this, _frontIdx);
}

template <class T, size_t N>
bool CircularBufferBase<T, N>::operator==(const CircularBufferBase<T, N>& other) const {
	if (_num != other._num) return false;
	CircularBufferIterator<T, N> it1 = this->begin();
	CircularBufferIterator<T, N> it2 = other.begin();
	while (it1 != this->end()) {
		if ((*it1)++ != (*it2)++) {
			return false;
		}
	}

	return true;
}

template <class T, size_t N>
bool CircularBufferBase<T, N>::operator!=(const CircularBufferBase<T, N>& other) const {
	return !(*this == other);
}

template <class T, size_t N>
CircularBufferBase<T, N>::CircularBufferBase() :
	_arr(nullptr),
	_num(0),
	_frontIdx(0),
	_backIdx(0) { }

template <class T, size_t N>
CircularBufferBase<T, N>::CircularBufferBase(const CircularBufferBase<T, N>& other) :
	_num(other.num),
	_frontIdx(other._frontIdx),
	_backIdx(other._backIdx) {
	std::memcpy(_arr, other._arr, N * sizeof(T));
}

template <class T, size_t N>
size_t CircularBufferBase<T, N>::incrementIdx(size_t idx) {
	if (idx == N - 1) {
		return 0;
	} else {
		return idx + 1;
	}
}

template <class T, size_t N>
size_t CircularBufferBase<T, N>::decrementIdx(size_t idx) {
	if (idx == 0) {
		return N - 1;
	} else {
		return idx - 1;
	}
}

template <class T, size_t N>
CircularBufferIterator<T, N>::CircularBufferIterator() :
	_ptr(nullptr),
	_idx(0) { }

template <class T, size_t N>
CircularBufferIterator<T, N>::CircularBufferIterator(const CircularBufferBase<T, N>* ptr) :
	_ptr(ptr),
	_idx(0) { }

template <class T, size_t N>
CircularBufferIterator<T, N>::CircularBufferIterator(const CircularBufferBase<T, N>* ptr, size_t idx) :
	_ptr(ptr),
	_idx(idx) { }

template <class T, size_t N>
CircularBufferIterator<T, N>::CircularBufferIterator(const CircularBufferIterator& other) :
	_ptr(other._ptr),
	_idx(other._idx) { }

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
	(*this)--;
	return *this;
}

template <class T, size_t N>
CircularBufferIterator<T, N>& CircularBufferIterator<T, N>::operator--() {
	CircularBufferIterator<T, N> tmp = *this;
	if (_idx == N - 1) {
		_idx = 0;
	} else {
		_idx = _idx + 1;
	}
	return tmp;
}

template <class T, size_t N>
CircularBufferIterator<T, N> CircularBufferIterator<T, N>::operator--(int) {
	CircularBufferIterator<T, N> tmp = *this;
	if (_idx == 0) {
		_idx = N - 1;
	} else {
		_idx = _idx - 1;
	}
	return tmp;
}

template <class T, size_t N>
bool CircularBufferIterator<T, N>::operator==(const CircularBufferIterator& other) {
	return (_ptr == other._ptr) && (_idx == other._idx);
}

template <class T, size_t N>
bool CircularBufferIterator<T, N>::operator!=(const CircularBufferIterator& other) {
	return !(*this == other);
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

	if (n > _N - 1) {
		n = _N - 1;
	}

	CircularBufferIterator<_T, _N> ret(it);
	ret._idx += n;
	if (ret._idx >= _N) {
		ret._idx -= _N;
	}
	return ret;
}

template <class _T, size_t _N>
CircularBufferIterator<_T, _N> operator-(const CircularBufferIterator<_T, _N>& it, int n) {

	if (n < 0) {
		return it + (-n);
	}

	if (n > _N - 1) {
		n = _N - 1;
	}

	CircularBufferIterator<_T, _N> ret(it);
	if (ret._idx < n) {
		// since we are using unsigned, don't subtract or it will underflow
		ret._idx += _N;
	}
	ret._idx -= n;
	return ret;
}

#endif /* _CIRCULAR_BUFFER_BASE_HPP */
