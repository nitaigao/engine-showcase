/// \file
/// \brief Tests the NAT-punchthrough plugin
///
/// This file is part of RakNet Copyright 2003 Jenkins Software LLC
///
/// Usage of RakNet is subject to the appropriate license agreement.


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
#include "ConnectionGraph2.h"
#include "PacketLogger.h"
#include "StringCompressor.h"
#include "PacketFileLogger.h"
#include "GetTime.h"
#include "NatPunchthroughClient.h"
#include "NatPunchthroughServer.h"
#include "SocketLayer.h"
#include "RakString.h"
#include "UDPProxyServer.h"
#include "UDPProxyCoordinator.h"
#include "UDPProxyClient.h"

// This file is in Samples/upnp
#include "UPNPNAT.h"

static const unsigned short NAT_PUNCHTHROUGH_FACILITATOR_PORT=60481;
static const char *NAT_PUNCHTHROUGH_FACILITATOR_PASSWORD="";
static const char *DEFAULT_NAT_PUNCHTHROUGH_FACILITATOR_IP="216.224.123.180";
static const char *COORDINATOR_PASSWORD="Dummy Coordinator Password";

// UPNP can directly tell the router to open a port (if it works)
void OpenWithUPNP(NatPunchthroughClient *npc)
{
	printf("Opening UPNP...\n");
	UPNPNAT nat;
	nat.init();
	if(!nat.discovery()){
		printf("discovery error is %s\n",nat.get_last_error());
		return;
	}
	// I'm not sure what use the first parameter is
	// The second parameter is the internal IP address for RakNet
	// The third parameter is the port other systems will connect to. The way NATPunchthrough is implemented
	// The fourth parameter is the internal port that RakNet is bound on
	if(!nat.add_port_mapping("Description",npc->GetUPNPInternalAddress().C_String(), npc->GetUPNPExternalPort(), npc->GetUPNPInternalPort(),"UDP")){
		printf("add_port_mapping error is %s\n",nat.get_last_error());
		return;
	}
};


struct NatPunchthroughDebugInterface_PacketFileLogger : public NatPunchthroughDebugInterface
{
	// Set to non-zero to write to the packetlogger!
	PacketFileLogger *pl;

	NatPunchthroughDebugInterface_PacketFileLogger() {pl=0;}
	~NatPunchthroughDebugInterface_PacketFileLogger() {}
	virtual void OnClientMessage(const char *msg)
	{
		if (pl)
		{
			pl->WriteMiscellaneous("Nat", msg);
		}
	}
	
};

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

	RakPeerInterface *rakPeer;
};

struct SystemStatus
{
	RakNetGUID guid;
	SystemAddress systemAddress;
	unsigned char packetId;
	int status; // -1 = failed. 0 = connecting. 1 = punched. 2 = connected.
};
DataStructures::List<SystemStatus> systems;

