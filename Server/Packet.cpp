#include "Packet.h"
#include <cstring>
#include <iostream>

Buffer::Buffer(uint32_t inSize)
{
	mIndex = 0;
	mSize = inSize;
	mData = new uint8_t[mSize];
}

Buffer::Buffer(uint32_t inSize, const char* inData)
{
	mIndex = 0;
	mSize = inSize;
	mData = new uint8_t[mSize];
	std::memcpy(mData, inData, inSize);
}

Buffer::~Buffer()
{
	delete[] mData;
}

uint8_t* Buffer::Data()
{
	return mData;
}

uint32_t Buffer::Length()
{
	return mSize;
}

uint32_t Buffer::Index()
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

	uint32_t readValue = *((uint32_t*)(mData + mIndex));
	mIndex += 4;
	return readValue;
}

uint16_t Buffer::ReadShort()
{
	assert(mIndex + 2 <= mSize);	//Don't read past the end of the buffer

	uint16_t readValue = *((uint16_t*)(mData + mIndex));
	mIndex += 2;
	return readValue;
}

uint8_t Buffer::ReadByte()
{
	assert(mIndex + 1 <= mSize);	//Don't read past the end of the buffer

	uint8_t readValue = *((uint8_t*)(mData + mIndex));
	mIndex += 1;
	return readValue;
}