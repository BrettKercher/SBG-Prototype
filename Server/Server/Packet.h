#pragma once

#include <cstdint>
#include <assert.h>


class Buffer
{
public:

	Buffer(int inSize);
	~Buffer();

	uint8_t* Data();
	int Length();
	int Index();

	void WriteInt(uint32_t value);
	void WriteShort(uint16_t value);
	void WriteByte(uint8_t value);

	uint32_t ReadInt();
	uint16_t ReadShort();
	uint8_t  ReadByte();

	void Clear();

private:
	uint8_t* mData;
	int mSize;
	int mIndex;

};
