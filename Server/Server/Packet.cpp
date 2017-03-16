#include "Packet.h"

Buffer::Buffer(int inSize)
{
	size = inSize;
	//malloc data?
}

void Buffer::Clear()
{
	index = 0;
	size = 0;
}

void Buffer::WriteInt(uint32_t value)
{
	assert(index + 4 < size);	//Can't write past the end of the buffer

	*((uint32_t*)(data + index)) = value;
	index += 4;
}

void Buffer::WriteShort(uint16_t value)
{
	assert(index + 2 < size);	//Can't write past the end of the buffer

	*((uint16_t*)(data + index)) = value;
	index += 2;
}

void Buffer::WriteByte(uint8_t value)
{
	assert(index + 1 < size);	//Can't write past the end of the buffer

	*((uint8_t*)(data + index)) = value;
	index += 1;
}

uint32_t Buffer::ReadInt()
{
	assert(index + 4 < size);	//Don't read past the end of the buffer

	uint32_t readValue = *((uint32_t*)(data + size));
	index += 4;
	return readValue;
}

uint16_t Buffer::ReadShort()
{
	assert(index + 2 < size);	//Don't read past the end of the buffer

	uint32_t readValue = *((uint16_t*)(data + size));
	index += 2;
	return readValue;
}

uint8_t Buffer::ReadByte()
{
	assert(index + 1 < size);	//Don't read past the end of the buffer

	uint32_t readValue = *((uint8_t*)(data + size));
	index += 1;
	return readValue;
}