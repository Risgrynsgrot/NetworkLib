#pragma once
#include "Serializer.h"
namespace Spooder
{

	struct TestPacket
	{
		int myA[32];
		float myB;
		void Write(BitWriter& aWriter)
		{
			int bitsToWrite = STATIC_BITS_REQUIRED(0, 255);
			for (size_t i = 0; i < 32; i++)
			{
				aWriter.WriteBits(myA + i, bitsToWrite);
			}
		}
		void Read(BitReader& aReader)
		{
			int bitsToRead = STATIC_BITS_REQUIRED(0, 255);

			for (size_t i = 0; i < 32; i++)
			{
				aReader.ReadBits(myA + i, bitsToRead);
			}
		}
		template <typename Stream> bool Serialize(Stream& aStream)
		{
			for (size_t i = 0; i < 32; i++)
			{
				SERIALIZE_INT(aStream, myA[i], 0, 255);
				SERIALIZE_FLOAT_COMPRESSED(aStream, myB, 0.f, 256.f, 0.01f);
			}
			return true;
		}
	};
}
