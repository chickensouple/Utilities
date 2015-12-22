namespace Alectryon {

namespace DataFrame {

template <bool Checksum, bool Address>
DataFrameTransmitBase<Checksum, Address>::DataFrameTransmitBase(uint16_t capacity) :
	_capacity(capacity),
	_idx(0) {

	uint16_t bufferSize = 5; // start delimiter + 2 bytes length (that could be escaped)
	bufferSize += capacity * 2; // twice the size of message, each byte could be delimited
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
DataFrameTransmit<Checksum, true>::DataFrameTransmit(uint16_t capacity) :
	DataFrameTransmitBase<Checksum, true>(capacity) { }

template <bool Checksum>
DataFrameTransmit<Checksum, false>::DataFrameTransmit(uint16_t capacity) :
	DataFrameTransmitBase<Checksum, false>(capacity) { }

template <bool Checksum>
void DataFrameTransmit<Checksum, false>::assemble(const uint8_t* packet, uint16_t size) {
	// do checks
	assert(size <= this->_capacity);

	this->clear();

	this->_buffer[this->_idx++] = DataFrameStartDelimiter;

	this->assembleByte(size >> 8);
	this->assembleByte(size & 0xFF);

	for (int i = 0; i < size; i++) {
		this->assembleByte(packet[i]);
	}

	if (Checksum) {
		uint8_t checksum = calculateChecksum(size, packet, 0, 0);
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
		uint8_t checksum = calculateChecksum(size, packet, srcAddr, dstAddr);
		this->assembleByte(checksum);
	}
}

template <bool Checksum, bool Address>
DataFrameRecieve<Checksum, Address>::DataFrameRecieve(uint16_t capacity) :
	_frameState(IDLE),
	_state(READY),
	_escapedState(false),
	_capacity(capacity),
	_frameSize(0),
	_idx(0),
	_srcAddr(0),
	_dstAddr(0),
	_checksum(0) {

	uint16_t bufferSize = capacity * 2;
	_data = (uint8_t*) malloc(bufferSize);
}

template <bool Checksum, bool Address>
DataFrameRecieve<Checksum, Address>::~DataFrameRecieve() {
	free(_data);
}

template <bool Checksum, bool Address>
void DataFrameRecieve<Checksum, Address>::pushByte(uint8_t byte) {
	// checking byte for errors
	if ((byte == DataFrameStartDelimiter && _state != READY) ||
		(byte == DataFrameEscapeChar && _escapedState)) {
		_state = ERR;
		_frameState = SPECIAL_CHAR_ERR;
		return;
	}

	switch (_state) {
		case READY:
			if (byte == DataFrameStartDelimiter) {
				_state = SIZE_HIGH;
				_frameState = IN_PROGRESS;
			}
			break;
		case SIZE_HIGH:
			pushByteHelper(byte, _frameSize, SIZE_LOW, true);
			break;
		case SIZE_LOW: {
			State nextState = Address ? SRC_ADDR_HIGH : DATA;
			pushByteHelper(byte, _frameSize, nextState, false);

			// if we read in the whole size, check to make sure it fits
			if (_state != SIZE_LOW && _frameSize > _capacity) {
				_state = ERR;
				_frameState = SIZE_ERR;
			}
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
			State nextState = (_idx < _capacity - 1) ? DATA : 
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

			if (_state == DONE) {
				_frameState = FINISHED;
			}
			break;
		}
		case CHECKSUM:
			if (_escapedState) {
				_checksum = byte ^ DataFrameXorVal;
				_escapedState = false;
				_state = DONE;
			} else {
				if (byte == DataFrameEscapeChar) {
					_escapedState = true;
				} else {
					_checksum = byte;
					_state = DONE;
				}
			}

			if (_state == DONE) {
				uint8_t checksum = calculateChecksum(_frameSize, _data, _srcAddr, _dstAddr);
				if (checksum != _checksum) {
					_state = ERR;
					_frameState = CHECKSUM_ERR;
				}			
			} 
			break;
		case DONE:
			break;
		case ERR:
			break;
	}
}

template <bool Checksum, bool Address>
void DataFrameRecieve<Checksum, Address>::clear() {
	_frameState = IDLE;
	_state = READY;
	_escapedState = false;
	_frameSize = 0;
	_idx = 0;
	_srcAddr = 0;
	_dstAddr = 0;
	_checksum = 0;
}

template <bool Checksum, bool Address>
typename DataFrameRecieve<Checksum, Address>::FrameState DataFrameRecieve<Checksum, Address>::getFrameState() const {
	return _frameState;
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

uint8_t calculateChecksum(uint16_t size, const uint8_t* buffer, 
	uint16_t srcAddr, uint16_t dstAddr) {
	uint8_t checksum = 0;
	checksum += (size >> 8);
	checksum += (size & 0xFF);
	checksum += (srcAddr >> 8);
	checksum += (srcAddr & 0xFF);
	checksum += (dstAddr >> 8);
	checksum += (dstAddr & 0xFF);
	for (uint16_t i = 0; i < size; i++) {
		checksum += buffer[i];
	}
	return checksum;
}

} // namespace DataFrame

} // namespace Alectryon
