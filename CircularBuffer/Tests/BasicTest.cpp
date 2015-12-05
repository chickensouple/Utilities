#define BOOST_TEST_MODULE BasicTest
#include <boost/test/included/unit_test.hpp>

#define CIRCULAR_BUFFER_ITERATOR
#include "CircularBuffer.hpp"

BOOST_AUTO_TEST_CASE(push_pop_read) {
	const int buffSize = 10;
	CircularBuffer<int> buff(buffSize);

	int currBuffSize = buffSize;

	// testing basic pushBack()
	for (int i = 0; i < buffSize + 3; i++) {
		bool full = buff.full();
		bool empty = buff.empty();
		bool valid = buff.pushBack(i);
		BOOST_CHECK(valid == (i < buffSize));
		BOOST_CHECK(full == (i >= buffSize));
		BOOST_CHECK(empty == (i == 0));
	}
	BOOST_CHECK(buff.num() == buffSize);

	// testing basic readFront()
	for (int i = 0; i < buffSize; i++) {
		int val = buff.readFront(i);
		BOOST_CHECK(val == i);
	}
	BOOST_CHECK(buff.num() == buffSize);

	// testing basic readBack()
	for (int i = 0; i < buffSize; i++) {
		int val = buff.readBack(i);
		BOOST_CHECK(val == buffSize - i - 1);
	}
	BOOST_CHECK(buff.num() == buffSize);

	// testing basic popBack()
	const int buffSubtractSize = 5;
	for (int i = 0; i < buffSubtractSize; i++) {
		buff.popBack();
	}
	currBuffSize -= buffSubtractSize;
	BOOST_CHECK(buff.num() == currBuffSize);

	for (int i = 0; i < currBuffSize; i++) {
		int val = buff.readFront(i);
		BOOST_CHECK(i == val);
	}
	BOOST_CHECK(buff.num() == currBuffSize);

	// testing basic pushFront()
	const int prevBuffSize = 3;
	for (int i = 1; i < prevBuffSize + 1; i++) {
		buff.pushFront(-i);
	}
	currBuffSize += prevBuffSize;
	BOOST_CHECK(buff.num() == currBuffSize);

	for (int i = 0; i < currBuffSize; i++) {
		int val = buff.readFront(i);
		BOOST_CHECK((i - prevBuffSize) == val);
	}

	// testing basic popFront()
	for (int i = 0; i < currBuffSize + 5; i++) {
		bool valid = buff.popFront();
		BOOST_CHECK(valid == (i < currBuffSize));
	}
	BOOST_CHECK(buff.num() == 0);

	for (int i = 0; i < buffSize; i++) {
		buff.pushFront(i);
	}
	BOOST_CHECK(buff.num() == buffSize);

	for (int i = 0; i < buffSize; i++) {
		buff.popBack();
	}
	BOOST_CHECK(buff.num() == 0);

	for (int i = 0; i < buffSize; i++) {
		buff.pushFront(i);
	}
	BOOST_CHECK(buff.num() == buffSize);
	for (int i = 0; i < currBuffSize; i++) {
		int val = buff.readBack(i);
		BOOST_CHECK(val == i);
	}

	buff.popBack();
	buff.pushBack(22);
	BOOST_CHECK(buff.readBack() == 22);
	buff.popFront();
	buff.pushFront(42);
	BOOST_CHECK(buff.readFront() == 42);
}

