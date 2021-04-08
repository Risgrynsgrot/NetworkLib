#include "Server.h"
#include <assert.h>
#include <cstdlib>



#include "Spooder/MessageFactory.h"
#include "Spooder/NetworkFunctions.h"
#include "Spooder/PacketTypes.h"

//namespace Spooder
//{
//
//	Server::Server()
//	{
//	}
//
//
//	Server::~Server()
//	{
//		closesocket(mySocketID);
//		WSACleanup();
//	}
//
//	void Server::Start()
//	{
//		int slen, recv_len;
//		char buf[BUFLEN];
//		WSADATA wsa;
//
//		slen = sizeof(myCurrentClient);
//		//Initialise winsock
//		printf("\nInitialising Winsock...");
//		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
//		{
//			printf("Failed. Error Code : %d", WSAGetLastError());
//			exit(EXIT_FAILURE);
//		}
//		printf("Initialised.\n");
//
//		//Create a socket
//		if ((mySocketID = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
//		{
//			printf("Could not create socket : %d", WSAGetLastError());
//		}
//		printf("Socket created.\n");
//
//		//Prepare the sockaddr_in structure
//		myServer.sin_family = AF_INET;
//		myServer.sin_addr.s_addr = INADDR_ANY;
//		myServer.sin_port = htons(PORT);
//
//		u_long ne = TRUE;
//		ioctlsocket(mySocketID, FIONBIO, &ne); //Non blocking
//
//		const int socketBufferSize = 64 * 1024;
//		setsockopt(mySocketID, SOL_SOCKET, SO_SNDBUF, (char*)&socketBufferSize, sizeof(socketBufferSize));
//		setsockopt(mySocketID, SOL_SOCKET, SO_RCVBUF, (char*)&socketBufferSize, sizeof(socketBufferSize));
//
//		//Bind
//		if (bind(mySocketID, (struct sockaddr*)&myServer, sizeof(myServer)) == SOCKET_ERROR)
//		{
//			printf("Bind failed with error code : %d", WSAGetLastError());
//			exit(EXIT_FAILURE);
//		}
//		printf("Bind done\n");
//
//
//	}
//
//	//bool Server::RecieveMessages(char aOutBuffer[BUFLEN])
//	//{
//	//	int slen, recv_len;
//	//	slen = sizeof(myCurrentClient);
//
//	//	fflush(stdout);
//
//	//	//clear the buffer by filling null, it might have previously received data
//	//	memset(aOutBuffer, '\0', BUFLEN);
//
//	//	if ((recv_len = recvfrom(mySocketID, aOutBuffer, BUFLEN, 0, (struct sockaddr*)&myCurrentClient, &slen)) == SOCKET_ERROR)
//	//	{
//	//		auto errorCode = WSAGetLastError();
//	//		if (errorCode != WSAEWOULDBLOCK)
//	//		{
//	//			printf("recvfrom() failed with error code : %d", errorCode);
//	//			exit(EXIT_FAILURE);
//	//		}
//	//	}
//	//	else

//	//	{
//	//		Recieve(aOutBuffer);
//	//		return true;
//	//	}
//
//	//	return false;
//
//	//}
//
//	void Server::Update(float aDeltaTime)
//	{
//		//mySendTimer += aDeltaTime;
//		//if (mySendTimer > Network::SendRate)
//		//{
//		//	SendQueue();
//		//	mySendTimer = 0;
//		//}
//	}
//
//	void Server::SendQueue()
//	{
//		//while (!myMessageQueue.empty())
//		//{
//		//	auto& message = myMessageQueue.front();
//		//	auto& data = message.myMessage;
//		//	if (message.mySendToEveryone)
//		//	{
//		//		SendToAllClientsRaw(data, message.mySenderID);
//		//	}
//		//	else

