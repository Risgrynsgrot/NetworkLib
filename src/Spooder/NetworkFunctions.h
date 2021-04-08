#pragma once
#include <cstdint>

#include "CommonNetworkIncludes.h"


const int MaxPacketSize = 1200;
const int ProtocolID = 0x696969FF;
const float ConnectionRequestTimeOut = 5.0f;
const float ChallengeResponseTimeOut = 5.0f;
const float KeepAliveTimeOut = 10.0f;
const float ConnectionKeepAliveSendRate = 1.0f;
const float ConnectionConfirmSendRate = 0.1f;
const float ConnectionResponseSendRate = 0.1f;

namespace Spooder
{
	class Object;
	struct PacketInfo;
	class Packet;
	class PacketFactory;
	class Address;
	class Socket;

void SendPacket(Socket* aSocket, PacketFactory* aPacketFactory, const Address& aAddress, Packet* aPacket);
Packet* ReceivePacket(Socket* aSocket, PacketFactory* aPacketFactory, Address& aAddress);
int WritePacket(const PacketInfo& aInfo, Packet* aPacket, uint8_t* aBuffer, int aBufferSize, Object* aHeader = nullptr);
Packet* ReadPacket(const PacketInfo& aInfo, const uint8_t* aBuffer, int aBufferSize, Object* aHeader = nullptr, int* aErrorCode = nullptr);
static bool ourNetworkInitialized = false;
bool InitializeNetwork();
}
