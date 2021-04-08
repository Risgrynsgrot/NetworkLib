#include "Client.h"
#include <time.h>
#include <random>
#include <iostream>

#include "Spooder/MessageFactory.h"
#include "Spooder/Serializer.h"
#include "Spooder\PacketTypes.h"
#include "Spooder/NetworkFunctions.h"

using namespace Spooder;

Client::Client(Socket& aSocket, PacketFactory& aPacketFactory)
{
	mySocket = &aSocket;
	myPacketFactory = &aPacketFactory;
	ResetConnectionData();
}

Spooder::Client::~Client()
{
}

void Spooder::Client::Init()
{
}

//void Spooder::Client::Start(const char* aIP)
//{
//
//	if (aIP == "" || aIP == "localhost")
//	{
//		myServerAddress = Address("127.0.0.1");
//	}
//	else
//	{
//		myServerAddress = Address(aIP);
//	}
//
//	int slen = sizeof(mySocketAddressIn);
//	char buf[BUFLEN];
//	char message[BUFLEN];
//
//	//Initialise winSock
//	printf("\nInitializing WinSock...");
//	if (WSAStartup(MAKEWORD(2, 2), &myWSA) != 0)
//	{
//		printf("Failed. Error Code : %d", WSAGetLastError());
//		exit(EXIT_FAILURE);
//	}
//	printf("Initialized.\n");
//
//	//create socket
//	if ((mySocketID = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
//	{
//		printf("socket() failed with error code : %d", WSAGetLastError());
//		exit(EXIT_FAILURE);
//	}
//
//	u_long ne = TRUE;
//	ioctlsocket(mySocketID, FIONBIO, &ne);
//
//	const int socketBufferSize = 64 * 1024;
//	setsockopt(mySocketID, SOL_SOCKET, SO_SNDBUF, (char*)&socketBufferSize, sizeof(socketBufferSize));
//	setsockopt(mySocketID, SOL_SOCKET, SO_RCVBUF, (char*)&socketBufferSize, sizeof(socketBufferSize));
//
//	//setup address structure
//	memset((char*)&mySocketAddressIn, 0, sizeof(mySocketAddressIn));
//	mySocketAddressIn.sin_family = AF_INET;
//	mySocketAddressIn.sin_port = htons(myServerAddress.GetPort());
//	mySocketAddressIn.sin_addr.S_un.S_addr = myServerAddress.GetAddress4();
//
//	//inet_pton(AF_INET, myServerIP.c_str(), &mySocketAddressIn.sin_addr.S_un.S_addr);
//
//	//HandShakeMessage hsMessage;
//	//hsMessage.SetSenderID(mySenderID);
//	//hsMessage.Set(myName.c_str());
//	//Network::Send(hsMessage, mySocketAddressIn, mySocketID);
//	myConnectionState = ConnectionState::ConnectionRequest;
//
//	Spooder::ConnectionRequest connectionRequest = 
//
//	bool verified = false;
//	while (!verified)
//	{
//		memset(buf, '\0', BUFLEN);
//		//try to receive some data, this is a blocking call
//		if (recvfrom(mySocketID, buf, BUFLEN, 0, (struct sockaddr*)&mySocketAddressIn, &slen) == SOCKET_ERROR)
//		{
//			const int errorCode = WSAGetLastError();
//			if (errorCode != WSAEWOULDBLOCK)
//			{
//				printf("recvfrom() failed with error code : %d", errorCode);
//				exit(EXIT_FAILURE);
//			}
//		}
//		//verified = IsOk(buf);
//	}
//	myConnectionState = ConnectionState::Connected;
//
//	printf("Client verified!\n");
//}

