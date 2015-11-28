#include <iostream>
#include <cstdio>
#include "CircularBufferDynamic.hpp"

int main() {
	CircularBufferDynamic<int, 8> buff;
	for (int i = 0; i < 10; i++) {
		bool valid = buff.pushFront(i);
		printf("Valid: %d\tnum: %zu\tfull: %d\tempty: %d\n", valid, buff.num(), buff.full(), buff.empty());
	}
	for (int i = 0; i < 2; i++) {
		buff.popBack();
		buff.popFront();
	}
	for (int i = 0; i < 10; i++) {
		buff.pushBack(i + 10);
	}

	for (int i = 0; i < 8; i++) {
		printf("idx %d: %d\n", i, buff.readBack(i));
	}

	for (auto i = buff.cbegin(); i != buff.cend(); i++) {
		printf("Iterator: %d\n", *i);
	}

	// for (int i = 0; i < 10; i++) {
	// 	int val = buff.readBack();

	// 	bool valid = buff.popBack();
	// 	printf("Valid: %d\tval: %d\tnum: %d\tfull: %d\tempty: %d\n", valid, val, buff.num(), buff.full(), buff.empty());
	// }

}
