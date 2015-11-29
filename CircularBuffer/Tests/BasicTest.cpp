#define BOOST_TEST_MODULE BasicTest
#include <boost/test/included/unit_test.hpp>

#define CIRCULAR_BUFFER_ITERATOR
#include "CircularBufferDynamic.hpp"
#include "CircularBufferStatic.hpp"

BOOST_AUTO_TEST_CASE(push_pop_read) {
	const int buffSize = 10;
	CircularBufferStatic<int, buffSize> staticBuff;
	CircularBufferDynamic<int, buffSize> dynamicBuff;
	CircularBufferBase<int, buffSize>* buffArr[2] =
		{&staticBuff, &dynamicBuff};

	for (int buffIdx = 0; buffIdx < 2; buffIdx++) {
		int currBuffSize = buffSize;
		CircularBufferBase<int, buffSize>& buff = *buffArr[buffIdx];

		for (int i = 0; i < buffSize + 3; i++) {
			bool full = buff.full();
			bool empty = buff.empty();
			bool valid = buff.pushBack(i);
			BOOST_CHECK(valid == (i < buffSize));
			BOOST_CHECK(full == (i >= buffSize));
			BOOST_CHECK(empty == (i == 0));
		}
		BOOST_CHECK(buff.num() == buffSize);

		for (int i = 0; i < buffSize; i++) {
			int val = buff.readFront(i);
			BOOST_CHECK(val == i);
		}
		BOOST_CHECK(buff.num() == buffSize);

		for (int i = 0; i < buffSize; i++) {
			int val = buff.readBack(i);
			BOOST_CHECK(val == buffSize - i - 1);
		}
		BOOST_CHECK(buff.num() == buffSize);

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
}

BOOST_AUTO_TEST_CASE(ctors_and_copy) {
	const int buffSize = 10;
	CircularBufferStatic<int, buffSize> staticBuff;
	CircularBufferDynamic<int, buffSize> dynamicBuff;
	CircularBufferBase<int, buffSize>* buffArr[2] =
		{&staticBuff, &dynamicBuff};

	for (int i = 0; i < 2; i++) {
		CircularBufferBase<int, buffSize>& buff = *buffArr[i];
		for (int j = 0; j < buffSize; j++) {
			buff.pushFront(j - 2);
		}
		buff.popBack();
		buff.popBack();
		buff.pushFront(8);
		buff.pushFront(9);
	}

	CircularBufferStatic<int, buffSize> staticBuffCopyStatic(staticBuff);
	CircularBufferStatic<int, buffSize> staticBuffCopyDynamic(dynamicBuff);
	CircularBufferDynamic<int, buffSize> dynamicBuffCopyStatic(staticBuff);
	CircularBufferDynamic<int, buffSize> dynamicBuffCopyDynamic(dynamicBuff);

	CircularBufferStatic<int, buffSize> staticBuffEqualsStatic;
	CircularBufferStatic<int, buffSize> staticBuffEqualsDynamic;
	CircularBufferDynamic<int, buffSize> dynamicBuffEqualsStatic;
	CircularBufferDynamic<int, buffSize> dynamicBuffEqualsDynamic;

	staticBuffEqualsStatic = staticBuff;
	staticBuffEqualsDynamic = dynamicBuff;
	dynamicBuffEqualsStatic = staticBuff;
	dynamicBuffEqualsDynamic = dynamicBuff;

	CircularBufferBase<int, buffSize>* copyBuffArr[8] = 
		{&staticBuffCopyStatic,
		&staticBuffCopyDynamic,
		&dynamicBuffCopyStatic,
		&dynamicBuffCopyDynamic,
		&staticBuffEqualsStatic,
		&staticBuffEqualsDynamic,
		&dynamicBuffEqualsStatic,
		&dynamicBuffEqualsDynamic};

	for (int i = 0; i < 8; i++) {
		CircularBufferBase<int, buffSize>& buff = *copyBuffArr[i];
		BOOST_CHECK(buff.num() == buffSize);
		for (int j = 0; j < buffSize; j++) {
			BOOST_CHECK(buff.readBack(j) == j);		
		}
	}

	staticBuff.readBack() = -1;
	dynamicBuff.readBack() = -2;
	staticBuff.popFront();
	dynamicBuff.popFront();

	for (int i = 0; i < 8; i++) {
		CircularBufferBase<int, buffSize>& buff = *copyBuffArr[i];
		BOOST_CHECK(buff.num() == buffSize);
		for (int j = 0; j < buffSize; j++) {
			BOOST_CHECK(buff.readBack(j) == j);		
		}
	}
}

BOOST_AUTO_TEST_CASE(iterator) {
	const int buffSize = 10;
	CircularBufferStatic<int, buffSize> staticBuff;
	CircularBufferDynamic<int, buffSize> dynamicBuff;
	CircularBufferBase<int, buffSize>* buffArr[2] =
		{&staticBuff, &dynamicBuff};


	for (int i = 0; i < 2; i++) {
		CircularBufferBase<int, buffSize>& buff = *buffArr[i];
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
}

