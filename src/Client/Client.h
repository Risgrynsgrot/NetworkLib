#pragma once
#include "Spooder\CommonNetworkIncludes.h"
#include <queue>
#include <string>

#define BUFLEN 512	//Max length of buffer
#define PORT 6174	//The port on which to listen for incoming data

namespace Spooder
{
	class Client
	{
	public:
		Client();
		~Client();
		void Init();
		void Start();
		void Shutdown();
		void Update(float aDeltaTime);
	private:
		WSADATA myWSA;
		struct sockaddr_in mySocketAddressIn;
		int mySocketID = -1;
		std::string myIp = "127.0.0.1";
		std::string myName = "big clive";
		unsigned short mySenderID;
		bool myIsRunning = true;
		float mySendTimer = 0;
		//std::queue<RawMessage> myMessageQueue;
	};
}