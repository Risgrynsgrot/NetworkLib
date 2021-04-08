#pragma once
#include "Spooder\CommonNetworkIncludes.h"
#include <queue>
#include <string>
#include <Spooder/Address.h>


enum class ConnectionState
{
	Disconnected,
	ConnectionRequest,
	ChallengeResponse,
	Connected,
	ConnectionRequestTimedOut,
	ChallengeResponseTimedOut,
	KeepAliveTimedOut,
	ServerFull,
	AlreadyConnected
};

namespace Spooder
{
namespace Packets {
struct ConnectionDisconnect;
struct ConnectionKeepAlive;
struct ConnectionDenied;
struct ConnectionChallenge;
}

class Packet;
	class PacketFactory;
	class Socket;

	class Client
	{
	public:
		Client(Socket& aSocket, PacketFactory& aPacketFactory);
		~Client();
		void Init();
		//void Start(const char* aIP);
		//void Start();
		void Shutdown();
		void Update(float aDeltaTime);
		void SendPackets(double aTime);
		void ReceivePackets(double aTime);
		void Connect(const Address& aAddress, double aTime);
		void Disconnect(double aTime);
		void CheckForTimeOut(double aTime);
	protected:
		void ResetConnectionData();
		void SendPacketToServer(Packet* aPacket, double aTime);
		void ProcessConnectionDenied(const Packets::ConnectionDenied& aPacket, const Address& aAddress, double aTime);
		void ProcessConnectionKeepAlive(const Packets::ConnectionKeepAlive& aPacket, const Address& aAddress, double aTime);
		void ProcessConnectionChallenge(const Packets::ConnectionChallenge& aPacket, const Address& aAddress, double aTime);
		void ProcessConnectionDisconnect(const Packets::ConnectionDisconnect& aPacket, const Address& aAddress, double aTime);
	private:
		Socket* mySocket;
		Address myServerAddress;
		ConnectionState myConnectionState = ConnectionState::Disconnected;
		PacketFactory* myPacketFactory;
		double myLastPacketSendTime;
		double myLastPacketReceiveTime;
	};
}