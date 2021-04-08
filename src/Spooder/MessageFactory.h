#pragma once
#include "PacketTypes.h"

namespace Spooder
{
	class MessageFactory
	{
	public:
		virtual ~MessageFactory() = default;
		MessageFactory(int aTypeAmount);
		Message* Create(int aType);
	protected:
		virtual Message* CreateInternal(int aType) = 0;
	private:
		int myTypeAmount;
	};

	class TestMessageFactory : public MessageFactory
	{
	public:
		TestMessageFactory() : MessageFactory(MESSAGECOUNT) {}
	protected:
		Message* CreateInternal(int aType) override;
	private:
	};


	class PacketFactory
	{
	public:
		PacketFactory(int aNumTypes);
		virtual ~PacketFactory();
		Packet* CreatePacket(int aType);
		void DestroyPacket(Packet* aPacket);
		int GetNumPacketTypes() const;
	protected:
		virtual Packet* Create(int aType) = 0;
		virtual void Destroy(Packet* aPacket) = 0;
	private:
		int myNumPacketTypes;
		int myNumAllocatedPackets = 0;
	};

	class ClientServerPacketFactory : public PacketFactory
	{
	public:
		ClientServerPacketFactory() : PacketFactory(PACKET_CONNECTION_NUM_PACKETS) {}
	private:
		Packet* Create(int aType) override;
		void Destroy(Packet* aPacket) override;
	};
}