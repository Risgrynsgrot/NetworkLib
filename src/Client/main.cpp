#include <stdio.h>
#include "Client.h"
#include "Spooder/MessageFactory.h"
#include "Spooder/NetworkFunctions.h"
#include "Spooder/Socket.h"
#include "Spooder/Timer.h"

int main()
{
	Spooder::Timer* timer = Spooder::Timer::GetInstance();
	Spooder::InitializeNetwork();
	printf("---Client tester---\n");
	uint16_t port = 60000;
	Spooder::Address address("::1", port);
	Spooder::Socket socket(port);

	Spooder::ClientServerPacketFactory packetFactory;
	
	Spooder::Client client(socket, packetFactory);

	printf("Enter IP:");
	char addressString[256];
	scanf("%s", addressString);

	double time = 0;
	const Spooder::Address serverAddress(addressString);

	timer->Update();
	time += timer->GetDeltaTimeDouble();
	client.Connect(serverAddress, time);

	while (true)
	{
		timer->Update();
		time += timer->GetDeltaTimeDouble();
		client.SendPackets(time);
		client.ReceivePackets(time);
		client.CheckForTimeOut(time);
	}

	return 0;
}
