#pragma once
#include "Serializer.h"

namespace Spooder
{
enum MessageType
{
	MESSAGE_TEST,
	MESSAGECOUNT
};
enum PacketTypes
{
	PACKET_CONNECTION_REQUEST,
	PACKET_CONNECTION_DENIED,
	PACKET_CONNECTION_CHALLENGE,
	PACKET_CONNECTION_RESPONSE,
	PACKET_CONNECTION_KEEP_ALIVE,
	PACKET_CONNECTION_DISCONNECT,
	PACKET_CONNECTION_NUM_PACKETS
};
class PacketFactory;
class MessageFactory;

struct TestPacket
{
	int myA[32];
	float myB;
	template <typename Stream>
	bool Serialize(Stream& aStream)
	{
		for (size_t i = 0; i < 32; i++)
		{
			SERIALIZE_INT(aStream, myA[i], 0, 255);
		}
		SERIALIZE_FLOAT_COMPRESSED(aStream, myB, 0.f, 256.f, 0.01f);
		return true;
	}
};

struct Header
{
	uint16_t sequence;
	uint16_t ack;
	uint16_t ackBits;
};
class Object
{
public:
	virtual ~Object() {}
	virtual bool SerializeInternal(class ReadStream& aStream)  = 0;
	virtual bool SerializeInternal(class WriteStream& aStream) = 0;
	// virtual bool SerializeInternal(class MeasureStream& aStream) = 0;
};

#define SPOODER_DECLARE_VIRTUAL_SERIALIZE_FUNCTIONS()                                           \
	bool SerializeInternal(class Spooder::ReadStream& aStream) { return Serialize(aStream); };  \
	bool SerializeInternal(class Spooder::WriteStream& aStream) { return Serialize(aStream); }; \
	// bool SerializeInternal(class Spooder::MeasureStream& aStream){return Serialize(aStream);};

//const int BufferSize = 1024;
//uint32_t sequenceBuffer[BufferSize];
//
//struct PacketData
//{
//	bool acked;
//};
//
//
//PacketData packetData[BufferSize];
//
//inline PacketData* GetPacketData(uint16_t aSequence)
//{
//	const int index = aSequence % BufferSize;
//	if (sequenceBuffer[index] == aSequence)
//	{
//		return &packetData[index];
//	}
//	return nullptr;
//}
//inline PacketData& InsertPacketData(uint16_t aSequence)
//{
//	const int index       = aSequence % BufferSize;
//	sequenceBuffer[index] = aSequence;
//	return packetData[index];
//}

class Packet : public Object
{
public:
	Packet(int aType)
	    : myType(aType)
	{
	}
	virtual ~Packet() {}
	int GetType() const { return myType; }

private:
	int myType;
};

struct PacketInfo
{
	bool rawFormat;
	int prefixBytes;
	uint32_t protocolId;
	PacketFactory* packetFactory;
	const uint8_t* allowedPacketTypes;
	void* context;
	inline PacketInfo()
	{
		rawFormat          = false;
		prefixBytes        = 0;
		protocolId         = 0;
		packetFactory      = nullptr;
		allowedPacketTypes = nullptr;
		context            = nullptr;
	}
};

class Message : public Object
{
public:
	Message(int aType)
	    : myRefCount(1)
	    , myId(0)
	    , myType(aType)
	{
	}

protected:
	~Message() { assert(myRefCount); }

private:
	Message(const Message& aOther);
	const Message& operator=(const Message& aOther) = delete;

	int myRefCount;
	uint32_t myId : 16;
	uint32_t myType : 16;
};

struct TestMessage : public Message
{
	TestMessage()
	    : Message(MESSAGE_TEST)
	{
		mySequence = 0;
	}

	template <typename Stream>
	bool Serialize(Stream& aStream)
	{
		SERIALIZE_BITS(aStream, mySequence, 16);
		return true;
	}
	SPOODER_DECLARE_VIRTUAL_SERIALIZE_FUNCTIONS();

	uint16_t mySequence;
};

namespace Packets
{

	struct ConnectionRequest : public Packet
	{
		ConnectionRequest()
			: Packet(PACKET_CONNECTION_REQUEST)
		{
		}
		template <typename Stream>
		bool Serialize(Stream& aStream)
		{
			return true;
		}
		SPOODER_DECLARE_VIRTUAL_SERIALIZE_FUNCTIONS();
	};

	struct ConnectionDisconnect : public Packet
	{
		ConnectionDisconnect()
			: Packet(PACKET_CONNECTION_DISCONNECT)
		{
		}
		template <typename Stream>
		bool Serialize(Stream& aStream)
		{
			return true;
		}
		SPOODER_DECLARE_VIRTUAL_SERIALIZE_FUNCTIONS();
	};

	enum ConnectionDeniedReasons
	{
		CONNECTION_DENIED_SERVER_FULL,
		CONNECTION_DENIED_ALREADY_CONNECTED,
		CONNECTION_DENIED_COUNT
	};
	struct ConnectionDenied : public Packet
	{
		int myReason;
		ConnectionDenied()
			: Packet(PACKET_CONNECTION_DENIED), myReason(0)
		{
		}
		template <typename Stream>
		bool Serialize(Stream& aStream)
		{
			SERIALIZE_INT(aStream, myReason, 0, CONNECTION_DENIED_COUNT);
			return true;
		}
		SPOODER_DECLARE_VIRTUAL_SERIALIZE_FUNCTIONS();
	};

	struct ConnectionKeepAlive : public Packet
	{
		ConnectionKeepAlive() : Packet(PACKET_CONNECTION_KEEP_ALIVE){}
		template <typename Stream>
		bool Serialize(Stream& aStream)
		{
			return true;
		}
		SPOODER_DECLARE_VIRTUAL_SERIALIZE_FUNCTIONS();
	};

	struct ConnectionChallenge : public Packet //Lägg till salt, literally därför det finns
	{
		ConnectionChallenge() : Packet(PACKET_CONNECTION_CHALLENGE){}

		template<typename Stream>
		bool Serialize(Stream& aStream)
		{
			return true;
		}
		SPOODER_DECLARE_VIRTUAL_SERIALIZE_FUNCTIONS();
	};

	struct ConnectionResponse : public Packet //Lägg till salt, literally därför det finns
	{
		ConnectionResponse() : Packet(PACKET_CONNECTION_RESPONSE){}

		template<typename Stream>
		bool Serialize(Stream& aStream)
		{
			return true;
		}
		SPOODER_DECLARE_VIRTUAL_SERIALIZE_FUNCTIONS();
	};

	struct ConnectionContext
	{
		MessageFactory* messageFactory;
	};
}
}  // namespace Spooder

