#ifndef _DATA_FRAME_HPP
#define _DATA_FRAME_HPP

#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cassert>

namespace Alectryon {

const uint8_t DataFrameStartDelimiter = 0x7E;
const uint8_t DataFrameEscapeChar = 0x7D;
const uint8_t DataFrameXorVal = 0x20;

template <bool Checksum, bool Address>
class DataFrameTransmitBase {
public:
	DataFrameTransmitBase() = delete;
	DataFrameTransmitBase(uint16_t size);
	~DataFrameTransmitBase();

	const uint8_t* getBuffer(size_t& size);

	void clear();
protected:
	void assembleByte(uint8_t byte);

	uint16_t _size;
	size_t _idx;
	uint8_t* _buffer;
};

template <bool Checksum, bool Address>
class DataFrameTransmit : public DataFrameTransmitBase<Checksum, Address> { };

template <bool Checksum>
class DataFrameTransmit<Checksum, false> : public DataFrameTransmitBase<Checksum, false> {
public:
	DataFrameTransmit(uint16_t size);

	void assemble(const uint8_t* packet, uint16_t size);
};

template <bool Checksum>
class DataFrameTransmit<Checksum, true> : public DataFrameTransmitBase<Checksum, true> {
public:
	DataFrameTransmit(uint16_t size);

	void assemble(const uint8_t* packet, uint16_t size, uint16_t srcAddr, uint16_t dstAddr);
};

template <bool Checksum, bool Address>
class DataFrameRecieve {
public:
	DataFrameRecieve() = delete;
	DataFrameRecieve(uint16_t size);
	~DataFrameRecieve();

	void pushByte(uint8_t byte);
	void clear();

	const uint8_t* getData() const;
	uint16_t getDataSize() const;
	uint16_t getSrcAddr() const;
	uint16_t getDstAddr() const;

protected:
	enum State { READY, SIZE_HIGH, SIZE_LOW, 
		SRC_ADDR_HIGH, SRC_ADDR_LOW,
		DST_ADDR_HIGH, DST_ADDR_LOW,
		DATA, CHECKSUM, DONE };

	State _state;
	bool _escapedState;

	uint16_t _size;
	size_t _idx;
	uint8_t* _data;

	uint16_t _srcAddr;
	uint16_t _dstAddr;
	uint16_t _checksum;

	void pushByteHelper(uint8_t byte, uint16_t& var, State nextState, bool shift);
};

}

#include "DataFrameImplementation.hpp"

#endif /* _DATA_FRAME_HPP */