bool PushSystemStatus(RakNetGUID remoteGuid, SystemAddress remoteAddress)
{
	DataStructures::DefaultIndexType idx;
	for (idx=0; idx < systems.Size(); idx++)
		if (systems[idx].guid==remoteGuid)
			return false;
	SystemStatus ss;
	ss.guid=remoteGuid;
	ss.status=0;
	ss.systemAddress=remoteAddress;
	systems.Push(ss, __FILE__, __LINE__);
	return true;
}
void PrintStatus(const DataStructures::List<SystemStatus> &systems)
{
	system("cls");
	DataStructures::DefaultIndexType i;
	printf("Failed\t\tWorking\t\tPunched\t\tConnected\n");
	for (i=0; i < systems.Size(); i++)
	{
		unsigned int tabCount = (systems[i].status+1)*2;
		for (unsigned int j=0; j < tabCount; j++)
			printf("\t");
		printf("%s", systems[i].systemAddress.ToString(true));
		if (systems[i].status==-1)
			printf(" (Code %i)", systems[i].packetId);
		printf("\n");
	}
}
void GUIClientTest()
{
	SystemAddress serverSystemAddress(DEFAULT_NAT_PUNCHTHROUGH_FACILITATOR_IP,NAT_PUNCHTHROUGH_FACILITATOR_PORT);
	RakPeerInterface *rakPeer=RakNetworkFactory::GetRakPeerInterface();
	NatPunchthroughClient natPunchthroughClient;
	ConnectionGraph2 connectionGraph;
	rakPeer->AttachPlugin(&connectionGraph);
	rakPeer->AttachPlugin(&natPunchthroughClient);
	rakPeer->Startup(1024,0,&SocketDescriptor(0,0), 1);
	rakPeer->SetMaximumIncomingConnections(32);
	rakPeer->Connect(DEFAULT_NAT_PUNCHTHROUGH_FACILITATOR_IP, NAT_PUNCHTHROUGH_FACILITATOR_PORT, NAT_PUNCHTHROUGH_FACILITATOR_PASSWORD, (int) strlen(NAT_PUNCHTHROUGH_FACILITATOR_PASSWORD));

	DataStructures::List<RakNetSmartPtr<RakNetSocket> > sockets;
	rakPeer->GetSockets(sockets);
	printf("Ports used by RakNet:\n");
	for (unsigned int i=0; i < sockets.Size(); i++)
	{
		printf("%i. %i\n", i+1, sockets[i]->boundAddress.port);
	}

	printf("My IP is %s\n", rakPeer->GetLocalIP(0));

	Packet *p;
	while (1)
	{
		p=rakPeer->Receive();
		while (p)
		{
			if (p->data[0]==ID_REMOTE_NEW_INCOMING_CONNECTION)
			{
				unsigned int count;
				RakNet::BitStream bsIn(p->data, p->length, false);
				bsIn.IgnoreBytes(sizeof(MessageID));
				bsIn.Read(count);
				SystemAddress remoteAddress;
				RakNetGUID remoteGuid;
				for (unsigned int i=0; i < count; i++)
				{
					bsIn.Read(remoteAddress);
					bsIn.Read(remoteGuid);

					if (remoteAddress!=serverSystemAddress)
					{
						if (PushSystemStatus(remoteGuid, remoteAddress))
							natPunchthroughClient.OpenNAT(remoteGuid, serverSystemAddress);
					}					
				}
			}
			if (p->data[0]==ID_DISCONNECTION_NOTIFICATION || p->data[0]==ID_CONNECTION_LOST)
			{
				DataStructures::DefaultIndexType i;
				for (i=0; i < systems.Size(); i++)
					if (systems[i].guid==p->guid)
						systems.RemoveAtIndex(i);
			}
			else if (p->data[0]==ID_NO_FREE_INCOMING_CONNECTIONS)
			{
			}
			else if (p->data[0]==ID_CONNECTION_GRAPH_REPLY)
			{
			}
			else if (p->data[0]==ID_REMOTE_DISCONNECTION_NOTIFICATION ||
				p->data[0]==ID_REMOTE_CONNECTION_LOST)
			{
				// Ignore this if not from the server
				if (p->systemAddress==serverSystemAddress)
				{
					RakNet::BitStream bsIn(p->data, p->length, false);
					SystemAddress remoteAddress;
					RakNetGUID remoteGuid;
					bsIn.IgnoreBytes(sizeof(MessageID));
					bsIn.Read(remoteAddress);
					bsIn.Read(remoteGuid);

					DataStructures::DefaultIndexType i;
					for (i=0; i < systems.Size(); i++)
						if (systems[i].systemAddress==remoteAddress)
							systems.RemoveAtIndex(i);
				}
			}
			else if (p->data[0]==ID_NEW_INCOMING_CONNECTION ||
				p->data[0]==ID_CONNECTION_REQUEST_ACCEPTED)
			{
				if (strcmp(p->systemAddress.ToString(false),DEFAULT_NAT_PUNCHTHROUGH_FACILITATOR_IP)!=0)
				{
					DataStructures::DefaultIndexType i;
					bool gotAny=false;
					for (i=0; i < systems.Size(); i++)
					{
						if (systems[i].guid==p->guid)
						{
							gotAny=true;
							systems[i].status=2;
							break;
						}
					}
					RakAssert(gotAny==true);
				}
				else
				{
					// Use UPNP to open the port connections will come in on, if possible
					OpenWithUPNP(&natPunchthroughClient);
				}
			}
			else if (
				p->data[0]==ID_NAT_TARGET_NOT_CONNECTED ||
				p->data[0]==ID_NAT_TARGET_UNRESPONSIVE ||
				p->data[0]==ID_NAT_CONNECTION_TO_TARGET_LOST ||
//				p->data[0]==ID_NAT_ALREADY_IN_PROGRESS ||
				p->data[0]==ID_NAT_PUNCHTHROUGH_FAILED
				)
			{
				RakNetGUID guid;
				if (p->data[0]==ID_NAT_PUNCHTHROUGH_FAILED)
					guid=p->guid;
				else
				{
					RakNet::BitStream bs(p->data,p->length,false);
					bs.IgnoreBytes(1);
					bool b = bs.Read(guid);
					RakAssert(b);
				}

				DataStructures::DefaultIndexType i;
				for (i=0; i < systems.Size(); i++)
				{
					if (systems[i].guid==guid)
					{
						systems[i].packetId=p->data[0];
						systems[i].status=-1;
						break;
					}
				}
			}
			else if (p->data[0]==ID_NAT_PUNCHTHROUGH_SUCCEEDED)
			{
				DataStructures::DefaultIndexType i;
				for (i=0; i < systems.Size(); i++)
				{
					if (systems[i].guid==p->guid && systems[i].status==0)
					{
						// Might get legitimately due to high lag for one of the systems
					//	RakAssert(systems[i].status==0);
						systems[i].status=1;
						break;
					}
				}

				if (p->data[1]==1)
					rakPeer->Connect(p->systemAddress.ToString(), p->systemAddress.port, 0, 0);
			}
			else if (p->data[0]==ID_PONG)
			{
			}
			else if (p->data[0]==ID_INVALID_PASSWORD)
			{
			}
			else if (p->data[0]==ID_NAT_ALREADY_IN_PROGRESS)
			{
			}

			rakPeer->DeallocatePacket(p);
			p=rakPeer->Receive();
		}

		if (kbhit())
		{
			char ch = getch();
			if (ch=='q' || ch=='Q')
				break;
		}

		PrintStatus(systems);
		RakSleep(0);
	}

	rakPeer->Shutdown(100,0);
	RakNetworkFactory::DestroyRakPeerInterface(rakPeer);

}

