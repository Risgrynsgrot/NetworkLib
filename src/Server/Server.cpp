#include "Server.h"

namespace Spooder
{

	Server::Server()
	{
	}


	Server::~Server()
	{
		closesocket(mySocketID);
		WSACleanup();
	}

	void Server::Start()
	{
		int slen, recv_len;
		char buf[BUFLEN];
		WSADATA wsa;

		slen = sizeof(myCurrentClient);
		//Initialise winsock
		printf("\nInitialising Winsock...");
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		{
			printf("Failed. Error Code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		printf("Initialised.\n");

		//Create a socket
		if ((mySocketID = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
		{
			printf("Could not create socket : %d", WSAGetLastError());
		}
		printf("Socket created.\n");

		//Prepare the sockaddr_in structure
		myServer.sin_family = AF_INET;
		myServer.sin_addr.s_addr = INADDR_ANY;
		myServer.sin_port = htons(PORT);

		u_long ne = TRUE;
		ioctlsocket(mySocketID, FIONBIO, &ne); //Non blocking

		const int socketBufferSize = 64 * 1024;
		setsockopt(mySocketID, SOL_SOCKET, SO_SNDBUF, (char*)&socketBufferSize, sizeof(socketBufferSize));
		setsockopt(mySocketID, SOL_SOCKET, SO_RCVBUF, (char*)&socketBufferSize, sizeof(socketBufferSize));

		//Bind
		if (bind(mySocketID, (struct sockaddr*)&myServer, sizeof(myServer)) == SOCKET_ERROR)
		{
			printf("Bind failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		printf("Bind done\n");


	}

	//bool Server::RecieveMessages(char aOutBuffer[BUFLEN])
	//{
	//	int slen, recv_len;
	//	slen = sizeof(myCurrentClient);

	//	fflush(stdout);

	//	//clear the buffer by filling null, it might have previously received data
	//	memset(aOutBuffer, '\0', BUFLEN);

	//	if ((recv_len = recvfrom(mySocketID, aOutBuffer, BUFLEN, 0, (struct sockaddr*)&myCurrentClient, &slen)) == SOCKET_ERROR)
	//	{
	//		auto errorCode = WSAGetLastError();
	//		if (errorCode != WSAEWOULDBLOCK)
	//		{
	//			printf("recvfrom() failed with error code : %d", errorCode);
	//			exit(EXIT_FAILURE);
	//		}
	//	}
	//	else
	//	{
	//		Recieve(aOutBuffer);
	//		return true;
	//	}

	//	return false;

	//}

	void Server::Update(float aDeltaTime)
	{
		//mySendTimer += aDeltaTime;
		//if (mySendTimer > Network::SendRate)
		//{
		//	SendQueue();
		//	mySendTimer = 0;
		//}
	}

	void Server::SendQueue()
	{
		//while (!myMessageQueue.empty())
		//{
		//	auto& message = myMessageQueue.front();
		//	auto& data = message.myMessage;
		//	if (message.mySendToEveryone)
		//	{
		//		SendToAllClientsRaw(data, message.mySenderID);
		//	}
		//	else
		//	{
		//		Network::SendRaw(data.myData, data.mySize, message.mySendTo, mySocketID);
		//	}
		//	myMessageQueue.pop();
		//}
	}

	//bool Server::VerifyHandShake(const HandShakeMessage& aMessage)
	//{
	//	if (myUsers.find(aMessage.mySenderID) != myUsers.end())
	//	{
	//		return false;
	//	}
	//	return true;
	//}

	//void Server::Recieve(char* aMessage)
	//{
	//	BaseMessage baseMessage;
	//	memcpy(&baseMessage, aMessage, sizeof(BaseMessage));

	//	switch (baseMessage.myMessageType)
	//	{
	//	case MessageType::Handshake:
	//	{
	//		HandShakeMessage message;
	//		memcpy(&message, aMessage, sizeof(HandShakeMessage));
	//		if (VerifyHandShake(message))
	//		{

	//			Network::User user;
	//			strcpy_s(user.myName, message.Get());
	//			user.mySocket = myCurrentClient;

	//			Network::JoinedMessage joinedMessage;
	//			joinedMessage.SetClientName(message.Get());
	//			joinedMessage.SetSenderID(message.mySenderID);

	//			SendToAllClients(joinedMessage);

	//			myUsers[message.mySenderID] = user;

	//			OKMessage okMessage;
	//			Network::Send(okMessage, myCurrentClient, mySocketID);
	//			printf("Verified a client with name %s\n", user.myName);

	//		}
	//		break;
	//	}
	//	case MessageType::NoMessage:
	//		break;
	//	case MessageType::Text:
	//	{
	//		StringMessage message;
	//		memcpy(&message, aMessage, sizeof(StringMessage));
	//		char outMessage[message.GetSize()];
	//		sprintf_s(outMessage, message.GetSize(), "%s: %s\n", myUsers.at(message.mySenderID).myName, message.Get());
	//		printf("%s", outMessage);
	//		strcpy_s(message.Get(), message.GetSize(), outMessage);
	//		SendToAllClients(message);

	//		break;
	//	}
	//	case MessageType::Disconnect:
	//	{

	//		StringMessage message;
	//		char outMessage[message.GetSize()];
	//		sprintf_s(outMessage, message.GetSize(), "%s disconnected\n", myUsers.at(baseMessage.mySenderID).myName);
	//		printf("%s", outMessage);
	//		strcpy_s(message.Get(), message.GetSize(), outMessage);

	//		myUsers.erase(baseMessage.mySenderID);
	//		SendToAllClients(message);
	//		break;
	//	}
	//	case MessageType::SetPosition:
	//	{
	//		auto message = Network::ParseMessage<SetPositionMessage>(aMessage);
	//		//printf("Position change: %f, %f", message.Get().x, message.Get().y);
	//		SendToAllClients(message);
	//	}
	//	}
	//}
	//void Server::SendToAllClientsRaw(const RawMessage& aMessage, unsigned short aSenderID)
	//{
	//	for (auto user : myUsers)
	//	{
	//		if (user.first == aSenderID)
	//		{
	//			continue;
	//		}

	//		Network::SendRaw(aMessage.myData, aMessage.mySize, user.second.mySocket, mySocketID);
	//	}
	//}
}