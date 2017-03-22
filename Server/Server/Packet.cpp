#include "Packet.h"

Buffer::Buffer(int inSize)
{
	mSize = inSize;
	mData = new uint8_t[mSize];
}

Buffer::~Buffer()
{
	delete[] mData;
}

uint8_t* Buffer::Data()
{
	return mData;
}

int Buffer::Length()
{
	return mSize;
}

int Buffer::Index()
{
	return mIndex;
}


void Buffer::Clear()
{
	mIndex = 0;
}

void Buffer::WriteInt(uint32_t value)
{
	assert(mIndex + 4 <= mSize);	//Can't write past the end of the buffer

	*((uint32_t*)(mData + mIndex)) = value;
	mIndex += 4;
}

void Buffer::WriteShort(uint16_t value)
{
	assert(mIndex + 2 <= mSize);	//Can't write past the end of the buffer

	*((uint16_t*)(mData + mIndex)) = value;
	mIndex += 2;
}

void Buffer::WriteByte(uint8_t value)
{
	assert(mIndex + 1 <= mSize);	//Can't write past the end of the buffer

	*((uint8_t*)(mData + mIndex)) = value;
	mIndex += 1;
}

uint32_t Buffer::ReadInt()
{
	assert(mIndex + 4 <= mSize);	//Don't read past the end of the buffer

	uint32_t readValue = *((uint32_t*)(mData + mSize));
	mIndex += 4;
	return readValue;
}

uint16_t Buffer::ReadShort()
{
	assert(mIndex + 2 <= mSize);	//Don't read past the end of the buffer

	uint32_t readValue = *((uint16_t*)(mData + mSize));
	mIndex += 2;
	return readValue;
}

uint8_t Buffer::ReadByte()
{
	assert(mIndex + 1 <= mSize);	//Don't read past the end of the buffer

	uint32_t readValue = *((uint8_t*)(mData + mSize));
	mIndex += 1;
	return readValue;
}