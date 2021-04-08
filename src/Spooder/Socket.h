#pragma once
#include <cstdint>

#include "Address.h"

namespace Spooder
{
	enum SocketType
	{
		SOCKET_TYPE_IPV4,
		SOCKET_TYPE_IPV6
	};

	enum SocketError
	{
		SOCKET_ERROR_NONE,
		SOCKET_ERROR_CREATE_FAILED,
		SOCKET_ERROR_SET_NON_BLOCKING_FAILED,
		SOCKET_ERROR_SOCKOPT_IPV6_ONLY_FAILED,
		SOCKET_ERROR_BIND_IPV4_FAILED,
		SOCKET_ERROR_BIND_IPV6_FAILED,
		SOCKET_ERROR_GET_SOCKNAME_IPV4_FAILED,
		SOCKET_ERROR_GET_SOCKNAME_IPV6_FAILED
	};

	typedef uint64_t SocketHandle;

	class Socket
	{
	public:
		Socket(uint16_t aPort, SocketType aType = SOCKET_TYPE_IPV4);
		~Socket();
		bool IsError() const;
		int GetError() const;
		bool SendPacket(const Address& aAddress, const void* aPacketData, size_t aPacketBytes);
		int ReceivePacket(Address& aFrom, void* aPacketData, int aMaxPacketSize);
	private:
		int myError;
		uint16_t myPort;
		SocketHandle mySocket;
	};
}