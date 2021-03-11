#pragma once
#include <assert.h>
#include <cmath>
#include "Packet.h"
#include "MathFunctions.h"

namespace Spooder
{

	class WriteStream
	{
	public:
		enum { IsWriting = 1 };
		enum { IsReading = 0 };

        inline bool SerializeBits(void* aData, int aBitsToWrite)
        {
            myWriter.WriteBits(aData, aBitsToWrite);
        }

		WriteStream(uint32_t* aBuffer, int aBytes) : myWriter(aBuffer) {}
		inline bool SerializeInt(int32_t aValue, int32_t aMin, int32_t aMax)
		{
			assert(aMin < aMax, "min cannot be bigger than max");
			assert(aValue >= aMin);
			assert(aValue <= aMax);

			const int bits = bits_required(aMin, aMax);
			uint32_t unsignedValue = aValue - aMin;
			myWriter.WriteBits(&aValue, bits);
            return true;
		}
	private:
		BitWriter myWriter;
	};

    class ReadStream
    {
    public:
        enum {IsWriting = 0};
        enum {IsReading = 1};

        inline bool SerializeBits(void* aData, int aBitsToRead)
        {
            myReader.ReadBits(aData, aBitsToRead);
        }
		inline bool SerializeInt(int32_t& aValue, int32_t aMin, int32_t aMax)
		{
			assert(aMin < aMax, "min cannot be bigger than max");

			const int bits = bits_required(aMin, aMax);
            if (myReader.WouldReadPastEnd(bits))
            {
                return false;
            }
            uint32_t unsignedValue = 0;
            myReader.ReadBits(&unsignedValue, bits);
            aValue = (int32_t)unsignedValue + aMin;
            return true;
		}
    private:
        BitReader myReader;
    };

    template <typename Stream>
    bool SerializeFloatInternal(Stream& aStream, float& aValue)
    {
        union FloatInt
        {
            float floatValue;
            uint32_t intValue;
        };
        FloatInt tmp;
        if (Stream::IsWriting)
        {
            aValue = tmp.floatValue;
        }
        bool result = aStream.SerializeBits(&tmp.intValue, 32);
        if (Stream::IsReading)
        {
            aValue = tmp.floatValue;
        }
        return result;
    }

    template <typename Stream>
    bool SerializeFloatCompressedInternal(Stream& aStream, float& aValue, float aMin, float aMax, float aRes)
    {
        const float delta = aMax - aMin;
        const float values = delta / aRes;
        const uint32_t maxIntegerValue = (uint32_t)ceil(values);
        const int bits = bits_required(0, maxIntegerValue);
        uint32_t intValue = 0;
        if (Stream::IsWriting)
        {
            float normalizedValue = clamp((aValue - aMin) / delta, 0.0f, 1.0f);
            intValue = (uint32_t)floor(normalizedValue * maxIntegerValue + 0.5f);
        }
        if (!aStream.SerializeBits(&intValue, bits))
        {
            return false;
        }
        if (Stream::IsReading)
        {
            const float normalizedValue = intValue / float(maxIntegerValue);
            aValue = normalizedValue * delta + aMin;
        }
        return true;
    }

}

#define SERIALIZE_FLOAT(stream, value)            \
    do                                            \
    {                                             \
                                                  \
		if(!SerializeFloatInternal(stream, value))\
		{                                         \
			return false;                         \
		}                                         \
                                                  \
    } while (false) 



#define SERIALIZE_INT( stream, value, minimum, maximum )            \
    do                                                              \
    {                                                               \
        assert( minimum < maximum );                                \
        int32_t int32_value;                                        \
        if ( Stream::IsWriting )                                    \
        {                                                           \
            assert( value >= minimum );                             \
            assert( value <= maximum );                             \
            int32_value = (int32_t) value;                          \
        }                                                           \
        if ( !stream.SerializeInt<minimum, maximum>(int32_value) )  \
        {                                                           \
            return false;                                           \
        }                                                           \
        if ( Stream::IsReading )                                    \
        {                                                           \
            value = int32_value;                                    \
            if ( value < minimum || value > maximum )               \
            {                                                       \
                return false;                                       \
            }                                                       \
        }                                                           \
     } while (0)

#define SERIALIZE_FLOAT_COMPRESSED(stream, value, minimum, maximum, res)     \
    do                                                                  \
    {                                                                   \
        if(!SerializeCompressedFloatInternal(stream, value, minimum, maximum, res))\
        {                                                               \
            return false;                                               \
        }                                                               \
    } while (0);