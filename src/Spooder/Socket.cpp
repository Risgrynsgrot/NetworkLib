#include "Socket.h"

#include "CommonNetworkIncludes.h"
#include <cassert>


Spooder::Socket::Socket(uint16_t aPort, SocketType aType)
{
	//Assert if networking isn't setup
	myError = SOCKET_ERROR_NONE;

	mySocket = socket((aType == SOCKET_TYPE_IPV6) ? AF_INET6 : AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if(mySocket <= 0)
	{
		myError = SOCKET_ERROR_CREATE_FAILED;
		return;
	}

	if(aType == SOCKET_TYPE_IPV6)
	{
		int yes = 1;
		if(setsockopt(mySocket, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&yes, sizeof(yes)) != 0)
		{
			printf("failed to set ipv6 only sockopt\n");
			myError = SOCKET_ERROR_SOCKOPT_IPV6_ONLY_FAILED;
			return;
		}
	}

	if(aType == SOCKET_TYPE_IPV6)
	{
		sockaddr_in6 sockAddress;
		memset(&sockAddress, 0, sizeof(sockaddr_in6));
		sockAddress.sin6_family = AF_INET6;
		sockAddress.sin6_addr = in6addr_any;
		sockAddress.sin6_port = htons(aPort);
		if(bind(mySocket, (const sockaddr*)&sockAddress, sizeof(sockAddress)) < 0)
		{
			printf("Failed to bind to port! maybe you already have a client running on this computer?\n");
			myError = SOCKET_ERROR_BIND_IPV6_FAILED;
			return;
		}
	}
	else
	{
		sockaddr_in sockAddress;
		memset(&sockAddress, 0, sizeof(sockaddr_in));
		sockAddress.sin_family = AF_INET;
		sockAddress.sin_addr.S_un.S_addr = INADDR_ANY;
		sockAddress.sin_port = htons(aPort);
		if(bind(mySocket, (const sockaddr*)&sockAddress, sizeof(sockAddress)) < 0)
		{
			printf("Failed to bind to port! maybe you already have a client running on this computer?\n");
			myError = SOCKET_ERROR_BIND_IPV4_FAILED;
			return;
		}
	}

	myPort = aPort;

	if(myPort == 0)
	{
		if(aType == SOCKET_TYPE_IPV6)
		{
			struct sockaddr_in6 sin;
			socklen_t len = sizeof(sin);
			if(getsockname(mySocket, (struct sockaddr*)&sin, &len) == -1)
			{
				myError = SOCKET_ERROR_GET_SOCKNAME_IPV6_FAILED;
				return;
			}
			myPort = ntohs(sin.sin6_port);
		}
		else
		{
			struct sockaddr_in sin;
			socklen_t len = sizeof(sin);
			if(getsockname(mySocket, (struct sockaddr*)&sin, &len) == -1)
			{
				myError = SOCKET_ERROR_GET_SOCKNAME_IPV4_FAILED;
				return;
			}
			myPort = ntohs(sin.sin_port);
		}
	}

	DWORD nonBlocking = 1;
	if(ioctlsocket(mySocket, FIONBIO, &nonBlocking) != 0)
	{
		printf("failed to make socket non-blocking\n");
		myError = SOCKET_ERROR_SET_NON_BLOCKING_FAILED;
		return;
	}
	
}

Spooder::Socket::~Socket()
{
	closesocket(mySocket);
	mySocket = 0;
}

bool Spooder::Socket::IsError() const
{
	//printf("error code: %d\n", myError);
	return myError != SOCKET_ERROR_NONE;
}

int Spooder::Socket::GetError() const
{
	return myError;
}

bool Spooder::Socket::SendPacket(const Address& aAddress, const void* aPacketData, size_t aPacketBytes)
{
	assert(aPacketData);
	assert(aPacketBytes > 0);
	assert(aAddress.IsValid());
	assert(mySocket);
	assert(!IsError());

	bool result = false;
	if(aAddress.GetType() == AddressType::ipv6)
	{
		sockaddr_in6 socketAddress;
		memset(&socketAddress, 0, sizeof(socketAddress));
		socketAddress.sin6_family = AF_INET6;
		socketAddress.sin6_port = htons(aAddress.GetPort());
		memcpy(&socketAddress.sin6_addr, aAddress.GetAddress6(), sizeof(socketAddress.sin6_addr));

		size_t sentBytes = sendto(mySocket, (const char*)aPacketData, (int)aPacketBytes, 0, (sockaddr*)&socketAddress, sizeof(sockaddr_in6));
		result = sentBytes == aPacketBytes;
	}
	else if(aAddress.GetType() == AddressType::ipv4)
	{
		sockaddr_in socketAddress;
		memset(&socketAddress, 0, sizeof(socketAddress));
		socketAddress.sin_family = AF_INET;
		socketAddress.sin_addr.S_un.S_addr = aAddress.GetAddress4();
		socketAddress.sin_port = htons((unsigned short)aAddress.GetPort());

		size_t sentBytes = sendto(mySocket, (const char*)aPacketData, (int)aPacketBytes, 0, (sockaddr*)&socketAddress, sizeof(sockaddr_in));
		result = sentBytes == aPacketBytes;
	}
	return result;
}

int Spooder::Socket::ReceivePacket(Address& aFrom, void* aPacketData, int aMaxPacketSize)
{
	assert(mySocket);
	assert(aPacketData);
	assert(aMaxPacketSize > 0);

	typedef int socklen_t;

	sockaddr_storage sockaddrFrom;
	socklen_t fromLength = sizeof(sockaddrFrom);

	int result = recvfrom(mySocket, (char*)aPacketData, aMaxPacketSize, 0, (sockaddr*)&sockaddrFrom, &fromLength);
	if(result == SOCKET_ERROR)
	{
		int error = WSAGetLastError();

		if(error == WSAEWOULDBLOCK)
		{
			return 0;
		}
		printf("recvfrom failed %d\n", error);
		return 0;
	}

	//printf("actually received something\n");
	aFrom = Address(sockaddrFrom);
	assert(result >= 0);
	const int bytesRead = result;
	return bytesRead;
}