//		//	{
//		//		Network::SendRaw(data.myData, data.mySize, message.mySendTo, mySocketID);
//		//	}
//		//	myMessageQueue.pop();
//		//}
//	}
//
//	//bool Server::VerifyHandShake(const HandShakeMessage& aMessage)
//	//{
//	//	if (myUsers.find(aMessage.mySenderID) != myUsers.end())
//	//	{
//	//		return false;
//	//	}
//	//	return true;
//	//}
//
//	//void Server::Recieve(char* aMessage)
//	//{
//	//	BaseMessage baseMessage;
//	//	memcpy(&baseMessage, aMessage, sizeof(BaseMessage));
//
//	//	switch (baseMessage.myMessageType)
//	//	{
//	//	case MessageType::Handshake:
//	//	{
//	//		HandShakeMessage message;
//	//		memcpy(&message, aMessage, sizeof(HandShakeMessage));
//	//		if (VerifyHandShake(message))
//	//		{
//
//	//			Network::User user;
//	//			strcpy_s(user.myName, message.Get());
//	//			user.mySocket = myCurrentClient;
//
//	//			Network::JoinedMessage joinedMessage;
//	//			joinedMessage.SetClientName(message.Get());
//	//			joinedMessage.SetSenderID(message.mySenderID);
//
//	//			SendToAllClients(joinedMessage);
//
//	//			myUsers[message.mySenderID] = user;
//
//	//			OKMessage okMessage;
//	//			Network::Send(okMessage, myCurrentClient, mySocketID);
//	//			printf("Verified a client with name %s\n", user.myName);
//
//	//		}
//	//		break;
//	//	}
//	//	case MessageType::NoMessage:
//	//		break;
//	//	case MessageType::Text:
//	//	{
//	//		StringMessage message;
//	//		memcpy(&message, aMessage, sizeof(StringMessage));
//	//		char outMessage[message.GetSize()];
//	//		sprintf_s(outMessage, message.GetSize(), "%s: %s\n", myUsers.at(message.mySenderID).myName, message.Get());
//	//		printf("%s", outMessage);
//	//		strcpy_s(message.Get(), message.GetSize(), outMessage);
//	//		SendToAllClients(message);
//
//	//		break;
//	//	}
//	//	case MessageType::Disconnect:
//	//	{
//
//	//		StringMessage message;
//	//		char outMessage[message.GetSize()];
//	//		sprintf_s(outMessage, message.GetSize(), "%s disconnected\n", myUsers.at(baseMessage.mySenderID).myName);
//	//		printf("%s", outMessage);
//	//		strcpy_s(message.Get(), message.GetSize(), outMessage);
//
//	//		myUsers.erase(baseMessage.mySenderID);
//	//		SendToAllClients(message);
//	//		break;
//	//	}
//	//	case MessageType::SetPosition:
//	//	{
//	//		auto message = Network::ParseMessage<SetPositionMessage>(aMessage);
//	//		//printf("Position change: %f, %f", message.Get().x, message.Get().y);
//	//		SendToAllClients(message);
//	//	}
//	//	}
//	//}
//	//void Server::SendToAllClientsRaw(const RawMessage& aMessage, unsigned short aSenderID)
//	//{
//	//	for (auto user : myUsers)
//	//	{
//	//		if (user.first == aSenderID)
//	//		{
//	//			continue;
//	//		}
//
//	//		Network::SendRaw(aMessage.myData, aMessage.mySize, user.second.mySocket, mySocketID);
//	//	}
//	//}
//}

//void Spooder::Server::Start()
//{
//
//	int slen, recv_len;
//	char buf[BUFLEN];
//	WSADATA wsa;
//
//	slen = sizeof(myCurrentClient);
//	//Initialise winsock
//	printf("\nInitialising Winsock...");
//	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
//	{
//		printf("Failed. Error Code : %d", WSAGetLastError());
//		exit(EXIT_FAILURE);
//	}
//	printf("Initialised.\n");
//
//	//Create a socket
//	if ((mySocketID = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
//	{
//		printf("Could not create socket : %d", WSAGetLastError());
//	}
//	printf("Socket created.\n");
//
//	//Prepare the sockaddr_in structure
//	myServer.sin_family = AF_INET;
//	myServer.sin_addr.s_addr = INADDR_ANY;
//	myServer.sin_port = htons(PORT);
//
//	u_long ne = TRUE;
//	ioctlsocket(mySocketID, FIONBIO, &ne); //Non blocking
//
//	const int socketBufferSize = 64 * 1024;
//	setsockopt(mySocketID, SOL_SOCKET, SO_SNDBUF, (char*)&socketBufferSize, sizeof(socketBufferSize));
//	setsockopt(mySocketID, SOL_SOCKET, SO_RCVBUF, (char*)&socketBufferSize, sizeof(socketBufferSize));
//
//	//Bind
//	if (bind(mySocketID, (struct sockaddr*)&myServer, sizeof(myServer)) == SOCKET_ERROR)
//	{
//		printf("Bind failed with error code : %d", WSAGetLastError());
//		exit(EXIT_FAILURE);
//	}
//	printf("Bind done\n");
//}

Spooder::Server::Server(Socket& aSocket, PacketFactory& aPacketFactory)
{
	mySocket = &aSocket;
	myPacketFactory = &aPacketFactory;
	myNumberOfConnectedClients = 0;
	for (size_t i = 0; i < MaxClients; i++)
	{
		ResetClientState(i);
	}
}

int Spooder::Server::FindFreeClientIndex() const
{
	for (size_t i = 0; i < MaxClients; ++i)
	{
		if (!myIsClientConnected[i])
		{
			return i;
		}
	}
	return -1;
}

