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
#include "UDPProxyClient.h"
#include "UDPProxyCoordinator.h"
#include "UDPProxyServer.h"

static const unsigned short COORDINATOR_PORT=60001;
static const char *COORDINATOR_PASSWORD="Dummy Password";
static const char *DEFAULT_COORDINATOR_IP="127.0.0.1";

void DefaultPacketMessage(Packet *p)
{
	if (p->data[0]==ID_DISCONNECTION_NOTIFICATION)
		printf("%s disconnected\n", p->systemAddress.ToString(true));
	else if (p->data[0]==ID_CONNECTION_LOST)
		printf("Lost connection to %s\n", p->systemAddress.ToString(true));
	else if (p->data[0]==ID_NO_FREE_INCOMING_CONNECTIONS)
		printf("%s has no free incoming connections.\n", p->systemAddress.ToString(true));
	else if (p->data[0]==ID_NEW_INCOMING_CONNECTION)
		printf("%s connected to us\n", p->systemAddress.ToString(true));
	else if (p->data[0]==ID_CONNECTION_REQUEST_ACCEPTED)
		printf("Connection request accepted from %s (success)\n", p->systemAddress.ToString(true));
	else if (p->data[0]==ID_CONNECTION_ATTEMPT_FAILED)
		printf("Failed to connect to %s\n", p->systemAddress.ToString(true));
}
void ProcessPackets(RakPeerInterface *rakPeer)
{
	Packet *p;
	for (p=rakPeer->Receive(); p; rakPeer->DeallocatePacket(p), p=rakPeer->Receive())
	{
		DefaultPacketMessage(p);
	}
}

struct UDPProxyClientResultHandler_Test : public RakNet::UDPProxyClientResultHandler
{
	virtual void OnForwardingSuccess(const char *proxyIPAddress, unsigned short proxyPort, unsigned short reverseProxyPort,
		SystemAddress proxyCoordinator, SystemAddress sourceAddress, SystemAddress targetAddress, RakNet::UDPProxyClient *proxyClientPlugin)
	{
		printf("Datagrams forwarded successfully by proxy %s:%i to target %s.\n", proxyIPAddress, proxyPort, targetAddress.ToString(false));
		printf("Connecting to proxy, which will be received by target.\n");
		rakPeer->Connect(proxyIPAddress, proxyPort, 0, 0);
	}
	virtual void OnForwardingNotification(const char *proxyIPAddress, unsigned short proxyPort, unsigned short reverseProxyPort,
		SystemAddress proxyCoordinator, SystemAddress sourceAddress, SystemAddress targetAddress, RakNet::UDPProxyClient *proxyClientPlugin)
	{
		printf("Source %s has setup forwarding to us through proxy %s:%i.\n", sourceAddress.ToString(false), proxyIPAddress, reverseProxyPort);
	}
	virtual void OnNoServersOnline(SystemAddress proxyCoordinator, SystemAddress sourceAddress, SystemAddress targetAddress, RakNet::UDPProxyClient *proxyClientPlugin)
	{
		printf("Failure: No servers logged into coordinator.\n");
	}
	virtual void OnAllServersBusy(SystemAddress proxyCoordinator, SystemAddress sourceAddress, SystemAddress targetAddress, RakNet::UDPProxyClient *proxyClientPlugin)
	{
		printf("Failure: No servers have available forwarding ports.\n");
	}
	virtual void OnForwardingInProgress(SystemAddress proxyCoordinator, SystemAddress sourceAddress, SystemAddress targetAddress, RakNet::UDPProxyClient *proxyClientPlugin)
	{
		printf("Notification: Forwarding already in progress.\n");
	}

