#include "Address.h"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include "CommonNetworkIncludes.h"

void Spooder::Address::Clear()
{
	myType = AddressType::Undefined;
	memset(myIpv6Address, 0, sizeof(myIpv6Address));
	myPort = 0;
}

void Spooder::Address::Parse(const char* aAddress)
{
	//IPV6
	assert(aAddress);

	char buffer[256];
	char* address = buffer;
	strncpy(address, aAddress, 255);
	address[255] = '\0';

	int addressLength = (int)strlen(address);
	myPort = 0;
	if (address[0] == '[')
	{
		const int baseIndex = addressLength - 1;
		for (size_t i = 0; i < 6; ++i)
		{
			const int index = baseIndex - i;
			if (index < 3)
			{
				break;
			}
			if (address[index] == ':')
			{
				myPort = uint16_t(atoi(&address[index + 1]));
				address[index - 1] = '\0';
			}
		}
		address += 1;
	}
	struct in6_addr sockaddr6;
	if (inet_pton(AF_INET6, address, &sockaddr6) == 1)
	{
		memcpy(myIpv6Address, &sockaddr6, 16);
		myType = AddressType::ipv6;
		return;
	}
	//IPV4
	addressLength = (int)strlen(address);
	const int baseIndex = addressLength - 1;
	for (size_t i = 0; i < 6; ++i)
	{
		const int index = baseIndex - i;
		if (index < 0)
		{
			break;
		}
		if (address[index] == ':')
		{
			myPort = (uint16_t)atoi(&address[index + 1]);
			address[index] = '\0';
		}
	}

	struct sockaddr_in sockaddr4;
	if (inet_pton(AF_INET, address, &sockaddr4.sin_addr) == 1)
	{
		myType = AddressType::ipv4;
		myIpv4Address = sockaddr4.sin_addr.S_un.S_addr;
	}
	else
	{
		Clear();
	}
}

uint32_t Spooder::Address::GetAddress4() const
{
	assert(myType == AddressType::ipv4);
	return myIpv4Address;
}

const uint16_t* Spooder::Address::GetAddress6() const
{
	assert(myType == AddressType::ipv6);
	return myIpv6Address;
}

void Spooder::Address::SetPort(uint16_t aPort)
{
	myPort = aPort;
}

uint16_t Spooder::Address::GetPort() const
{
	return myPort;
}

Spooder::AddressType Spooder::Address::GetType() const
{
	return myType;
}

const char* Spooder::Address::ToString(char aBuffer[], int aBufferSize) const
{
	if (myType == AddressType::ipv4)
	{
		const uint8_t a = myIpv4Address & 0xff;
		const uint8_t b = (myIpv4Address >> 8) & 0xff;
		const uint8_t c = (myIpv4Address >> 16) & 0xff;
		const uint8_t d = (myIpv4Address >> 24) & 0xff;
		if (myPort != 0)
		{
			snprintf(aBuffer, aBufferSize, "%d.%d.%d.%d:%d", a, b, c, d, myPort);
		}
		else
		{
			snprintf(aBuffer, aBufferSize, "%d.%d.%d.%d", a, b, c, d);
		}
		return aBuffer;
	}
}

bool Spooder::Address::operator==(const Address& aOther) const
{
	if (myType != aOther.myType)
	{
		return false;
	}
	if (myPort != aOther.myPort)
	{
		return false;
	}
	if (myType == AddressType::ipv4 && myIpv4Address == aOther.myIpv4Address)
	{
		return true;
	}
	else if (myType == AddressType::ipv6 && memcmp(myIpv6Address, aOther.myIpv6Address, sizeof(myIpv6Address)) == 0)
	{
		return true;
	}
	return false;
}

bool Spooder::Address::operator!=(const Address& aOther) const
{
	return !(*this == aOther);
}

bool Spooder::Address::IsValid() const
{
	return myType != AddressType::Undefined;
}

Spooder::Address::Address()
{
	Clear();
}

Spooder::Address::Address(uint8_t aA, uint8_t aB, uint8_t aC, uint8_t aD, uint16_t aPort)
{
	myType = AddressType::ipv4;
	myIpv4Address = uint32_t(aA) | uint32_t(aB)<<8 | uint32_t(aC)<<16 | uint32_t(aD)<<24;
	myPort = aPort;
}

Spooder::Address::Address(uint32_t aAddress, uint16_t aPort)
{
	myType = AddressType::ipv4;
	myIpv4Address = htonl(aAddress);
	myPort = aPort;
}

Spooder::Address::Address(uint16_t aA, uint16_t aB, uint16_t aC, uint16_t aD, uint16_t aE, uint16_t aF, uint16_t aG, uint16_t aH, uint16_t aPort)
{
	myType = AddressType::ipv6;
	myIpv6Address[0] = htons(aA);
	myIpv6Address[1] = htons(aB);
	myIpv6Address[2] = htons(aC);
	myIpv6Address[3] = htons(aD);
	myIpv6Address[4] = htons(aE);
	myIpv6Address[5] = htons(aF);
	myIpv6Address[6] = htons(aG);
	myIpv6Address[7] = htons(aH);
	myPort = aPort;
}

Spooder::Address::Address(uint16_t aAddress[], uint16_t aPort)
{
	myType = AddressType::ipv6;
	for (size_t i = 0; i < 8; i++)
	{
		myIpv6Address[i] = htons(aAddress[i]);
	}
	myPort = aPort;
}

Spooder::Address::Address(const char* aAddress)
{
	Parse(aAddress);
}

Spooder::Address::Address(const char* aAddress, uint16_t aPort)
{
	Parse(aAddress);
	myPort = aPort;
}

Spooder::Address::Address(const sockaddr_storage& aAddress)
{
	if(aAddress.ss_family == AF_INET)
	{
		const sockaddr_in& addrIPv4 = reinterpret_cast<const sockaddr_in&>(aAddress);
		myType = AddressType::ipv4;
		myIpv4Address = addrIPv4.sin_addr.S_un.S_addr;
		myPort = ntohs(addrIPv4.sin_port);
	}
	else if(aAddress.ss_family == AF_INET6)
	{
		const sockaddr_in6& addrIPv6 = reinterpret_cast<const sockaddr_in6&>(aAddress);
		myType = AddressType::ipv6;
		memcpy(myIpv6Address, &addrIPv6.sin6_addr, 16);
		myPort = ntohs(addrIPv6.sin6_port);
	}
	else
	{
		assert(false);
		Clear();
	}
}
