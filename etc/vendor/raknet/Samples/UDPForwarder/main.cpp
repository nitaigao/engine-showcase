#include "RakPeerInterface.h"
#include "RakSleep.h"
#include <stdio.h>
#include <stdlib.h>
#include "RakNetworkFactory.h"
#include <string.h>
#include "Kbhit.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakSleep.h"
#include "UDPForwarder.h"

void main(void)
{
	RakPeerInterface *rakPeer[2];
	rakPeer[0]=RakNetworkFactory::GetRakPeerInterface();
	rakPeer[1]=RakNetworkFactory::GetRakPeerInterface();
	rakPeer[0]->Startup(1,0,&SocketDescriptor(50000,0), 1);
	rakPeer[1]->Startup(1,0,&SocketDescriptor(50002,0), 1);
	rakPeer[1]->SetMaximumIncomingConnections(1);
	RakNet::UDPForwarder udpForwarder;
	
	printf("Demonstrates the UDP Forwarder class\n");
	printf("It routes datagrams from system to another, at the UDP level.\n");
	printf("You probably won't use UDPForwarder directly.\n");
	printf("See UDPProxyClient, UDPProxyServer, UDPProxyCoordinator.\n");
	
	// Start the forwarder
	udpForwarder.Startup();

	// RakNet will send a message at least every 5 seconds. Add another second to account for thread latency
	const RakNetTime timeoutOnNoDataMS=6000;

	// Address is probably 192.168.0.1. Fix it to be 127.0.0.1.
	// Only necessary to do this when connecting through the loopback on the local system. In a real system we'd stick with the external IP
	SystemAddress peer0Addr = rakPeer[0]->GetInternalID(UNASSIGNED_SYSTEM_ADDRESS);
	SystemAddress peer1Addr = rakPeer[1]->GetInternalID(UNASSIGNED_SYSTEM_ADDRESS);
	peer0Addr.SetBinaryAddress("127.0.0.1");
	peer1Addr.SetBinaryAddress("127.0.0.1");

	unsigned short peer0To1FowardPort;
	unsigned short peer1To0FowardPort;
	if (!udpForwarder.StartForwarding(peer0Addr,peer1Addr, timeoutOnNoDataMS, 0, &peer0To1FowardPort, &peer1To0FowardPort))
	{
		printf("Socket error\n");
		return;
	}

	// Send a connect message to the forwarder, on the port to forward to rakPeer[1]
	rakPeer[0]->Connect("127.0.0.1", peer0To1FowardPort, 0, 0);
	
	printf("'q'uit.\n");
	Packet *p;
	while (1)
	{
		for (int i=0; i < 2 ; i++)
		{
			p=rakPeer[i]->Receive();
			while (p)
			{
				if (p->data[0]==ID_DISCONNECTION_NOTIFICATION)
					printf("%s disconnected\n", p->systemAddress.ToString(true));
				else if (p->data[0]==ID_CONNECTION_LOST)
					printf("Lost connection to %s (failure)\n", p->systemAddress.ToString(true));
				else if (p->data[0]==ID_NO_FREE_INCOMING_CONNECTIONS)
					printf("%s has no free incoming connections.\n", p->systemAddress.ToString(true));
				else if (p->data[0]==ID_NEW_INCOMING_CONNECTION)
					printf("%s connected to us (success)\n", p->systemAddress.ToString(true));
				else if (p->data[0]==ID_CONNECTION_REQUEST_ACCEPTED)
					printf("Connection request accepted from %s (success)\n", p->systemAddress.ToString(true));
				else if (p->data[0]==ID_CONNECTION_ATTEMPT_FAILED)
					printf("Failed to connect to %s (failure)\n", p->systemAddress.ToString(true));
	
				rakPeer[i]->DeallocatePacket(p);
				p=rakPeer[i]->Receive();
			}
		}

		udpForwarder.Update();

		if (kbhit())
		{
			char ch = getch();
			if (ch=='q' || ch=='Q')
				break;
		}

		RakSleep(30);
	}

	rakPeer[0]->Shutdown(100,0);
	rakPeer[1]->Shutdown(100,0);
	RakNetworkFactory::DestroyRakPeerInterface(rakPeer[0]);
	RakNetworkFactory::DestroyRakPeerInterface(rakPeer[1]);
}