	void RunClient(void)
	{
		rakPeer = RakNetworkFactory::GetRakPeerInterface();
		rakPeer->AttachPlugin(&proxyClient);
		rakPeer->Startup(32,0,&SocketDescriptor(0,0), 1);
		rakPeer->SetMaximumIncomingConnections(32);
		// Set this class to get the event callbacks
		proxyClient.SetResultHandler(this);

		char coordinatorIP[256];
		printf("Enter IP address of coordinator:\n");
		gets(coordinatorIP);
		if (coordinatorIP[0]==0)
			strcpy(coordinatorIP, DEFAULT_COORDINATOR_IP);
		printf("Connecting...\n");
		rakPeer->Connect(coordinatorIP,COORDINATOR_PORT,0,0);

		SystemAddress coordinatorAddress;
		coordinatorAddress.SetBinaryAddress(coordinatorIP);
		coordinatorAddress.port=COORDINATOR_PORT;


		while (1)
		{
			Packet *p;
			for (p=rakPeer->Receive(); p; rakPeer->DeallocatePacket(p), p=rakPeer->Receive())
			{
				if (p->data[0]==ID_CONNECTION_REQUEST_ACCEPTED)
				{
					if (p->systemAddress==coordinatorAddress)
					{
						printf("Connected to coordinator at %s.\n", p->systemAddress.ToString(true));
						printf("My external IP address is %s.\n", rakPeer->GetExternalID(p->systemAddress).ToString(true));
						printf("If another client is already connected, you can press 'f' to request forwarding\n");
						printf("Press 'c' to try connecting directly.\n");
					}
					else
					{
						printf("Connected to client at %s.\n", p->systemAddress.ToString(true));
						printf("Test successful\n");
					}
				}
				else if (p->data[0]==ID_CONNECTION_LOST)
				{
					if (p->systemAddress==coordinatorAddress)
					{
						printf("Lost connection to the coordinator at %s\n", p->systemAddress.ToString(true));
					}
					else
					{
						printf("Lost connection to another client %s\n", p->systemAddress.ToString(true));
					}
				}
				else
					DefaultPacketMessage(p);
			}

			if (kbhit())
			{
				char ch = getch();
				if (ch=='q' || ch=='Q')
					break;
				if (ch=='f' || ch=='F' ||
					ch=='c' || ch=='C')
				{
					char targetAddressStr[256];
					do 
					{
						printf("Enter the other client's external IP address in the format x.x.x.x:port\n");
						gets(targetAddressStr);
					} while (targetAddressStr[0]==0);
					SystemAddress targetAddr;
					targetAddr.SetBinaryAddress(targetAddressStr);

					if (ch=='f' || ch=='F')
					{
						// The first parameter is the address of the coordinator.
						// The second parameter is to forward our own address (but it could be any, including systems not running RakNet)
						// The third parameter is the address to forward to
						// The last address is to drop the forward if no messages between the two systems are sent for 7 secondss
						proxyClient.RequestForwarding(coordinatorAddress, UNASSIGNED_SYSTEM_ADDRESS, targetAddr, 7000);
					}
					else
					{
						rakPeer->Connect(targetAddressStr, targetAddr.port, 0, 0);
					}
					
				}
			}

			RakSleep(30);
		}	

		rakPeer->Shutdown(100,0);
		RakNetworkFactory::DestroyRakPeerInterface(rakPeer);
	}

	RakPeerInterface *rakPeer;
	RakNet::UDPProxyClient proxyClient;
};

struct UDPProxyServerResultHandler_Test : public RakNet::UDPProxyServerResultHandler
{
	virtual void OnLoginSuccess(RakNet::RakString usedPassword, RakNet::UDPProxyServer *proxyServer)
	{
		printf("Server login success.\nClients can requesting forwarding from the coordinator.\n");
	}

	virtual void OnAlreadyLoggedIn(RakNet::RakString usedPassword, RakNet::UDPProxyServer *proxyServer)
	{
		printf("Notification: Already logged in.\n");
	}

	virtual void OnNoPasswordSet(RakNet::RakString usedPassword, RakNet::UDPProxyServer *proxyServer)
	{
		printf("Failure: No password was set on the coordinator, so all logins rejected.\n");
	}

