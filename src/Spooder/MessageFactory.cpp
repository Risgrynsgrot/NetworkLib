#include "MessageFactory.h"

Spooder::MessageFactory::MessageFactory(int aTypeAmount)
{
	myTypeAmount = aTypeAmount;
}

Spooder::Message* Spooder::MessageFactory::Create(int aType)
{
	assert(aType >= 0);
	assert(aType < myTypeAmount);
	return CreateInternal(aType);
}

Spooder::Message* Spooder::TestMessageFactory::CreateInternal(int aType)
{
	using namespace Spooder;
	switch (aType)
	{
	case MESSAGE_TEST: return new TestMessage();
	default:
		return NULL;
	}
}

Spooder::PacketFactory::PacketFactory(int aNumTypes)
{
	myNumPacketTypes = aNumTypes;
}

Spooder::PacketFactory::~PacketFactory()
{
}

Spooder::Packet* Spooder::PacketFactory::CreatePacket(int aType)
{
	assert(aType >= 0);
	assert(aType < myNumPacketTypes);

	Packet* packet = Create(aType);
	if(!packet)
	{
		return nullptr;
	}
	myNumAllocatedPackets++;
	return packet;
}

void Spooder::PacketFactory::DestroyPacket(Packet* aPacket)
{
	if(!aPacket)
	{
		return;
	}
	assert(myNumAllocatedPackets > 0);
	myNumAllocatedPackets--;
	Destroy(aPacket);
}

int Spooder::PacketFactory::GetNumPacketTypes() const
{
	return myNumPacketTypes;
}

Spooder::Packet* Spooder::ClientServerPacketFactory::Create(int aType)
{
	switch(aType)
	{
	case PACKET_CONNECTION_REQUEST:
		return new Packets::ConnectionRequest();
	case PACKET_CONNECTION_DISCONNECT:
		return new Packets::ConnectionDisconnect();
	case PACKET_CONNECTION_DENIED:
		return new Packets::ConnectionDenied();
	case PACKET_CONNECTION_KEEP_ALIVE:
		return new Packets::ConnectionKeepAlive();
	case PACKET_CONNECTION_RESPONSE:
		return new Packets::ConnectionResponse();
	case PACKET_CONNECTION_CHALLENGE:
		return new Packets::ConnectionChallenge();
	default:
		return nullptr;
	}
}

void Spooder::ClientServerPacketFactory::Destroy(Packet* aPacket)
{
	delete aPacket;
}
