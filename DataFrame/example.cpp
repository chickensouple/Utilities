#include "DataFrame.hpp"
#include <cstdio>

using namespace Alectryon::DataFrame;

int main() {
	uint8_t data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0x7E, 0x7D, 0x20, 2};
	DataFrameTransmit<false, true> tx(sizeof(data));
	tx.assemble(data, sizeof(data), 0x42, 0x7E);

	size_t size;
	const uint8_t* buf = tx.getBuffer(size);

	printf("data size: %lu\n", sizeof(data));
	printf("buff size: %lu\n", size);
	for (size_t i = 0; i < size; i++) {
		printf("0x%02x\t", buf[i]);
	}
	printf("\n");


	DataFrameRecieve<false, true> rx(sizeof(data));
	for (size_t i = 0; i < size; i++) {
		rx.pushByte(buf[i]);
	}

	const uint8_t* buf2 = rx.getData();
	uint16_t rxSize = rx.getDataSize();
	printf("rx size: %d\n", rxSize);
	printf("src addr: 0x%02x\n", rx.getSrcAddr());
	printf("dst addr: 0x%02x\n", rx.getDstAddr());
	printf("frame state: %d\n", rx.getFrameState());
	for (uint16_t i = 0; i < rxSize; i++) {
		printf("0x%02x\t", buf2[i]);
	}
	printf("\n");


}