void VerboseTest()
{
	RakPeerInterface *rakPeer=RakNetworkFactory::GetRakPeerInterface();

	// Base systems, try to connect through the router
	// NAT punchthrough plugin for the client
	NatPunchthroughClient natPunchthroughClient;
	// Prints messages to the screen, for debugging the client
	NatPunchthroughDebugInterface_Printf debugInterface;
	// NAT punchthrough plugin for the server
	NatPunchthroughServer natPunchthroughServer;

	// Fallback systems - if NAT punchthrough fails, we route messages through a server
	// Maintains a list of running UDPProxyServer
	RakNet::UDPProxyCoordinator udpProxyCoordinator;
	// Routes messages between two systems that cannot connect through NAT punchthrough
	RakNet::UDPProxyServer udpProxyServer;
	// Connects to UDPProxyCoordinator
	RakNet::UDPProxyClient udpProxyClient;
	// Handles progress notifications for udpProxyClient
	UDPProxyClientResultHandler_Test udpProxyClientResultHandler;
	udpProxyClientResultHandler.rakPeer=rakPeer;
	// Coordinator requires logon to use, to prevent sending to non-servers
	udpProxyCoordinator.SetRemoteLoginPassword(COORDINATOR_PASSWORD);

	// Helps with the sample, to connect systems that join
	ConnectionGraph2 connectionGraph;
	natPunchthroughClient.SetDebugInterface(&debugInterface);
	rakPeer->AttachPlugin(&connectionGraph);

	char mode[64], facilitatorIP[64];
	printf("Tests the NAT Punchthrough plugin\n");
	printf("Difficulty: Intermediate\n\n");

	int i;
	DataStructures::List<RakNet::RakString> internalIds;
	char ipList[ MAXIMUM_NUMBER_OF_INTERNAL_IDS ][ 16 ];
	unsigned int binaryAddresses[MAXIMUM_NUMBER_OF_INTERNAL_IDS];
	SocketLayer::Instance()->GetMyIP( ipList, binaryAddresses );

	for (i=0; i < MAXIMUM_NUMBER_OF_INTERNAL_IDS; i++)
	{
		RakNet::RakString rs;
		if (ipList[i][0])
		{
			rs = ipList[i];

			if (internalIds.GetIndexOf(rs)==(unsigned int)-1)
			{	
				internalIds.Push(rs);
			}
		}
	}

	SocketDescriptor socketDescriptor;
	char str[128];
	if (internalIds.Size()>1)
	{
		printf("Which IP to bind to (Enter = any)?\n");
		for (i=0; i < (int) internalIds.Size(); i++)
		{
			printf("%i. %s\n", i+1, internalIds[i].C_String());
		}
		gets(str);
		if (str[0])
		{
			int index = atoi(str);
			strcpy(socketDescriptor.hostAddress, internalIds[index-1].C_String());
		}
	}
	if (socketDescriptor.hostAddress[0]==0)
		printf("My internal IP is %s.\n", internalIds[0].C_String());

	printf("Act as (S)ender, (R)ecipient, or (F)acilitator?\n");
	gets(mode);
	SystemAddress facilitatorSystemAddress=UNASSIGNED_SYSTEM_ADDRESS;
	if (mode[0]=='s' || mode[0]=='S' || mode[0]=='r' || mode[0]=='R')
	{
		PunchthroughConfiguration *pc = natPunchthroughClient.GetPunchthroughConfiguration();
		printf("Modify TIME_BETWEEN_PUNCH_ATTEMPTS_INTERNAL?\n");
		gets(str);
		if (str[0]!=0)
			pc->TIME_BETWEEN_PUNCH_ATTEMPTS_INTERNAL=atoi(str);

		printf("Modify TIME_BETWEEN_PUNCH_ATTEMPTS_EXTERNAL?\n");
		gets(str);
		if (str[0]!=0)
			pc->TIME_BETWEEN_PUNCH_ATTEMPTS_EXTERNAL=atoi(str);

		printf("Modify UDP_SENDS_PER_PORT?\n");
		gets(str);
		if (str[0]!=0)
			pc->UDP_SENDS_PER_PORT_EXTERNAL=atoi(str);

		printf("Modify INTERNAL_IP_WAIT_AFTER_ATTEMPTS?\n");
		gets(str);
		if (str[0]!=0)
			pc->INTERNAL_IP_WAIT_AFTER_ATTEMPTS=atoi(str);

		printf("Modify MAX_PREDICTIVE_PORT_RANGE?\n");
		gets(str);
		if (str[0]!=0)
			pc->MAX_PREDICTIVE_PORT_RANGE=atoi(str);

		printf("Modify EXTERNAL_IP_WAIT_BETWEEN_PORTS?\n");
		gets(str);
		if (str[0]!=0)
			pc->EXTERNAL_IP_WAIT_BETWEEN_PORTS=atoi(str);

		printf("Modify EXTERNAL_IP_WAIT_AFTER_ALL_ATTEMPTS?\n");
		gets(str);
		if (str[0]!=0)
			pc->EXTERNAL_IP_WAIT_AFTER_ALL_ATTEMPTS=atoi(str);

		rakPeer->Startup(1024,0,&socketDescriptor, 1);
		rakPeer->SetMaximumIncomingConnections(32);
		printf("Enter facilitator IP: ");
		gets(facilitatorIP);
		if (facilitatorIP[0]==0)
			strcpy(facilitatorIP, DEFAULT_NAT_PUNCHTHROUGH_FACILITATOR_IP);

		rakPeer->AttachPlugin(&natPunchthroughClient);
		rakPeer->AttachPlugin(&udpProxyClient);
		// Set the C++ class to get the result of our proxy operation
		udpProxyClient.SetResultHandler(&udpProxyClientResultHandler);

		printf("Connecting to facilitator.\n");
		rakPeer->Connect(facilitatorIP, NAT_PUNCHTHROUGH_FACILITATOR_PORT, NAT_PUNCHTHROUGH_FACILITATOR_PASSWORD, (int) strlen(NAT_PUNCHTHROUGH_FACILITATOR_PASSWORD));

		facilitatorSystemAddress.SetBinaryAddress(facilitatorIP);
		facilitatorSystemAddress.port=NAT_PUNCHTHROUGH_FACILITATOR_PORT;
	}
	else
	{
		socketDescriptor.port=NAT_PUNCHTHROUGH_FACILITATOR_PORT;
		rakPeer->Startup(1024,0,&socketDescriptor, 1);
		rakPeer->SetMaximumIncomingConnections(32);
		rakPeer->AttachPlugin(&natPunchthroughServer);
		rakPeer->AttachPlugin(&udpProxyServer);
		rakPeer->AttachPlugin(&udpProxyCoordinator);

		// Login proxy server to proxy coordinator
		// Normally the proxy server is on a different computer. Here, we login to our own IP address since the plugin is on the same system
		udpProxyServer.LoginToCoordinator(COORDINATOR_PASSWORD, rakPeer->GetInternalID(UNASSIGNED_SYSTEM_ADDRESS));

		printf("Ready.\n");
	}

	
	printf("My GUID is %s\n", rakPeer->GetGuidFromSystemAddress(UNASSIGNED_SYSTEM_ADDRESS).ToString());

	printf("'c'lear the screen.\n'q'uit.\n");
	Packet *p;
	while (1)
	{
		p=rakPeer->Receive();
		while (p)
		{
			if (p->data[0]==ID_REMOTE_NEW_INCOMING_CONNECTION && (mode[0]=='s' || mode[0]=='S'))
			{
				RakNet::BitStream bsIn(p->data, p->length, false);
				SystemAddress remoteAddress;
				RakNetGUID remoteGuid;
				bsIn.IgnoreBytes(sizeof(MessageID));
				unsigned int addressesCount;
				bsIn.Read(addressesCount);
				bsIn.Read(remoteAddress);
				bsIn.Read(remoteGuid);

				printf("Connecting to remote system.\n");
				natPunchthroughClient.OpenNAT(remoteGuid, rakPeer->GetSystemAddressFromIndex(0));
			}
			if (p->data[0]==ID_DISCONNECTION_NOTIFICATION)
				printf("ID_DISCONNECTION_NOTIFICATION\n");
			else if (p->data[0]==ID_CONNECTION_LOST)
			{
				printf("ID_CONNECTION_LOST from %s\n", p->systemAddress.ToString());
			}
			else if (p->data[0]==ID_NO_FREE_INCOMING_CONNECTIONS)
				printf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
			else if (p->data[0]==ID_NEW_INCOMING_CONNECTION)
			{
					printf("ID_NEW_INCOMING_CONNECTION from %s with guid %s\n", p->systemAddress.ToString(), p->guid.ToString());
			}
			else if (p->data[0]==ID_REMOTE_NEW_INCOMING_CONNECTION)
			{
				printf("Got ID_REMOTE_NEW_INCOMING_CONNECTION from %s\n", p->systemAddress.ToString(true));
			}
			else if (p->data[0]==ID_CONNECTION_GRAPH_REPLY)
			{
				printf("Got ID_CONNECTION_GRAPH_REPLY from %s\n", p->systemAddress.ToString(true));
			}
			else if (p->data[0]==ID_REMOTE_DISCONNECTION_NOTIFICATION)
			{
				printf("Got ID_REMOTE_DISCONNECTION_NOTIFICATION from %s\n", p->systemAddress.ToString(true));
			}
			else if (p->data[0]==ID_REMOTE_CONNECTION_LOST)
			{
				printf("Got ID_REMOTE_CONNECTION_LOST from %s\n", p->systemAddress.ToString(true));
			}
			else if (p->data[0]==ID_CONNECTION_REQUEST_ACCEPTED)
			{
				// Use UPNP to open the port connections will come in on, if possible
				if (p->systemAddress==facilitatorSystemAddress)
				{
					printf("Connected to facilitator with GUID %s.\n", p->guid.ToString());
					OpenWithUPNP(&natPunchthroughClient);
				}
				if (mode[0]=='s' || mode[0]=='S')
				{
					printf("ID_CONNECTION_REQUEST_ACCEPTED from %s.\nMy external IP is %s\n", p->systemAddress.ToString(), rakPeer->GetExternalID(p->systemAddress).ToString());
				}
				else
					printf("ID_CONNECTION_REQUEST_ACCEPTED from %s.\nMy external IP is %s\n", p->systemAddress.ToString(), rakPeer->GetExternalID(p->systemAddress). ToString());
			}
			else if (p->data[0]==ID_CONNECTION_ATTEMPT_FAILED)
			{
				if (p->systemAddress.port==NAT_PUNCHTHROUGH_FACILITATOR_PORT)
					printf("ID_CONNECTION_ATTEMPT_FAILED (facilitator)\n");
				else
					printf("ID_CONNECTION_ATTEMPT_FAILED (recipient)\n");
			}
			else if (p->data[0]==ID_NAT_TARGET_NOT_CONNECTED)
			{
				RakNetGUID g;
				RakNet::BitStream b(p->data, p->length, false);
				b.IgnoreBits(8); // Ignore the ID_...
				b.Read(g);
				printf("ID_NAT_TARGET_NOT_CONNECTED for %s\n", g.ToString());
			}
			else if (p->data[0]==ID_NAT_TARGET_UNRESPONSIVE)
			{
				RakNetGUID g;
				RakNet::BitStream b(p->data, p->length, false);
				b.IgnoreBits(8); // Ignore the ID_...
				b.Read(g);
				printf("ID_NAT_TARGET_UNRESPONSIVE for %s\n", g.ToString());
			}
			else if (p->data[0]==ID_NAT_CONNECTION_TO_TARGET_LOST)
			{
				RakNetGUID g;
				RakNet::BitStream b(p->data, p->length, false);
				b.IgnoreBits(8); // Ignore the ID_...
				b.Read(g);
				printf("ID_NAT_CONNECTION_TO_TARGET_LOST for %s\n", g.ToString());
			}
			else if (p->data[0]==ID_NAT_ALREADY_IN_PROGRESS)
			{
				RakNetGUID g;
				RakNet::BitStream b(p->data, p->length, false);
				b.IgnoreBits(8); // Ignore the ID_...
				b.Read(g);
				printf("ID_NAT_ALREADY_IN_PROGRESS for %s\n", g.ToString());
			}
			else if (p->data[0]==ID_NAT_PUNCHTHROUGH_FAILED)
			{
				printf("ID_NAT_PUNCHTHROUGH_FAILED for %s\n", p->guid.ToString());

				// Fallback to use the proxy
				// Only one system needs to do this, so just have the sender do it
				if (mode[0]=='s' || mode[0]=='S')
				{
					printf("Attemping proxy connection.\n");
					udpProxyClient.RequestForwarding(facilitatorSystemAddress, UNASSIGNED_SYSTEM_ADDRESS, p->systemAddress, 7000);
				}
				else
					printf("Waiting for proxy connection.\n");
			}
			else if (p->data[0]==ID_NAT_PUNCHTHROUGH_SUCCEEDED)
			{
				printf("ID_NAT_PUNCHTHROUGH_SUCCEEDED for %s\n", p->systemAddress.ToString());
				if (mode[0]=='s' || mode[0]=='S')
				{
					rakPeer->Connect(p->systemAddress.ToString(), p->systemAddress.port, 0, 0);
				}
			}
			else if (p->data[0]==ID_PONG)
			{
				RakNetTime time;
				memcpy((char*)&time, p->data+1, sizeof(RakNetTime));
				printf("Got pong from %s with time %i\n", p->systemAddress.ToString(), RakNet::GetTime() - time);
			}
			else if (p->data[0]==ID_INVALID_PASSWORD)
			{
				printf("ID_INVALID_PASSWORD\n");
			}
			else if (p->data[0]==ID_NAT_ALREADY_IN_PROGRESS)
			{
				SystemAddress systemAddress;
				RakNet::BitStream b(p->data, p->length, false);
				b.IgnoreBits(8); // Ignore the ID_...
				b.Read(systemAddress);
				printf("ID_NAT_ALREADY_IN_PROGRESS to %s\n", systemAddress.ToString());
			}
			else
			{
				printf("Unknown packet ID %i from %s\n", p->data[0], p->systemAddress.ToString(true));
			}

			rakPeer->DeallocatePacket(p);
			p=rakPeer->Receive();
		}

		if (kbhit())
		{
			char ch = getch();
			if (ch=='q' || ch=='Q')
				break;
			if (ch=='c' || ch=='C')
			{
#ifdef _WIN32
				system("cls");
#else
				printf("Unsupported on this OS\n");
#endif
			}
		}

		RakSleep(30);
	}

	rakPeer->Shutdown(100,0);
	RakNetworkFactory::DestroyRakPeerInterface(rakPeer);
}



// This sample starts one of three systems: A facilitator, a recipient, and a sender.  The sender wants to connect to the recipient but
// cannot because both the sender and recipient are behind NATs.  So they both connect to the facilitator first.
// The facilitator will synchronize a countdown timer so that the sender will try to connect to the recipient at the same time
// the recipient will send an offline message to the sender.  This will, in theory, punch through the NAT.
//
// IMPORTANT: The recipient and sender need to know each other's external IP addresses to be able to try to connect to each other.
// The facilitator should transmit this on connection, such as with a lobby server, which I do here using the ConnectionGraph plugin.
// That plugin will cause the lobby server to send ID_REMOTE_* system notifications to its connected systems.
int main(void)
{
//#ifdef _DEBUG
//	VerboseTest();
//#else
	GUIClientTest();
//#endif
	return 1;
}