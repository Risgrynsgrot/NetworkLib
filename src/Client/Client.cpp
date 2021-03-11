#include "Client.h"
#include <time.h>
#include <random>
#include <iostream>
#include "Spooder/Packet.h"
#include "Spooder/Serializer.h"
#include "Spooder\PacketTypes.h"

using namespace Spooder;

Spooder::Client::Client()
{
}

Spooder::Client::~Client()
{
}

void Spooder::Client::Init()
{
}

void Spooder::Client::Start()
{
	printf("Welcome to the chat client, use /exit to quit gracefully\n");

	Spooder::TestPacket testData;
	for (size_t i = 0; i < 32; i++)
	{
		testData.myA[i] = i;
	}
	uint32_t buffer[BUFLEN];
	Spooder::BitWriter writer(buffer);
	testData.Write(writer);
	writer.Flush();

	Spooder::WriteStream stream(buffer, sizeof(buffer));
	float test = 3;

	Spooder::TestPacket testData2;

	Spooder::BitReader reader(buffer, sizeof(testData2) * 8);
	testData2.Read(reader);


	for (size_t i = 0; i < 4; i++)
	{
		printf("element %d: %d", i, buffer[i]);
	}

	srand(time(nullptr));
	mySenderID = rand();

	int s, slen = sizeof(mySocketAddressIn);
	char buf[BUFLEN];
	char message[BUFLEN];

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &myWSA) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");

	//create socket
	if ((mySocketID = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf("socket() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	u_long ne = TRUE;
	ioctlsocket(mySocketID, FIONBIO, &ne);

	const int socketBufferSize = 64 * 1024;
	setsockopt(mySocketID, SOL_SOCKET, SO_SNDBUF, (char*)&socketBufferSize, sizeof(socketBufferSize));
	setsockopt(mySocketID, SOL_SOCKET, SO_RCVBUF, (char*)&socketBufferSize, sizeof(socketBufferSize));

	//setup address structure
	memset((char*)&mySocketAddressIn, 0, sizeof(mySocketAddressIn));
	mySocketAddressIn.sin_family = AF_INET;
	mySocketAddressIn.sin_port = htons(PORT);

	printf("Enter IP: ");
	std::getline(std::cin, myIp);
	if (myIp == "" || myIp == "localhost")
	{
		myIp = "127.0.0.1";
	}

	printf("Enter name: ");
	std::getline(std::cin, myName);

	inet_pton(AF_INET, myIp.c_str(), &mySocketAddressIn.sin_addr.S_un.S_addr);

	//HandShakeMessage hsMessage;
	//hsMessage.SetSenderID(mySenderID);
	//hsMessage.Set(myName.c_str());
	//Network::Send(hsMessage, mySocketAddressIn, mySocketID);

	//start communication
	//for(int i = 0; i < 100; i++)
	bool verified = false;
	while (!verified)
	{
		memset(buf, '\0', BUFLEN);
		//try to receive some data, this is a blocking call
		if (recvfrom(mySocketID, buf, BUFLEN, 0, (struct sockaddr*)&mySocketAddressIn, &slen) == SOCKET_ERROR)
		{
			auto errorCode = WSAGetLastError();
			if (errorCode != WSAEWOULDBLOCK)
			{
				printf("recvfrom() failed with error code : %d", errorCode);
				exit(EXIT_FAILURE);
			}
		}
		//verified = IsOk(buf);
	}

	printf("Client verified!\n");
}

void Spooder::Client::Shutdown()
{
}

void Spooder::Client::Update(float aDeltaTime)
{
}
