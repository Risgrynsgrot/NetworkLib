#pragma once
#include <cstdint>
#include <cmath>
namespace Spooder
{
	template <uint32_t x> struct PopCount
	{
		enum {
			a = x - ((x >> 1) & 0x55555555),
			b = (((a >> 2) & 0x33333333) + (a & 0x33333333)),
			c = (((b >> 4) + b) & 0x0f0f0f0f),
			d = c + (c >> 8),
			e = d + (d >> 16),
			result = e & 0x0000003f
		};
	};

	template <uint32_t x> struct Log2
	{
		enum {
			a = x | (x >> 1),
			b = a | (a >> 2),
			c = b | (b >> 4),
			d = c | (c >> 8),
			e = d | (d >> 16),
			f = e >> 1,
			result = PopCount<f>::result
		};
	};

	template <int64_t min, int64_t max> struct BitsRequired
	{
		static const uint32_t result =
			(min == max) ? 0 : (Log2<uint32_t(max - min)>::result + 1);
	};

#define STATIC_BITS_REQUIRED( min, max ) BitsRequired<min,max>::result

	inline int bits_required(uint32_t min, uint32_t max)
	{
		return (min == max) ? 0 : log2(max - min) + 1;
	}

	struct Buffer
	{
		uint8_t* data;
		int size;
		int index;
	};

	class BitWriter
	{
	public:
		BitWriter();
		BitWriter(uint32_t* aBuffer);
		void WriteBits(void* aData, int aBitsToWrite);
		void Flush();
		~BitWriter();
	private:
		uint64_t myScratch;
		int myScratchBits;
		int myWordIndex;
		uint32_t* myBuffer;
	};

	class BitReader
	{
	public:
		BitReader();
		BitReader(uint32_t* aBuffer, int aBytes);
		void ReadBits(void* aData, int aBitsToRead);
		void Flush();
		bool WouldReadPastEnd(int aBits);
		~BitReader();
	private:
		uint64_t myScratch;
		int myScratchBits;
		int myTotalBits;
		int myNumBitsRead;
		int myWordIndex;
		uint32_t* myBuffer;
	};

	void WriteInt(Buffer& aBuffer, uint32_t aValue);
	void WriteShort(Buffer& aBuffer, uint16_t aValue);
	void WriteChar(Buffer& aBuffer, uint8_t aValue);

	uint32_t ReadInt(Buffer& aBuffer);
	uint16_t ReadShort(Buffer& aBuffer);
	uint8_t ReadChar(Buffer& aBuffer);


}
