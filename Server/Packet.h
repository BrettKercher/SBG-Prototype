#pragma once

#include <cstdint>
#include <assert.h>


class Buffer
{
public:

	Buffer(uint32_t inSize);
	Buffer(uint32_t inSize, const char* inData);
	~Buffer();

	uint8_t* Data();
	uint32_t Length();
	uint32_t Index();

	void WriteInt(uint32_t value);
	void WriteShort(uint16_t value);
	void WriteByte(uint8_t value);

	uint32_t ReadInt();
	uint16_t ReadShort();
	uint8_t  ReadByte();

	void Clear();

private:
	uint8_t* mData;
	uint32_t mSize;
	uint32_t mIndex;

};
