// Common includes
#include <stdio.h>
#include <stdlib.h>
#include "Kbhit.h"
#include "RakNetworkFactory.h"
#include "GetTime.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "StringCompressor.h"
#include "FileListTransfer.h"
#include "FileList.h" // FLP_Printf
#include "PacketizedTCP.h"

// Server only includes
#include "AutopatcherServer.h"
// Replace this repository with your own implementation if you don't want to use PostgreSQL
#include "AutopatcherPostgreRepository.h"

#ifdef _WIN32
#include "WindowsIncludes.h" // Sleep
#else
#include <unistd.h> // usleep
#endif

#define USE_TCP
#define LISTEN_PORT 60000
#define MAX_INCOMING_CONNECTIONS 8

void main(int argc, char **argv)
{
	printf("Server starting... ");
	AutopatcherServer autopatcherServer;
	FLP_Printf progressIndicator;
	FileListTransfer fileListTransfer;
	AutopatcherPostgreRepository postgre;
	autopatcherServer.SetAutopatcherRepositoryInterface(&postgre);
	fileListTransfer.SetCallback(&progressIndicator);
	autopatcherServer.SetFileListTransferPlugin(&fileListTransfer);
#ifdef USE_TCP
	PacketizedTCP packetizedTCP;
	if (packetizedTCP.Start(LISTEN_PORT,MAX_INCOMING_CONNECTIONS)==false)
	{
		printf("Failed to start TCP. Is the port already in use?");
		return;
	}
	packetizedTCP.AttachPlugin(&autopatcherServer);
	packetizedTCP.AttachPlugin(&fileListTransfer);
#else
	RakPeerInterface *rakPeer;
	rakPeer = RakNetworkFactory::GetRakPeerInterface();
	SocketDescriptor socketDescriptor(LISTEN_PORT,0);
	rakPeer->Startup(8,0,&socketDescriptor, 1);
	rakPeer->SetMaximumIncomingConnections(MAX_INCOMING_CONNECTIONS);
	rakPeer->AttachPlugin(&autopatcherServer);
	rakPeer->AttachPlugin(&fileListTransfer);
#endif
	printf("started.\n");

	printf("Enter database password:\n");
	char connectionString[256],password[128];
	char username[256];
	strcpy(username, "postgres");
	gets(password);
	if (password[0]==0) strcpy(password, "aaaa");
	strcpy(connectionString, "user=");
	strcat(connectionString, username);
	strcat(connectionString, " password=");
	strcat(connectionString, password);
	if (postgre.Connect(connectionString)==false)
	{
		printf("Database connection failed.\n");
		return;
	}
	printf("Database connection suceeded.\n");
	printf("(D)rop database\n(C)reate database.\n(A)dd application\n(U)pdate revision.\n(R)emove application\n(Q)uit\n");

	char ch;
	Packet *p;
	while (1)
	{
#ifdef USE_TCP
		SystemAddress notificationAddress;
		notificationAddress=packetizedTCP.HasCompletedConnectionAttempt();
		if (notificationAddress!=UNASSIGNED_SYSTEM_ADDRESS)
			printf("ID_CONNECTION_REQUEST_ACCEPTED\n");
		notificationAddress=packetizedTCP.HasNewIncomingConnection();
		if (notificationAddress!=UNASSIGNED_SYSTEM_ADDRESS)
			printf("ID_NEW_INCOMING_CONNECTION\n");
		notificationAddress=packetizedTCP.HasLostConnection();
		if (notificationAddress!=UNASSIGNED_SYSTEM_ADDRESS)
			printf("ID_CONNECTION_LOST\n");

		p=packetizedTCP.Receive();
		while (p)
		{
			packetizedTCP.DeallocatePacket(p);
			p=packetizedTCP.Receive();
		}
#else
		p=rakPeer->Receive();
		while (p)
		{
			if (p->data[0]==ID_NEW_INCOMING_CONNECTION)
				printf("ID_NEW_INCOMING_CONNECTION\n");
			else if (p->data[0]==ID_DISCONNECTION_NOTIFICATION)
				printf("ID_DISCONNECTION_NOTIFICATION\n");
			else if (p->data[0]==ID_CONNECTION_LOST)
				printf("ID_CONNECTION_LOST\n");

			rakPeer->DeallocatePacket(p);
			p=rakPeer->Receive();
		}
#endif

		if (kbhit())
		{
			ch=getch();
			if (ch=='q')
				break;
			else if (ch=='c')
			{
				if (postgre.CreateAutopatcherTables()==false)
					printf("%s", postgre.GetLastError());
			}
			else if (ch=='d')
			{
                if (postgre.DestroyAutopatcherTables()==false)
					printf("%s", postgre.GetLastError());
			}
			else if (ch=='a')
			{
				printf("Enter application name to add: ");
				char appName[512];
				gets(appName);
				if (appName[0]==0)
					strcpy(appName, "TestApp");

				if (postgre.AddApplication(appName, username)==false)
					printf("%s", postgre.GetLastError());
				else
					printf("Done\n");
			}
			else if (ch=='r')
			{
				printf("Enter application name to remove: ");
				char appName[512];
				gets(appName);
				if (appName[0]==0)
					strcpy(appName, "TestApp");

				if (postgre.RemoveApplication(appName)==false)
					printf("%s", postgre.GetLastError());
				else
					printf("Done\n");
			}
			else if (ch=='u')
			{
				printf("Enter application name: ");
				char appName[512];
				gets(appName);
				if (appName[0]==0)
					strcpy(appName, "TestApp");

				printf("Enter application directory: ");
				char appDir[512];
				gets(appDir);
				if (appDir[0]==0)
					strcpy(appDir, "C:/temp");

				if (postgre.UpdateApplicationFiles(appName, appDir, username, &progressIndicator)==false)
				{
					printf("%s", postgre.GetLastError());
				}
				else
				{
					printf("Update success.\n");
				}
			}
		}

		Sleep(30);
	}


#ifdef USE_TCP
	packetizedTCP.Stop();
#else
	RakNetworkFactory::DestroyRakPeerInterface(rakPeer);
#endif
}
