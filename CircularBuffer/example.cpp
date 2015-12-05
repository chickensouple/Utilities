#include <iostream>
#include <cstdio>
#include "CircularBuffer.hpp"

int main() {
	CircularBuffer<int> buff(8);
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
}
