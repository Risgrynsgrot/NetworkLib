#pragma once
#include <cstdint>
#include "CommonNetworkIncludes.h"

namespace Spooder
{

	enum class AddressType
	{
		Undefined,
		ipv4,
		ipv6
	};
	class Address
	{
	public:
		Address();
		explicit Address(uint8_t aA, uint8_t aB, uint8_t aC, uint8_t aD, uint16_t aPort = 0);
		explicit Address(uint32_t aAddress, uint16_t aPort = 0);
		explicit Address(uint16_t aA, uint16_t aB, uint16_t aC, uint16_t aD,
			uint16_t aE, uint16_t aF, uint16_t aG, uint16_t aH, uint16_t aPort = 0);
		explicit Address(uint16_t aAddress[], uint16_t aPort = 0);
		explicit Address(const char* aAddress);
		explicit Address(const char* aAddress, uint16_t aPort);
		explicit Address(const sockaddr_storage& aAddress);

		void Clear();

		void Parse(const char* aAddress);

		uint32_t GetAddress4() const;
		const uint16_t* GetAddress6() const;
		void SetPort(uint16_t aPort);
		uint16_t GetPort() const;
		AddressType GetType() const;

		const char* ToString(char aBuffer[], int aBufferSize) const;

		bool operator==(const Address& aOther) const;
		bool operator!=(const Address& aOther) const;

		bool IsValid() const;
	private:
		AddressType myType;
		union
		{
			uint32_t myIpv4Address;
			uint16_t myIpv6Address[8];
		};
		uint16_t myPort;
	};
}
