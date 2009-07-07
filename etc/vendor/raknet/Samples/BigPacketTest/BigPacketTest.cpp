#include "RakPeerInterface.h"
#include "RakNetworkFactory.h"
#include "BitStream.h"
#include <stdlib.h> // For atoi
#include <cstring> // For strlen
#include "RakNetStatistics.h"
#include "GetTime.h"
#include "MessageIdentifiers.h"
#include "MTUSize.h"
#include <stdio.h>

#ifdef _COMPATIBILITY_1
#include "Compatibility1Includes.h" // Developers of a certain platform will know what to do here.
#else
#include "RakSleep.h"
#endif

bool quit;
bool sentPacket=false;

#define BIG_PACKET_SIZE 60739000

RakPeerInterface *client, *server;
char *text;

int main(void)
{
	client=server=0;

	text= new char [BIG_PACKET_SIZE];
	quit=false;
	RakNetTime nextStatTime = RakNet::GetTime() + 1000;
	char ch;

	printf("This is a test I use to test the packet splitting capabilities of RakNet\n");
	printf("All it does is send a large block of data to the feedback loop\n");
	printf("Difficulty: Beginner\n\n");

	printf("Enter 's' to run as server, 'c' to run as client, space to run local.\n");
	gets(text);
	ch=text[0];
	if (ch=='c')
	{
		client=RakNetworkFactory::GetRakPeerInterface();
		printf("Working as client\n");
		printf("Enter remote IP: ");
		gets(text);
		if (text[0]==0)
			strcpy(text, "216.224.123.180");
	}
	else if (ch=='s')
	{
		server=RakNetworkFactory::GetRakPeerInterface();
		printf("Working as server\n");
	}
	else
	{
		client=RakNetworkFactory::GetRakPeerInterface();
		server=RakNetworkFactory::GetRakPeerInterface();;
		strcpy(text, "127.0.0.1");
	}
	if (client)
	{
		SocketDescriptor socketDescriptor(0,0);
		client->Startup(1, 0, &socketDescriptor, 1);
		client->SetSplitMessageProgressInterval(100); // Get ID_DOWNLOAD_PROGRESS notifications
		client->Connect(text, 60000, 0, 0);
	}
	if (server)
	{
		SocketDescriptor socketDescriptor(60000,0);
		server->SetMaximumIncomingConnections(32);
		server->Startup(32, 0, &socketDescriptor, 1);
	}
	RakSleep(500);

	// Always apply the network simulator on two systems, never just one, with half the values on each.
	// Otherwise the flow control gets confused.
	//if (client)
	// client->ApplyNetworkSimulator(128000, 0, 0);
	//if (server)
	//	server->ApplyNetworkSimulator(128000, 0, 0);

	RakNetTime start,stop;

	start=RakNet::GetTime();
	Packet *packet;
	while (!quit)
	{
		if (server)
		{
			for (packet = server->Receive(); packet; server->DeallocatePacket(packet), packet=server->Receive())
			{
				if (packet->data[0]==ID_NEW_INCOMING_CONNECTION)
				{
					for (int i=0; i < BIG_PACKET_SIZE; i++)
						text[i]=i%256;
					text[0]=(char)255; // So it doesn't register as an internal ID
					server->Send(text, BIG_PACKET_SIZE, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
					// Keep the stat from updating until the messages move to the thread or it quits right away
					nextStatTime=RakNet::GetTime()+1000;
				}
				if (packet->data[0]==ID_CONNECTION_LOST)
					printf("ID_CONNECTION_LOST from %s\n", packet->systemAddress.ToString());
				else if (packet->data[0]==ID_DISCONNECTION_NOTIFICATION)
					printf("ID_DISCONNECTION_NOTIFICATION from %s\n", packet->systemAddress.ToString());
				else if (packet->data[0]==ID_NEW_INCOMING_CONNECTION)
					printf("ID_NEW_INCOMING_CONNECTION from %s\n", packet->systemAddress.ToString());
				else if (packet->data[0]==ID_CONNECTION_REQUEST_ACCEPTED)
					printf("ID_CONNECTION_REQUEST_ACCEPTED from %s\n", packet->systemAddress.ToString());
			}
		}
		if (client)
		{
			packet = client->Receive();
			while (packet)
			{
				if (packet->data[0]==ID_DOWNLOAD_PROGRESS)
				{
					RakNet::BitStream progressBS(packet->data, packet->length, false);
					progressBS.IgnoreBits(8); // ID_DOWNLOAD_PROGRESS
					unsigned int progress;
					unsigned int total;
					unsigned int partLength;
					RakNetStatistics *rss=client->GetStatistics(client->GetSystemAddressFromIndex(0));

					// Disable endian swapping on reading this, as it's generated locally in ReliabilityLayer.cpp
					progressBS.ReadBits( (unsigned char* ) &progress, BYTES_TO_BITS(sizeof(progress)), true );
					progressBS.ReadBits( (unsigned char* ) &total, BYTES_TO_BITS(sizeof(total)), true );
					progressBS.ReadBits( (unsigned char* ) &partLength, BYTES_TO_BITS(sizeof(partLength)), true );

					printf("Progress: msgID=%i Progress %i/%i Partsize=%i Full=%i\n",
						(unsigned char) packet->data[0],
						progress,
						total,
						partLength,
						rss->bandwidthExceeded);
				}
				else if (packet->data[0]>=ID_USER_PACKET_ENUM)
				{
					if (packet->data[0]==255)
					{
						bool dataValid=true;
						for (int i=1; i < BIG_PACKET_SIZE; i++)
						{
							if (packet->data[i]!=i%256)
							{
								dataValid=false;
								break;
							}
						}

						if (dataValid)
							printf("Test succeeded. %i bytes.\n", packet->length);
						else
							printf("Test failed. %i bytes.\n", packet->length);
					}
					else
						printf("Unknown packet %i: Test failed. %i bytes.\n", packet->data[0], packet->length);

					quit=true;
				}
				else if (packet->data[0]==ID_CONNECTION_LOST)
					printf("ID_CONNECTION_LOST from %s\n", packet->systemAddress.ToString());
				else if (packet->data[0]==ID_DISCONNECTION_NOTIFICATION)
					printf("ID_DISCONNECTION_NOTIFICATION from %s\n", packet->systemAddress.ToString());
				else if (packet->data[0]==ID_NEW_INCOMING_CONNECTION)
					printf("ID_NEW_INCOMING_CONNECTION from %s\n", packet->systemAddress.ToString());
				else if (packet->data[0]==ID_CONNECTION_REQUEST_ACCEPTED)
					printf("ID_CONNECTION_REQUEST_ACCEPTED from %s\n", packet->systemAddress.ToString());

				client->DeallocatePacket(packet);
				packet = client->Receive();
			}
		}

		if (RakNet::GetTime() > nextStatTime)
		{
			nextStatTime=RakNet::GetTime()+1000;
			RakNetStatistics *rssSender;
			RakNetStatistics *rssReceiver;
			if (server)
			{
				unsigned int i;
				unsigned short numSystems;
				server->GetConnectionList(0,&numSystems);
				if (numSystems>0)
				{
					printf("KPBS,ploss: ");
					for (i=0; i < numSystems; i++)
					{
						rssSender=server->GetStatistics(server->GetSystemAddressFromIndex(i));
						printf("%i:%.0f,%.1f ", i+1,rssSender->bitsPerSecondSent/1000, 100.0f * ( float ) rssSender->messagesTotalBitsResent / ( float ) rssSender->totalBitsSent);
					}
					printf("\n");
				}

				/*
				if (rssSender)
				{
				printf("Snd: %i waiting. %i waiting on ack. Got %i acks. KBPS=%.1f. Ploss=%.1f. Full=%i\n", rssSender->messageSendBuffer[HIGH_PRIORITY], rssSender->messagesOnResendQueue,rssSender->acknowlegementsReceived, rssSender->bitsPerSecond/1000, 100.0f * ( float ) rssSender->messagesTotalBitsResent / ( float ) rssSender->totalBitsSent, rssSender->bandwidthExceeded);
				if (client==0)
				printf("\n");
				if (sentPacket && rssSender->messageSendBuffer[HIGH_PRIORITY]==0 && rssSender->messagesOnResendQueue==0 && client==0)
				{
				RakNetStatistics *rss=server->GetStatistics(server->GetSystemAddressFromIndex(0));
				StatisticsToString(rss, text, 2);
				printf("%s", text);
				printf("Sender quitting with no messages on resend queue.\n");
				quit=true;
				}
				}
				*/
			}
			if (client)
			{
				rssReceiver=client->GetStatistics(client->GetSystemAddressFromIndex(0));
				if (rssReceiver)
					printf("Receiver: %i acks waiting.\n", rssReceiver->acknowlegementsPending);
			}
		}

		RakSleep(100);
	}
	stop=RakNet::GetTime();
	double seconds = (double)(stop-start)/1000.0;

	if (server)
	{
		RakNetStatistics *rssSender=server->GetStatistics(server->GetSystemAddressFromIndex(0));
		StatisticsToString(rssSender, text, 2);
		printf("%s", text);
	}

	printf("%i bytes per second transfered. Press enter to quit\n", (int)((double)(BIG_PACKET_SIZE) / seconds )) ;
	gets(text);

	delete []text;
	RakNetworkFactory::DestroyRakPeerInterface(client);
	RakNetworkFactory::DestroyRakPeerInterface(server);

	return 0;
}