	virtual void OnWrongPassword(RakNet::RakString usedPassword, RakNet::UDPProxyServer *proxyServer)
	{
		printf("Failure: Bad login password.\n");
	}

	void RunServer(void)
	{
		rakPeer = RakNetworkFactory::GetRakPeerInterface();
		rakPeer->AttachPlugin(&proxyServer);
		rakPeer->Startup(1,0,&SocketDescriptor(0,0), 1);
		// Set this class to get the event callbacks
		proxyServer.SetResultHandler(this);
		char str[256];
		printf("Enter IP address of coordinator:\n");
		gets(str);
		if (str[0]==0)
			strcpy(str, DEFAULT_COORDINATOR_IP);
		printf("Connecting...\n");
		rakPeer->Connect(str,COORDINATOR_PORT,0,0);

		while (1)
		{	
			Packet *p;
			for (p=rakPeer->Receive(); p; rakPeer->DeallocatePacket(p), p=rakPeer->Receive())
			{
				if (p->data[0]==ID_CONNECTION_REQUEST_ACCEPTED)
				{
					printf("Connected to coordinator at %s. Sending login.\n", p->systemAddress.ToString(true));
					proxyServer.LoginToCoordinator(COORDINATOR_PASSWORD, p->systemAddress);
				}
				else
					DefaultPacketMessage(p);
			}

			if (kbhit())
			{
				char ch = getch();
				if (ch=='q' || ch=='Q')
					break;
			}

			RakSleep(30);
		}	

		rakPeer->Shutdown(100,0);
		RakNetworkFactory::DestroyRakPeerInterface(rakPeer);
	}

	RakPeerInterface *rakPeer;
	RakNet::UDPProxyServer proxyServer;
};

void RunCoordinator(void)
{
	RakPeerInterface *rakPeer = RakNetworkFactory::GetRakPeerInterface();
	RakNet::UDPProxyCoordinator coordinator;
	rakPeer->AttachPlugin(&coordinator);
	rakPeer->Startup(64,0,&SocketDescriptor(COORDINATOR_PORT,0), 1);
	rakPeer->SetMaximumIncomingConnections(64);
	printf("Coordinator started.\nYou can now start the server(s), and connect to %s.\n", rakPeer->GetInternalID(UNASSIGNED_SYSTEM_ADDRESS).ToString(true));

	// Coordinator requires that you set a password for servers to login
	coordinator.SetRemoteLoginPassword(COORDINATOR_PASSWORD);

	while (1)
	{
		ProcessPackets(rakPeer);

		if (kbhit())
		{
			char ch = getch();
			if (ch=='q' || ch=='Q')
				break;
		}

		RakSleep(30);
	}	

	rakPeer->Shutdown(100,0);
	RakNetworkFactory::DestroyRakPeerInterface(rakPeer);
}

void main(void)
{	
	printf("UDPProxy is a system to tell a server to forward datagrams between two systems.\n");
	printf("These datagrams can be from any two clients, even clients not running RakNet.\n");
	printf("To test, run 1 coordinator, then at least 1 server, lastly 2 clients\n");
	printf("\n");
	printf("1. UDPProxyCoordinator: Tracks UDPProxyServer(s). Manages client requests\n");
	printf("2. UDPProxyServer: Forwards datagrams. Logs into UDPProxyCoordinator.\n");
	printf("3. UDPProxyClient: Makes requests of UDPProxyCoordinator.\n");
	
	char str[256];
	do
	{
		printf("Enter number of system to start: ");
		gets(str);
	} while (str[0]!='1' && str[0]!='2' && str[0]!='3');
	
	printf("Press 'q' to quit.\n");
	if (str[0]=='1')
	{
		RunCoordinator();
	}
	else if (str[0]=='2')
	{
		UDPProxyServerResultHandler_Test t;
		t.RunServer();
	}
	else
	{
		UDPProxyClientResultHandler_Test t;
		t.RunClient();
	}
}
