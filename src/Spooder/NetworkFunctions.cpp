#include "NetworkFunctions.h"

#include <cassert>

#include "Address.h"
#include "MessageFactory.h"
#include "PacketTypes.h"
#include "Socket.h"

void Spooder::SendPacket(Socket* aSocket, PacketFactory* aPacketFactory, const Address& aAddress, Packet* aPacket)
{
	assert(aSocket);
	assert(aPacketFactory);
	assert(aAddress.IsValid());
	assert(aPacket);

	uint8_t packetData[MaxPacketSize];

	PacketInfo info;
	info.protocolId = ProtocolID;
	info.packetFactory = aPacketFactory;

	const int packetSize = WritePacket(info, aPacket, packetData, MaxPacketSize);

	if(packetSize > 0)
	{
		aSocket->SendPacket(aAddress, packetData, packetSize);
	}
	aPacketFactory->DestroyPacket(aPacket);
}

Spooder::Packet* Spooder::ReceivePacket(Socket* aSocket, PacketFactory* aPacketFactory, Address& aAddress)
{
	uint8_t packetData[MaxPacketSize];

	const int packetBytes = aSocket->ReceivePacket(aAddress, packetData, MaxPacketSize);

	if(!packetBytes)
	{
		return nullptr;
	}

	PacketInfo info;
	info.protocolId = ProtocolID;
	info.packetFactory = aPacketFactory;

	return ReadPacket(info, packetData, packetBytes, nullptr);
}

int Spooder::WritePacket(const PacketInfo& aInfo, Packet* aPacket, uint8_t* aBuffer, int aBufferSize, Object* aHeader)
{
	assert(aPacket);
	assert(aBuffer);
	assert(aBufferSize > 0);
	assert(aInfo.protocolId);
	assert(aInfo.packetFactory);

	const int numPacketTypes = aInfo.packetFactory->GetNumPacketTypes();

	WriteStream stream(aBuffer, aBufferSize);

	stream.SetContext(aInfo.context);

	for (size_t i = 0; i < aInfo.prefixBytes; i++)
	{
		uint8_t zero = 0;
		stream.SerializeBits(&zero, 8);
	}
	if(aHeader)
	{
		if(!aHeader->SerializeInternal(stream))
		{
			return 0;
		}
	}

	int packetType = aPacket->GetType();
	assert(numPacketTypes > 0);

	if(numPacketTypes > 1)
	{
		stream.SerializeInt(packetType, 0, numPacketTypes - 1);
	}
	if(!aPacket->SerializeInternal(stream))
	{
		return 0;
	}
	//stream.SerializeCheck("end of packet");

	stream.Flush();

	if(stream.GetError())
	{
		return 0;
	}
	return stream.GetBytesProcessed();
}

Spooder::Packet* Spooder::ReadPacket(const PacketInfo& aInfo, const uint8_t* aBuffer, int aBufferSize, Object* aHeader,
	int* aErrorCode)
{
	assert(aBuffer);
	assert(aBufferSize > 0);
	assert(aInfo.protocolId != 0);
	assert(aInfo.packetFactory);

	if(aErrorCode)
	{
		*aErrorCode = PACKET_ERROR_NONE;
	}

	ReadStream stream(aBuffer, aBufferSize);

	stream.SetContext(aInfo.context);

	for (int i = 0; i < aInfo.prefixBytes; i++)
	{
		uint32_t dummy = 0;
		stream.SerializeBits(&dummy, 8);
	}

	if(aHeader)
	{
		if(!aHeader->SerializeInternal(stream))
		{
			if(aErrorCode)
			{
				*aErrorCode = PACKET_ERROR_SERIALIZE_HEADER_FAILED;
			}
			return nullptr;
		}
	}

	int packetType = 0;

	const int numPacketTypes = aInfo.packetFactory->GetNumPacketTypes();

	assert(numPacketTypes > 0);

	if(numPacketTypes > 1)
	{
		if(!stream.SerializeInt(packetType, 0, numPacketTypes - 1))
		{
			if(aErrorCode)
			{
				*aErrorCode = PACKET_ERROR_INVALID_PACKET_TYPE;
			}
			return nullptr;
		}
	}

	if(aInfo.allowedPacketTypes)
	{
		if(!aInfo.allowedPacketTypes[packetType])
		{
			if(aErrorCode)
			{
				*aErrorCode = PACKET_ERROR_PACKET_TYPE_NOT_ALLOWED;
			}
			return nullptr;
		}
	}

	Packet* packet = aInfo.packetFactory->CreatePacket(packetType);
	if(!packet)
	{
		if(aErrorCode)
		{
			*aErrorCode = PACKET_ERROR_SERIALIZE_PACKET_FAILED;
		}
	}
	if (stream.GetError())
	{
		*aErrorCode = stream.GetError();
		goto cleanup;
	}
	return packet;

cleanup: //Mega ugly but I mean, kinda nice to not rewrite code
	aInfo.packetFactory->DestroyPacket(packet);
	return nullptr;
}

bool Spooder::InitializeNetwork()
{
	assert(!ourNetworkInitialized);
	bool result = true;

	WSADATA wsaData;
	result = WSAStartup(MAKEWORD(2,2), &wsaData) == NO_ERROR;

	if(result)
	{
		ourNetworkInitialized = result;
	}
	return result;
}