BOOST_AUTO_TEST_CASE(ctors_and_copy) {
	const int buffSize = 10;
	CircularBuffer<int> buff(buffSize);
	for (int j = 0; j < buffSize; j++) {
		buff.pushFront(j - 2);
	}
	buff.popBack();
	buff.popBack();
	buff.pushFront(8);
	buff.pushFront(9);

	// testing same size assignment and copy ctor
	CircularBuffer<int> buffCopy(buff);
	CircularBuffer<int> buffEquals(buffSize);
	buffEquals = buff;

	CircularBuffer<int>* buffArr[2] = {&buffCopy, &buffEquals};

	for (int i = 0; i < 2; i++) {
		CircularBuffer<int>& tempBuff = *buffArr[i];
		BOOST_CHECK(tempBuff.num() == buffSize);
		for (int j = 0; j < buffSize; j++) {
			BOOST_CHECK(tempBuff.readBack(j) == j);		
		}
	}

	buff.readBack() = -2;
	buff.popFront();

	for (int i = 0; i < 2; i++) {
		CircularBuffer<int>& tempBuff = *buffArr[i];
		BOOST_CHECK(tempBuff.num() == buffSize);
		for (int j = 0; j < buffSize; j++) {
			BOOST_CHECK(tempBuff.readBack(j) == j);		
		}
	}

	// testing move assignment and move ctor
	CircularBuffer<int> buffMove = std::move(buffCopy);
	CircularBuffer<int> buffMoveAssign(2);
	buffMoveAssign = std::move(buffEquals);
	buffArr[0] = &buffMove;
	buffArr[1] = &buffMoveAssign;

	for (int i = 0; i < 2; i++) {
		CircularBuffer<int>& tempBuff = *buffArr[i];
		BOOST_CHECK(tempBuff.num() == buffSize);
		for (int j = 0; j < buffSize; j++) {
			BOOST_CHECK(tempBuff.readBack(j) == j);		
		}
	}

	// testing assignment with different sized buffers
	const size_t buffSize2 = 8;
	CircularBuffer<int> newBuff2(buffSize2);
	BOOST_CHECK(newBuff2.capacity() == buffSize2);
	newBuff2 = buffMove;
	BOOST_CHECK(newBuff2.capacity() == buffSize);
	for (int i = 0; i < newBuff2.num(); i++) {
		BOOST_CHECK(newBuff2.readBack(i) == i);
	}

	BOOST_CHECK(newBuff2 == buffMove);

	const size_t buffSize3 = 12;
	CircularBuffer<int> newBuff3(buffSize3);
	BOOST_CHECK(newBuff3.capacity() == buffSize3);
	newBuff3 = newBuff2;
	BOOST_CHECK(newBuff3.capacity() == buffSize);
	BOOST_CHECK(newBuff3.num() == newBuff2.num());
	for (int i = 0; i < newBuff3.num(); i++) {
		BOOST_CHECK(newBuff3.readBack(i) == i);
	}
	newBuff3.popBack();

	BOOST_CHECK(newBuff3 != newBuff2);
}

BOOST_AUTO_TEST_CASE(iterator) {
	const int buffSize = 10;
	CircularBuffer<int> buff(buffSize);

	for (int j = 0; j < buffSize; j++) {
		buff.pushFront(j - 8);
	}
	for (int j = 0; j < 8; j++) {
		buff.popBack();
	}
	for (int j = 2; j < buffSize; j++) {
		buff.pushFront(j);
	}

	int count = 0;
	for (auto it = buff.begin(); it != buff.end(); it++) {
		BOOST_CHECK(*it == count++);
	}
	BOOST_CHECK(buff.num() == buffSize);

	int diff = buff.end() - buff.begin();
	BOOST_CHECK(diff == buff.num());
	diff = buff.begin() - buff.end();
	BOOST_CHECK(diff == -buff.num());

	auto beginIt = buff.begin();
	count = buffSize;
	for (auto it = buff.end(); it != beginIt; it--) {
		int diff = it - beginIt;
		bool greaterThan = it > beginIt;
		bool lessThan = it < beginIt;
		bool greaterThanEqual = it >= beginIt;
		bool lessThanEqual = it <= beginIt;
		BOOST_CHECK(diff == count--);
		BOOST_CHECK(greaterThan == true);
		BOOST_CHECK(lessThan == false);
		BOOST_CHECK(greaterThanEqual == true);
		BOOST_CHECK(lessThanEqual == (it == buff.begin()));
	}
	diff = buff.begin() - buff.begin();
	BOOST_CHECK(diff == 0);


	auto middleIt = buff.begin() + (buffSize / 2);
	count = -(buffSize / 2);
	for (auto it = buff.begin(); it != buff.end(); it++) {
		int diff = it - middleIt;
		bool greaterThan = it > middleIt;
		bool lessThan = it < middleIt;
		bool greaterThanEqual = it >= middleIt;
		bool lessThanEqual = it <= middleIt;
		BOOST_CHECK(greaterThan == (count > 0));
		BOOST_CHECK(lessThan == (count < 0));
		BOOST_CHECK(greaterThanEqual == (count >= 0));
		BOOST_CHECK(lessThanEqual == (count <= 0));
		BOOST_CHECK(diff == count++);
	}

	for (int i = 0; i < buffSize; i++) {
		BOOST_CHECK(*(beginIt + i) == i);
		BOOST_CHECK(beginIt[i] == i);
	}
	for (int i = 1; i <= buffSize; i++) {
		auto endIt = buff.end();
		auto diffIt = endIt - i;
		BOOST_CHECK(*diffIt == (buffSize - i));
	}
}

