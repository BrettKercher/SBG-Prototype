#pragma once

#include <cstdint>
#include <assert.h>


class Buffer
{
public:
	uint8_t* data;
	int size;
	int index;

	Buffer(int inSize);

	void WriteInt(uint32_t value);
	void WriteShort(uint16_t value);
	void WriteByte(uint8_t value);

	uint32_t ReadInt();
	uint16_t ReadShort();
	uint8_t  ReadByte();

	void Clear();
};
