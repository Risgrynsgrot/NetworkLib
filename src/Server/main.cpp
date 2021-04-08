#include <cstdint>
#include <stdio.h>


#include "Server.h"
#include "Spooder/Address.h"
#include "Spooder/MessageFactory.h"
#include "Spooder/NetworkFunctions.h"
#include "Spooder/Socket.h"
#include "Spooder/Timer.h"

int main()
{
	Spooder::InitializeNetwork();
	Spooder::Timer* timer = Spooder::Timer::GetInstance();
	
	printf("---Server tester---\n");

	uint16_t port = 6970;
	//Spooder::Address address("127.0.0.1", port);
	Spooder::Socket socket(port);

	Spooder::ClientServerPacketFactory packetFactory;
	
	Spooder::Server server(socket, packetFactory);

	double time = 0;
	while (true)
	{
		time += timer->GetDeltaTime();
		server.SendPackets(time);
		server.ReceivePackets(time);
		server.CheckForTimeOut(time);
		timer->Update();
	}

	return 0;
}