int Spooder::Server::FindExistingClientIndex(const Address & aAddress) const
{
	for (size_t i = 0; i < MaxClients; ++i)
	{
		if(!myIsClientConnected[i])
		{
			continue;
		}
		if (myClientAddresses[i] == aAddress)
		{
			return i;
		}
	}
	return -1;
}

void Spooder::Server::ResetClientState(int aClientIndex)
{
	assert(aClientIndex >= 0);
	assert(aClientIndex < MaxClients);

	myIsClientConnected[aClientIndex] = false;
	myClientAddresses[aClientIndex] = Address();
	myClientData[aClientIndex] = ServerClientData();
}

bool Spooder::Server::IsConnected(const Address& aAddress) const //Add salt
{
	for (int i = 0; i < MaxClients; ++i)
	{
		if(!myIsClientConnected[i])
		{
			continue;
		}
		if(myClientAddresses[i] == aAddress) //Add salt
		{
			return true;
		}
		return false;
	}
	return false;
}

void Spooder::Server::ProcessConnectionRequest(
	const Packets::ConnectionRequest& aPacket,
	const Address& aAddress,
	double aTime)
{
	char buffer[256];
	const char* addressString = aAddress.ToString(buffer, sizeof(buffer));
	printf("Processing connection request from: %s\n", addressString);

	if(myNumberOfConnectedClients == MaxClients)
	{
		printf("Connection denied: Server is full\n");
		Packets::ConnectionDenied* connectionDeniedPacket = (Packets::ConnectionDenied*)myPacketFactory->CreatePacket((PACKET_CONNECTION_DENIED));
		connectionDeniedPacket->myReason = Packets::CONNECTION_DENIED_SERVER_FULL;
		SendPacket(mySocket, myPacketFactory, aAddress, connectionDeniedPacket);
		return;
	}
	if(IsConnected(aAddress))
	{
		printf("Connection denied: already connected\n");
		Packets::ConnectionDenied* connectionDeniedPacket = (Packets::ConnectionDenied*)myPacketFactory->CreatePacket((PACKET_CONNECTION_DENIED));
		connectionDeniedPacket->myReason = Packets::CONNECTION_DENIED_ALREADY_CONNECTED;
		SendPacket(mySocket, myPacketFactory, aAddress, connectionDeniedPacket);
		return;
	}

	//Fixa salt
	if(true) //if havent sent salt in a while
	{
		printf("Sending connection challenge to %s\n", addressString);
		Packets::ConnectionChallenge* connectionChallengePacket = (Packets::ConnectionChallenge*)myPacketFactory->CreatePacket(PACKET_CONNECTION_CHALLENGE);
		SendPacket(mySocket, myPacketFactory, aAddress, connectionChallengePacket);
	}
}

void Spooder::Server::ProcessConnectionResponse(
	const Packets::ConnectionResponse& aPacket,
	const Address& aAddress,
	double aTime)
{
	const int existingClientIndex = FindExistingClientIndex(aAddress);
	if(existingClientIndex != -1)
	{
		assert(existingClientIndex >= 0);
		assert(existingClientIndex < MaxClients);

		if(myClientData[existingClientIndex].lastPacketSendTime + ConnectionConfirmSendRate < aTime)
		{
			Packets::ConnectionKeepAlive* connectionKeepAlivePacket = (Packets::ConnectionKeepAlive*)myPacketFactory->CreatePacket(PACKET_CONNECTION_KEEP_ALIVE);
			SendPacketToConnectedClient(existingClientIndex, connectionKeepAlivePacket, aTime);
		}
		return;
	}
	char buffer[256];
	const char* addressString = aAddress.ToString(buffer, sizeof(buffer));
	printf("Processing connection response from client %s\n", addressString); //add salt

	//Salt stuff

	if(myNumberOfConnectedClients == MaxClients)
	{
		printf("connection denied: Server is full\n");
		Packets::ConnectionDenied* connectionDeniedPacket = (Packets::ConnectionDenied*)myPacketFactory->CreatePacket(PACKET_CONNECTION_DENIED);
		connectionDeniedPacket->myReason = Packets::CONNECTION_DENIED_SERVER_FULL;
		SendPacket(mySocket, myPacketFactory, aAddress, connectionDeniedPacket);
		return;
	}

	const int clientIndex = FindFreeClientIndex();
	assert(clientIndex != -1);
	if (clientIndex == -1)
		return;
	ConnectClient(clientIndex, aAddress, aTime);
}

void Spooder::Server::ProcessConnectionKeepAlive(
	const Packets::ConnectionKeepAlive& aPacket,
	const Address& aAddress,
	double aTime)
{
	const int clientIndex = FindExistingClientIndex(aAddress); //Add salt
	if(clientIndex == -1)
	{
		return;
	}
	assert(clientIndex >= 0);
	assert(clientIndex < MaxClients);

	myClientData[clientIndex].lastPacketReceiveTime = aTime;
}

