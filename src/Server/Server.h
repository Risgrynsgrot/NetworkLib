#pragma once
#include "Spooder/CommonNetworkIncludes.h"
#include <cstdint>

#include "Spooder/Address.h"
#include "Spooder/PacketTypes.h"

#define BUFLEN 512



namespace Spooder
{
namespace Packets {
struct ConnectionRequest;
}


class PacketFactory;
class Socket;

const int MaxClients = 1;

struct ServerClientData
{
	Address address;
	double connectTime;
	double lastPacketSendTime;
	double lastPacketReceiveTime;
};

	class Server
	{
	public:
		Server(Socket& aSocket, PacketFactory& aPacketFactory);
		//void Start();
		bool IsClientConnected(int aClientIndex) const;
		bool IsConnected(const Address& aAddress) const;
		const Address& GetClientAddress(int aClientIndex) const;
		void SendPackets(double aTime);
		void ReceivePackets(double aTime);
		void CheckForTimeOut(double aTime);
		void SendPacketToConnectedClient(int aClientIndex, Packet* aPacket, double aTime);
		void ConnectClient(int aClientIndex, const Address& aAddress, double aTime); //Add salt and challenge later, which we want for like everything hehe
		void DisconnectClient(int aClientIndex, double aTime);
	protected:
		int FindFreeClientIndex() const;
		int FindExistingClientIndex(const Address& aAddress) const;
		void ResetClientState(int aClientIndex);
	private:
		void ProcessConnectionRequest(const Packets::ConnectionRequest& aPacket, const Address& aAddress, double aTime);
		void ProcessConnectionResponse(const Packets::ConnectionResponse& aPacket, const Address& aAddress, double aTime);
		void ProcessConnectionKeepAlive(const Packets::ConnectionKeepAlive& aPacket, const Address& aAddress, double aTime);
		void ProcessConnectionDisconnect(const Packets::ConnectionDisconnect& aPacket, const Address& aAddress, double aTime);
		
		int myNumberOfConnectedClients;
		bool myIsClientConnected[MaxClients];
		Address myClientAddresses[MaxClients];
		Socket* mySocket;
		PacketFactory* myPacketFactory;
		ServerClientData myClientData[MaxClients];
	};

}







//#include "Spooder\CommonNetworkIncludes.h"
//#include <unordered_map>
//#include <queue>
//
//#define BUFLEN 512
//#define PORT 6174
//
//namespace Spooder
//{
//	struct ServerMessage
//	{
//		sockaddr_in mySendTo;
//		unsigned short mySenderID;
//		bool mySendToEveryone;
//		//RawMessage myMessage;
//	};
//	class Server
//	{
//	public:
//		Server();
//		~Server();
//		void Start();
//		bool RecieveMessages(char aOutBuffer[BUFLEN]);
//		template <class T>
//		void Send(const T& aMessage);
//		template <class T>
//		void Send(const T& aMessage, unsigned short aSenderID);
//
//		void Update(float aDeltaTime);
//	private:
//		void SendQueue();
//		//bool VerifyHandShake(const HandShakeMessage& aMessage);
//		void Recieve(char* aMessage);
//		template <class T>
//		void SendToAllClients(const T& aMessage);
//		//void SendToAllClientsRaw(const RawMessage& aMessage, unsigned short aSenderID);
//		SOCKET mySocketID;
//		sockaddr_in myServer;
//		sockaddr_in myCurrentClient;
//		//std::unordered_map<unsigned short, Network::User> myUsers;
//		float mySendTimer = 0;
//		std::queue<ServerMessage> myMessageQueue;
//	};
//
//	template<class T>
//	inline void Server::Send(const T& aMessage)
//	{
//		RawMessage message;
//		memcpy(message.myData, &aMessage, sizeof(aMessage));
//		message.mySize = sizeof(aMessage);
//		message.myData[message.mySize] = '\0';
//
//		ServerMessage serverMessage;
//		serverMessage.myMessage = message;
//		serverMessage.mySendToEveryone = true;
//		serverMessage.mySenderID = aMessage.mySenderID;
//		myMessageQueue.push(serverMessage);
//	}
//
//	template<class T>
//	inline void Server::Send(const T& aMessage, unsigned short aSenderID)
//	{
//		Network::User& user = myUsers.at(aSenderID);
//		RawMessage message;
//		memcpy(message.myData, &aMessage, sizeof(aMessage));
//		message.mySize = sizeof(aMessage);
//		message.myData[message.mySize] = '\0';
//
//		ServerMessage serverMessage;
//		serverMessage.myMessage = message;
//		serverMessage.mySendToEveryone = false;
//		serverMessage.mySenderID = aMessage.mySenderID;
//		serverMessage.mySendTo = user.mySocket;
//		myMessageQueue.push(serverMessage);
//	}
//
//	template<class T>
//	inline void Server::SendToAllClients(const T& aMessage)
//	{
//		for (auto user : myUsers)
//		{
//			if (user.first == aMessage.mySenderID)
//			{
//				continue;
//			}
//
//			Network::Send(aMessage, user.second.mySocket, mySocketID);
//		}
//	}
//}