//void Spooder::Client::Start()
//{
//	//printf("Welcome to the chat client, use /exit to quit gracefully\n");
//
//	//Spooder::TestPacket testData;
//	//for (size_t i = 0; i < 32; i++)
//	//{
//	//	testData.myA[i] = i;
//	//}
//	//uint32_t buffer[BUFLEN];
//	//Spooder::BitWriter writer(buffer);
//	//testData.Write(writer);
//	//writer.Flush();
//
//	//Spooder::WriteStream stream(buffer, sizeof(buffer));
//	//float test = 3;
//
//	//Spooder::TestPacket testData2;
//
//	//Spooder::BitReader reader(buffer, sizeof(testData2) * 8);
//	//testData2.Read(reader);
//
//
//	//for (size_t i = 0; i < 4; i++)
//	//{
//	//	printf("element %d: %d", i, buffer[i]);
//	//}
//
//	//srand(time(nullptr));
//	//mySenderID = rand();
//
//	//int s, slen = sizeof(mySocketAddressIn);
//	//char buf[BUFLEN];
//	//char message[BUFLEN];
//
//	////Initialise winsock
//	//printf("\nInitialising Winsock...");
//	//if (WSAStartup(MAKEWORD(2, 2), &myWSA) != 0)
//	//{
//	//	printf("Failed. Error Code : %d", WSAGetLastError());
//	//	exit(EXIT_FAILURE);
//	//}
//	//printf("Initialised.\n");
//
//	////create socket
//	//if ((mySocketID = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
//	//{
//	//	printf("socket() failed with error code : %d", WSAGetLastError());
//	//	exit(EXIT_FAILURE);
//	//}
//
//	//u_long ne = TRUE;
//	//ioctlsocket(mySocketID, FIONBIO, &ne);
//
//	//const int socketBufferSize = 64 * 1024;
//	//setsockopt(mySocketID, SOL_SOCKET, SO_SNDBUF, (char*)&socketBufferSize, sizeof(socketBufferSize));
//	//setsockopt(mySocketID, SOL_SOCKET, SO_RCVBUF, (char*)&socketBufferSize, sizeof(socketBufferSize));
//
//	////setup address structure
//	//memset((char*)&mySocketAddressIn, 0, sizeof(mySocketAddressIn));
//	//mySocketAddressIn.sin_family = AF_INET;
//	//mySocketAddressIn.sin_port = htons(PORT);
//
//	//printf("Enter IP: ");
//	//std::getline(std::cin, myServerIP);
//	//if (myServerIP == "" || myServerIP == "localhost")
//	//{
//	//	myServerIP = "127.0.0.1";
//	//}
//
//	//printf("Enter name: ");
//	//std::getline(std::cin, myName);
//
//	//inet_pton(AF_INET, myServerIP.c_str(), &mySocketAddressIn.sin_addr.S_un.S_addr);
//
//	////HandShakeMessage hsMessage;
//	////hsMessage.SetSenderID(mySenderID);
//	////hsMessage.Set(myName.c_str());
//	////Network::Send(hsMessage, mySocketAddressIn, mySocketID);
//
//	////start communication
//	////for(int i = 0; i < 100; i++)
//	//bool verified = false;
//	//while (!verified)
//	//{
//	//	memset(buf, '\0', BUFLEN);
//	//	//try to receive some data, this is a blocking call
//	//	if (recvfrom(mySocketID, buf, BUFLEN, 0, (struct sockaddr*)&mySocketAddressIn, &slen) == SOCKET_ERROR)
//	//	{
//	//		auto errorCode = WSAGetLastError();
//	//		if (errorCode != WSAEWOULDBLOCK)
//	//		{
//	//			printf("recvfrom() failed with error code : %d", errorCode);
//	//			exit(EXIT_FAILURE);
//	//		}
//	//	}
//	//	//verified = IsOk(buf);
//	//}
//
//	//printf("Client verified!\n");
//}

void Spooder::Client::Shutdown()
{
}

void Spooder::Client::Update(float aDeltaTime)
{
}

void Client::SendPackets(double aTime)
{
	switch(myConnectionState)
	{
	case ConnectionState::ConnectionRequest:
	{
		char buffer[256];
		const char* addressString = myServerAddress.ToString(buffer, sizeof(buffer));
		printf("ConnectionRequest to %s\n", addressString);

		Packets::ConnectionRequest* packet = (Packets::ConnectionRequest*)myPacketFactory->CreatePacket(PACKET_CONNECTION_REQUEST);

		SendPacketToServer(packet, aTime);
		break;
	}
	case ConnectionState::ChallengeResponse:
	{
		
		if(myLastPacketSendTime + ConnectionResponseSendRate > aTime)
		{
			return;
		}

		char buffer[256];
		const char* addressString = myServerAddress.ToString(buffer, sizeof(buffer));
		printf("client sending challenge response to server: %s\n", addressString);

		Packets::ConnectionResponse* packet = (Packets::ConnectionResponse*)myPacketFactory->CreatePacket(PACKET_CONNECTION_RESPONSE);
		//add salt

		SendPacketToServer(packet, aTime);
		break;
	}
	case ConnectionState::Connected:
	{
		if(myLastPacketSendTime + ConnectionKeepAliveSendRate > aTime)
		{
			return;
		}

		Packets::ConnectionKeepAlive* packet = (Packets::ConnectionKeepAlive*)myPacketFactory->CreatePacket(PACKET_CONNECTION_KEEP_ALIVE);
		//add salt
		SendPacketToServer(packet, aTime);
		break;
	}
	default:
		break;
	}
}

void Client::ReceivePackets(double aTime)
{
	while(true)
	{
		Address address;
		Packet* packet = ReceivePacket(mySocket, myPacketFactory, address);
		if (!packet)
		{
			break;
		}

		switch (packet->GetType())
		{
		case PACKET_CONNECTION_DENIED:
			ProcessConnectionDenied(*(Packets::ConnectionDenied*)packet, address, aTime);
			break;
		case PACKET_CONNECTION_CHALLENGE:
			ProcessConnectionChallenge(*(Packets::ConnectionChallenge*)packet, address, aTime);
			break;
		case PACKET_CONNECTION_KEEP_ALIVE:
			ProcessConnectionKeepAlive(*(Packets::ConnectionKeepAlive*)packet, address, aTime);
			break;
		case PACKET_CONNECTION_DISCONNECT:
			ProcessConnectionDisconnect(*(Packets::ConnectionDisconnect*)packet, address, aTime);
			break;
		default:
			break;
		}
		myPacketFactory->DestroyPacket(packet);
	}
}

