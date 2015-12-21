#include <cstdio>

namespace Alectryon {

template <bool Checksum, bool Address>
DataFrameTransmitBase<Checksum, Address>::DataFrameTransmitBase(uint16_t size) :
	_size(size),
	_idx(0) {

	uint16_t bufferSize = 5; // start delimiter + 2 bytes length (that could be escaped)
	bufferSize += size * 2; // twice the size of message, each byte could be delimited
	if (Checksum) {
		bufferSize += 2;
	}
	if (Address) {
		bufferSize += 8;
	}
	_buffer = (uint8_t*) malloc(bufferSize);
}

template <bool Checksum, bool Address>
DataFrameTransmitBase<Checksum, Address>::~DataFrameTransmitBase() {
	free(_buffer);
}

template <bool Checksum, bool Address>
const uint8_t* DataFrameTransmitBase<Checksum, Address>::getBuffer(size_t& size) {
	size = _idx;
	return _buffer;
}

template <bool Checksum, bool Address>
void DataFrameTransmitBase<Checksum, Address>::clear() {
	_idx = 0;
}

template <bool Checksum, bool Address>
void DataFrameTransmitBase<Checksum, Address>::assembleByte(uint8_t byte) {
	bool isSpecialChar = (byte == DataFrameStartDelimiter) |
		(byte == DataFrameEscapeChar);
	uint8_t xorVal = isSpecialChar << 5;
	_buffer[_idx] = DataFrameEscapeChar;
	_buffer[_idx + isSpecialChar] = byte ^ xorVal;
	_idx += isSpecialChar + 1;
}

template <bool Checksum>
DataFrameTransmit<Checksum, true>::DataFrameTransmit(uint16_t size) :
	DataFrameTransmitBase<Checksum, true>(size) { }

template <bool Checksum>
DataFrameTransmit<Checksum, false>::DataFrameTransmit(uint16_t size) :
	DataFrameTransmitBase<Checksum, false>(size) { }

template <bool Checksum>
void DataFrameTransmit<Checksum, false>::assemble(const uint8_t* packet, uint16_t size) {
	// do checks
	assert(size <= this->_size);

	this->clear();

	this->_buffer[this->_idx++] = DataFrameStartDelimiter;

	this->assembleByte(size >> 8);
	this->assembleByte(size & 0xFF);

	for (int i = 0; i < size; i++) {
		this->assembleByte(packet[i]);
	}

	if (Checksum) {
		uint8_t checksum = 0;
		checksum += (size >> 8);
		checksum += (size & 0xFF);
		for (int i = 0; i < size; i++) {
			checksum += packet[i];
		}
		checksum = 0xFF - checksum;
		this->assembleByte(checksum);
	}
}

template <bool Checksum>
void DataFrameTransmit<Checksum, true>::assemble(const uint8_t* packet, uint16_t size, 
	uint16_t srcAddr, uint16_t dstAddr) {
	// do checks

	this->clear();

	this->_buffer[this->_idx++] = DataFrameStartDelimiter;

	this->assembleByte(size >> 8);
	this->assembleByte(size & 0xFF);

	this->assembleByte(srcAddr >> 8);
	this->assembleByte(srcAddr & 0xFF);

	this->assembleByte(dstAddr >> 8);
	this->assembleByte(dstAddr & 0xFF);

	for (int i = 0; i < size; i++) {
		this->assembleByte(packet[i]);
	}

	if (Checksum) {
		uint8_t checksum = 0;
		checksum += (size >> 8);
		checksum += (size & 0xFF);
		checksum += (srcAddr >> 8);
		checksum += (srcAddr & 0xFF);
		checksum += (dstAddr >> 8);
		checksum += (dstAddr & 0xFF);
		for (int i = 0; i < size; i++) {
			checksum += packet[i];
		}
		checksum = 0xFF - checksum;
		this->assembleByte(checksum);
	}
}

template <bool Checksum, bool Address>
DataFrameRecieve<Checksum, Address>::DataFrameRecieve(uint16_t size) :
	_state(READY),
	_escapedState(false),
	_size(size),
	_frameSize(0),
	_idx(0),
	_srcAddr(0),
	_dstAddr(0),
	_checksum(0) {

	uint16_t bufferSize = size * 2;
	_data = (uint8_t*) malloc(bufferSize);
}

template <bool Checksum, bool Address>
DataFrameRecieve<Checksum, Address>::~DataFrameRecieve() {
	free(_data);
}

// TODO:
// check for framSize error (too big, etc.)
// check for special character error
// check for checksum error
template <bool Checksum, bool Address>
void DataFrameRecieve<Checksum, Address>::pushByte(uint8_t byte) {
	// printf("byte: 0x%02x\tstate: %d\n", byte, _state);
	switch (_state) {
		case READY:
			if (byte == DataFrameStartDelimiter) {
				_state = SIZE_HIGH;
			}
			break;
		case SIZE_HIGH:
			pushByteHelper(byte, _frameSize, SIZE_LOW, true);
			break;
		case SIZE_LOW: {
			State nextState = Address ? SRC_ADDR_HIGH : DATA;
			pushByteHelper(byte, _frameSize, nextState, false);
			break;
		}
		case SRC_ADDR_HIGH:
			pushByteHelper(byte, _srcAddr, SRC_ADDR_LOW, true);
			break;
		case SRC_ADDR_LOW:
			pushByteHelper(byte, _srcAddr, DST_ADDR_HIGH, false);
			break;
		case DST_ADDR_HIGH:
			pushByteHelper(byte, _dstAddr, DST_ADDR_LOW, true);
			break;
		case DST_ADDR_LOW:
			pushByteHelper(byte, _dstAddr, DATA, false);
			break;
		case DATA: {
			State nextState = (_idx < _size - 1) ? DATA : 
				(Checksum ? CHECKSUM : DONE);
			if (_escapedState) {
				_data[_idx++] = byte ^ DataFrameXorVal;
				_escapedState = false;
			} else {
				if (byte == DataFrameEscapeChar) {
					_escapedState = true;
				} else {
					_data[_idx++] = byte;
					_state = nextState;
				}
			}
		}
		case CHECKSUM:
			break;
		case DONE:
			break;
	}
}

template <bool Checksum, bool Address>
void DataFrameRecieve<Checksum, Address>::pushByteHelper(uint8_t byte, uint16_t& var, State nextState, bool shift) {
	if (_escapedState) {
		uint16_t tempVal = byte ^ DataFrameXorVal;
		if (shift) {
			tempVal = tempVal << 8;
		}
		var += tempVal;
		_state = nextState;
		_escapedState = false;
	} else {
		if (byte == DataFrameEscapeChar) {
			_escapedState = true;
		} else {
			uint16_t tempVal = byte;
			if (shift) {
				tempVal = tempVal << 8;
			}
			var += tempVal;
			_state = nextState;
		}
	}
}

template <bool Checksum, bool Address>
void DataFrameRecieve<Checksum, Address>::clear() {
	_frameSize = 0;
	_state = READY;
	_escapedState = false;
	_idx = 0;
	_srcAddr = 0;
	_dstAddr = 0;
	_checksum = 0;
}

template <bool Checksum, bool Address>
const uint8_t* DataFrameRecieve<Checksum, Address>::getData() const {
	return _data;
}

template <bool Checksum, bool Address>
uint16_t DataFrameRecieve<Checksum, Address>::getDataSize() const {
	return _frameSize;
}

template <bool Checksum, bool Address>
uint16_t DataFrameRecieve<Checksum, Address>::getSrcAddr() const {
	return _srcAddr;
}

template <bool Checksum, bool Address>
uint16_t DataFrameRecieve<Checksum, Address>::getDstAddr() const {
	return _dstAddr;
}


}