void Spooder::Server::ProcessConnectionDisconnect(
	const Packets::ConnectionDisconnect& aPacket,
	const Address& aAddress,
	double aTime)
{
	const int clientIndex = FindExistingClientIndex(aAddress); //Add salt
	if (clientIndex == -1)
		return;

	assert(clientIndex >= 0);
	assert(clientIndex < MaxClients);

	DisconnectClient(clientIndex, aTime);
}

bool Spooder::Server::IsClientConnected(int aClientIndex) const
{
	return myIsClientConnected[aClientIndex];
}

const Spooder::Address& Spooder::Server::GetClientAddress(int aClientIndex) const
{
	return myClientAddresses[aClientIndex];
}

void Spooder::Server::SendPackets(double aTime)
{
	for (int i = 0; i < MaxClients; ++i)
	{
		if (!myIsClientConnected[i])
		{
			continue;
		}
		if (myClientData[i].lastPacketSendTime + ConnectionKeepAliveSendRate > aTime)
		{
			return;
		}

		Packets::ConnectionKeepAlive* packet = (Packets::ConnectionKeepAlive*)myPacketFactory->CreatePacket(PACKET_CONNECTION_KEEP_ALIVE);
		SendPacketToConnectedClient(i, packet, aTime);
	}
}

void Spooder::Server::ReceivePackets(double aTime)
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
		case PACKET_CONNECTION_REQUEST:
			ProcessConnectionRequest(*(Packets::ConnectionRequest*)packet, address, aTime);
			break;
		case PACKET_CONNECTION_RESPONSE:
			ProcessConnectionResponse(*(Packets::ConnectionResponse*)packet, address, aTime);
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

void Spooder::Server::CheckForTimeOut(double aTime)
{
	for (int i = 0; i < MaxClients; i++)
	{
		if(!myIsClientConnected[i])
		{
			continue;
		}

		if(myClientData[i].lastPacketReceiveTime + KeepAliveTimeOut < aTime)
		{
			char buffer[256];
			const char* addressString = myClientAddresses[i].ToString(buffer, sizeof(buffer));
			printf("Client %d timed out (client address = %s)", i, addressString);
			DisconnectClient(i, aTime);
		}
	}
}

void Spooder::Server::SendPacketToConnectedClient(int aClientIndex, Packet* aPacket, double aTime)
{
	assert(aPacket);
	assert(aClientIndex >= 0);
	assert(aClientIndex < MaxClients);
	assert(myIsClientConnected[aClientIndex]);
	myClientData[aClientIndex].lastPacketSendTime = aTime;
	SendPacket(mySocket, myPacketFactory, myClientAddresses[aClientIndex], aPacket);
}

void Spooder::Server::ConnectClient(int aClientIndex, const Address& aAddress, double aTime)
{
	assert(myNumberOfConnectedClients >= 0);
	assert(myNumberOfConnectedClients < MaxClients);
	assert(!myIsClientConnected[aClientIndex]);

	myNumberOfConnectedClients++;

	myIsClientConnected[aClientIndex] = true;
	myClientAddresses[aClientIndex] = aAddress;

	myClientData[aClientIndex].address = aAddress;
	myClientData[aClientIndex].connectTime = aTime;
	myClientData[aClientIndex].lastPacketSendTime = aTime;
	myClientData[aClientIndex].lastPacketReceiveTime = aTime;

	char buffer[256];
	const char* addressString = aAddress.ToString(buffer, sizeof(buffer));
	printf("Client %d connected (client address = %s)", aClientIndex, addressString);

	Packets::ConnectionKeepAlive* connectionKeepAlivePacket = (Packets::ConnectionKeepAlive*)myPacketFactory->CreatePacket(PACKET_CONNECTION_KEEP_ALIVE);

	SendPacketToConnectedClient(aClientIndex, connectionKeepAlivePacket, aTime);
}

void Spooder::Server::DisconnectClient(int aClientIndex, double aTime)
{
	assert(aClientIndex >= 0);
	assert(aClientIndex < MaxClients);
	assert(myNumberOfConnectedClients > 0);
	assert(myIsClientConnected[aClientIndex]);

	char buffer[256];
	const char* addressString = myClientAddresses[aClientIndex].ToString(buffer, sizeof(buffer));
	printf("Client %d disconnected (client address = %s)", aClientIndex, addressString);

	Packets::ConnectionDisconnect* packet = (Packets::ConnectionDisconnect*)myPacketFactory->CreatePacket(PACKET_CONNECTION_DISCONNECT);

	SendPacketToConnectedClient(aClientIndex, packet, aTime);

	ResetClientState(aClientIndex);

	myNumberOfConnectedClients--;
}
