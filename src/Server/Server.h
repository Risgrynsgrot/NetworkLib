#pragma once
#include "Spooder\CommonNetworkIncludes.h"
#include <unordered_map>
#include <queue>

#define BUFLEN 512
#define PORT 6174

namespace Spooder
{
	struct ServerMessage
	{
		sockaddr_in mySendTo;
		unsigned short mySenderID;
		bool mySendToEveryone;
		//RawMessage myMessage;
	};
	class Server
	{
	public:
		Server();
		~Server();
		void Start();
		bool RecieveMessages(char aOutBuffer[BUFLEN]);
		template <class T>
		void Send(const T& aMessage);
		template <class T>
		void Send(const T& aMessage, unsigned short aSenderID);

		void Update(float aDeltaTime);
	private:
		void SendQueue();
		//bool VerifyHandShake(const HandShakeMessage& aMessage);
		void Recieve(char* aMessage);
		template <class T>
		void SendToAllClients(const T& aMessage);
		//void SendToAllClientsRaw(const RawMessage& aMessage, unsigned short aSenderID);
		SOCKET mySocketID;
		sockaddr_in myServer;
		sockaddr_in myCurrentClient;
		//std::unordered_map<unsigned short, Network::User> myUsers;
		float mySendTimer = 0;
		std::queue<ServerMessage> myMessageQueue;
	};

	template<class T>
	inline void Server::Send(const T& aMessage)
	{
		RawMessage message;
		memcpy(message.myData, &aMessage, sizeof(aMessage));
		message.mySize = sizeof(aMessage);
		message.myData[message.mySize] = '\0';

		ServerMessage serverMessage;
		serverMessage.myMessage = message;
		serverMessage.mySendToEveryone = true;
		serverMessage.mySenderID = aMessage.mySenderID;
		myMessageQueue.push(serverMessage);
	}

	template<class T>
	inline void Server::Send(const T& aMessage, unsigned short aSenderID)
	{
		Network::User& user = myUsers.at(aSenderID);
		RawMessage message;
		memcpy(message.myData, &aMessage, sizeof(aMessage));
		message.mySize = sizeof(aMessage);
		message.myData[message.mySize] = '\0';

		ServerMessage serverMessage;
		serverMessage.myMessage = message;
		serverMessage.mySendToEveryone = false;
		serverMessage.mySenderID = aMessage.mySenderID;
		serverMessage.mySendTo = user.mySocket;
		myMessageQueue.push(serverMessage);
	}

	template<class T>
	inline void Server::SendToAllClients(const T& aMessage)
	{
		for (auto user : myUsers)
		{
			if (user.first == aMessage.mySenderID)
			{
				continue;
			}

			Network::Send(aMessage, user.second.mySocket, mySocketID);
		}
	}
}
