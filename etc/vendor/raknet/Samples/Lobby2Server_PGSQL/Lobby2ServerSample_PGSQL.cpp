#include "Lobby2Message.h"
#include "RakPeerInterface.h"
#include "RakNetworkFactory.h"
#include "MessageIdentifiers.h"
#include "Kbhit.h"
#include "RakSleep.h"
#include "Lobby2Server_PGSQL.h"
#include "Lobby2Message_PGSQL.h"
#include <ctype.h>
#include <stdlib.h>

void main(void)
{
	printf("The 2nd interation of the lobby server.\n");
	printf("Difficulty: Intermediate\n\n");

	char serverPort[30];
	RakPeerInterface *rakPeer=RakNetworkFactory::GetRakPeerInterface();
//	puts("Enter the rakPeer port to listen on");
	serverPort[0]=0;
	//gets(serverPort);
	if (serverPort[0]==0)
		strcpy(serverPort, "10000");

	SocketDescriptor socketDescriptor(atoi(serverPort),0);
	rakPeer->SetMaximumIncomingConnections(32);
	if (rakPeer->Startup(32,30,&socketDescriptor, 1)==false)
	{
		printf("Startup call failed\n");
		return;
	}
	// Attach the plugin Lobby2Server
	// The class factory will create messages with server functionality
	RakNet::Lobby2Server_PGSQL lobby2Server;
	rakPeer->AttachPlugin(&lobby2Server);
	RakNet::Lobby2MessageFactory_PGSQL messageFactory;
	lobby2Server.SetMessageFactory(&messageFactory);

//	printf("Enter database password:\n");
	char connectionString[256],password[128];
	char username[256];
	strcpy(username, "postgres");
	password[0]=0;
//	gets(password);
	if (password[0]==0) strcpy(password, "aaaa");
	strcpy(connectionString, "user=");
	strcat(connectionString, username);
	strcat(connectionString, " password=");
	strcat(connectionString, password);

	if (lobby2Server.ConnectToDB(connectionString, 4)==false)
	{
		printf("Database connection failed\n");
		return;
	}

	printf("Lobby2Server started and waiting for connections\n");


	RakNet::Lobby2Server::ConfigurationProperties c;
	c.requiresEmailAddressValidationToLogin=true;
	c.requiresTitleToLogin=true;
	c.accountRegistrationRequiresCDKey=true;
	c.accountRegistrationRequiredAgeYears=13;
	lobby2Server.SetConfigurationProperties(c);

	Packet *packet;
	// Loop for input
	while (1)
	{
		for (packet=rakPeer->Receive(); packet; rakPeer->DeallocatePacket(packet), packet=rakPeer->Receive())
		{
			switch (packet->data[0])
			{
			case ID_DISCONNECTION_NOTIFICATION:
				// Connection lost normally
				printf("ID_DISCONNECTION_NOTIFICATION\n");
				break;
			case ID_NEW_INCOMING_CONNECTION:
				// Connection lost normally
				printf("ID_NEW_INCOMING_CONNECTION\n");
				printf("Allowing all operations from remote client for testing (insecure)\n");
				printf("Use Lobby2Server::ExecuteCommand for local server operations\n");
				// For this test, allow all operations
				lobby2Server.AddAdminAddress(packet->systemAddress);
				lobby2Server.AddRankingAddress(packet->systemAddress);
				break;
			case ID_MODIFIED_PACKET:
				// Cheater!
				printf("ID_MODIFIED_PACKET\n");
				break;
			case ID_CONNECTION_LOST:
				// Couldn't deliver a reliable packet - i.e. the other system was abnormally
				// terminated
				printf("ID_CONNECTION_LOST\n");
				break;
			}
		}

		// This sleep keeps RakNet responsive
		RakSleep(30);

		if (kbhit())
		{
		}
	}

	RakNetworkFactory::DestroyRakPeerInterface(rakPeer);
}