void Client::Connect(const Address& aAddress, double aTime)
{
	Disconnect(aTime);
	myServerAddress = aAddress;
	myConnectionState = ConnectionState::ConnectionRequest;
	myLastPacketSendTime = aTime - 1.0;
	myLastPacketReceiveTime = aTime;
}

void Client::Disconnect(double aTime)
{
	if(myConnectionState == ConnectionState::Connected)
	{
		printf("client-side disconnect\n");  // Something something salt dunno how to do that yet
		Packets::ConnectionDisconnect* packet =
		    (Packets::ConnectionDisconnect*)myPacketFactory->CreatePacket(PACKET_CONNECTION_DISCONNECT);
		SendPacketToServer(packet, aTime);
	}
	ResetConnectionData();
}

void Client::CheckForTimeOut(double aTime)
{
	switch(myConnectionState)
	{
	case ConnectionState::ConnectionRequest:
	{
		if(myLastPacketReceiveTime + ConnectionRequestTimeOut < aTime)
		{
			printf("Connection request to server timed out\n");
			myConnectionState = ConnectionState::ConnectionRequestTimedOut;
			return;
		}
		break;
	}
	case ConnectionState::ChallengeResponse:
	{
		if(myLastPacketReceiveTime + ChallengeResponseTimeOut < aTime)
		{
			printf("Challenge response to server timed out\n");
			myConnectionState = ConnectionState::ChallengeResponseTimedOut;
			return;
		}
		break;
	}
	case ConnectionState::Connected:
	{
		if(myLastPacketReceiveTime + KeepAliveTimeOut < aTime)
		{
			printf("Keep alive timed out\n");
			myConnectionState = ConnectionState::KeepAliveTimedOut;
			Disconnect(aTime);
			return;
		}
		break;
	}
	default:
		break;
	}
}

void Client::ResetConnectionData()
{
	myServerAddress   = Address();
	myConnectionState = ConnectionState::Disconnected;
}

void Client::SendPacketToServer(Packet* aPacket, double aTime)
{
	assert(myConnectionState != ConnectionState::Disconnected);
	assert(myServerAddress.IsValid());

	SendPacket(mySocket, myPacketFactory, myServerAddress, aPacket);

	myLastPacketSendTime = aTime;
}

void Client::ProcessConnectionDenied(const Packets::ConnectionDenied& aPacket, const Address& aAddress, double aTime)
{
	if(myConnectionState != ConnectionState::ConnectionRequest)
	{
		return;
	}
	if(aAddress != myServerAddress)
	{
		return;
	}

	char buffer[256];
	const char* addressString = aAddress.ToString(buffer, sizeof(buffer));
	if(aPacket.myReason == Packets::CONNECTION_DENIED_SERVER_FULL)
	{
		printf("Client received connection denied from server: %s (server is full)\n", addressString);
		myConnectionState = ConnectionState::ServerFull;
	}
	else if(aPacket.myReason == Packets::CONNECTION_DENIED_ALREADY_CONNECTED)
	{
		printf("Client received connection denied from server: %s (already connected)\n", addressString);
		myConnectionState = ConnectionState::AlreadyConnected;
	}
}

void Client::ProcessConnectionKeepAlive(
	const Packets::ConnectionKeepAlive& aPacket,
	const Address& aAddress,
	double aTime)
{
	if((int)myConnectionState < (int)ConnectionState::ChallengeResponse)
	{
		return;
	}
	if(aAddress != myServerAddress)
	{
		return;
	}
	if(myConnectionState == ConnectionState::ChallengeResponse)
	{
		char buffer[256];
		const char* addressString = aAddress.ToString(buffer, sizeof(buffer));
		printf("Client is now connected to server: %s\n", addressString);
		myConnectionState = ConnectionState::Connected;
	}
	myLastPacketReceiveTime = aTime;
}

void Client::ProcessConnectionChallenge(
	const Packets::ConnectionChallenge& aPacket,
	const Address& aAddress,
	double aTime)
{
	if(myConnectionState != ConnectionState::ConnectionRequest)
	{
		return;
	}
	//Add salt stuff
	if(aAddress != myServerAddress)
	{
		return;
	}

	char buffer[256];
	const char* addressString = aAddress.ToString(buffer, sizeof(buffer));
	printf("Client received connection challenge from server: %s\n", addressString); //Add salt here also

	myConnectionState = ConnectionState::ChallengeResponse;
	myLastPacketReceiveTime = aTime;
}

void Client::ProcessConnectionDisconnect(
	const Packets::ConnectionDisconnect& aPacket,
	const Address& aAddress,
	double aTime)
{
	if (myConnectionState != ConnectionState::Connected)
	{
		return;
	}
	if(aAddress != myServerAddress)
	{
		return;
	}
	Disconnect(aTime);
}
