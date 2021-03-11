#include "Packet.h"
#include <assert.h>
#include <intrin.h>

void Spooder::WriteInt(Buffer& aBuffer, uint32_t aValue)
{
	assert(aBuffer.index + 4 <= aBuffer.size);
#ifdef BIG_ENDIAN
	*((uint32_t*)(aBuffer.data + aBuffer.index)) = _byteswap_ulong(aValue);
#else
	*((uint32_t*)(aBuffer.data + aBuffer.index)) = aValue;
#endif
	aBuffer.index += 4;
}

void Spooder::WriteShort(Buffer& aBuffer, uint16_t aValue)
{
	assert(aBuffer.index + 2 <= aBuffer.size);
#ifdef BIG_ENDIAN
	* ((uint16_t*)(aBuffer.data + aBuffer.index)) = _byteswap_ushort(aValue);
#else
	* ((uint16_t*)(aBuffer.data + aBuffer.index)) = aValue;
#endif
	aBuffer.index += 2;
}

void Spooder::WriteChar(Buffer& aBuffer, uint8_t aValue)
{
	assert(aBuffer.index + 1 <= aBuffer.size);
	*((uint8_t*)(aBuffer.data + aBuffer.index)) = aValue;
	aBuffer.index += 1;
}

uint32_t Spooder::ReadInt(Buffer & aBuffer)
{
	assert(aBuffer.index + 4 <= aBuffer.size);
	uint32_t value;
#ifdef BIG_ENDIAN
	value = _byteswap_uint64(*((uint32_t*)(aBuffer.data + aBuffer.index))); //need to check that this is fine for ints
#else
	value = *((uint32_t*)(aBuffer.data + aBuffer.index));
#endif
	aBuffer.index += 4;
	return value;
}

uint16_t Spooder::ReadShort(Buffer & aBuffer)
{
	assert(aBuffer.index + 2 <= aBuffer.size);
	uint16_t value;
#ifdef BIG_ENDIAN
	value = _byteswap_ushort(*((uint16_t*)(aBuffer.data + aBuffer.index))); //need to check that this is fine for ints
#else
	value = *((uint16_t*)(aBuffer.data + aBuffer.index));
#endif
	aBuffer.index += 2;
	return value;
}

uint8_t Spooder::ReadChar(Buffer& aBuffer)
{
	assert(aBuffer.index + 2 <= aBuffer.size);
	uint8_t value;
	value = *((uint8_t*)(aBuffer.data + aBuffer.index));
	aBuffer.index += 1;
	return value;
}

Spooder::BitWriter::BitWriter()
{
	myScratch = 0;
	myScratchBits = 0;
	myWordIndex = 0;
	myBuffer = nullptr;
}

bool Spooder::BitReader::WouldReadPastEnd(int aBits)
{
	return myNumBitsRead + aBits < myTotalBits;
}

Spooder::BitWriter::BitWriter(uint32_t* aBuffer)
{
	myBuffer = aBuffer;
	myScratch = 0;
	myScratchBits = 0;
	myWordIndex = 0;
}

void Spooder::BitWriter::WriteBits(void* aData, int aBitsToWrite)
{
	myScratch |= (*(uint32_t*)aData) << myScratchBits; //Write data to scratch, scratchbits is how much data has been written to that scratch
	myScratchBits += aBitsToWrite; //Add how much we just wrote, so we can write from that point
	if (myScratchBits >= 32) //If the scratch has gone into its overflow buffer
	{
		myBuffer[myWordIndex] = myScratch; //write to the buffer
		myWordIndex++; //move to next part of writing
		myScratch >>= 32; //bitshift so we can start at the overflowbuffer
		myScratchBits -= 32; //and move where we are in the scratch accordingly
	}
}
void Spooder::BitWriter::Flush()
{
	myBuffer[myWordIndex] = myScratch; //I think this should be fine;
}

Spooder::BitWriter::~BitWriter()
{

}

Spooder::BitReader::BitReader()
{

	myScratch = 0;
	myScratchBits = 0;
	myTotalBits = 0;
	myNumBitsRead = 0;
	myWordIndex = 0;
	myBuffer = 0;
}
Spooder::BitReader::BitReader(uint32_t * aBuffer, int aBytes)
{
	myScratch = 0;
	myScratchBits = 0;
	myTotalBits = aBytes * 8;
	myNumBitsRead = 0;
	myWordIndex = 0;
	myBuffer = aBuffer;
}
void Spooder::BitReader::ReadBits(void* aDest, int aBitsToRead)
{
	if (myScratchBits < aBitsToRead)
	{
		myScratch |= myBuffer[myWordIndex] << myScratchBits;
		myScratchBits += 32;
		myWordIndex++;
	}
	*(uint32_t*)aDest = myScratch & ((1 << aBitsToRead) - 1);
	myScratch >>= aBitsToRead;
	myScratchBits -= aBitsToRead;
	myNumBitsRead += aBitsToRead;
}
void Spooder::BitReader::Flush()
{

}
Spooder::BitReader::~BitReader()
{

}
