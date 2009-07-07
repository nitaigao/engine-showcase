/// \file
/// \brief Tests connecting two peers at the same time with the internet simulator running.
///
/// This file is part of RakNet Copyright 2003 Jenkins Software LLC
///
/// Usage of RakNet is subject to the appropriate license agreement.


#include "RakPeerInterface.h"
#include "RakNetworkFactory.h"
#include "PacketLogger.h"
#include "Rand.h"
#include "Kbhit.h"
#include <stdio.h> // Printf
#include "RakSleep.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "GetTime.h"

void main(void)
{
	printf("An internal test to test two peers connecting to each other\n");
	printf("at the same time.  This causes bugs so I fix them here\n");
	printf("Difficulty: Beginner\n\n");
	printf("(S)erver or (c)lient?: ");
	static const unsigned short SERVER_PORT=1234;
	char serverMode[32];
	bool isServer;
	char serverIP[64];
	gets(serverMode);
	if (serverMode[0]=='s' || serverMode[0]=='S')
	{
		isServer=true;
	}
	else
	{
		isServer=false;

		printf("Enter server IP: ");
		gets(serverIP);
		if (serverIP[0]==0)
			strcpy(serverIP,"127.0.0.1");	
	}
	char clientIP[64];
	RakPeerInterface *rakPeer;
	unsigned short clientPort;
	bool gotNotification;
	rakPeer=RakNetworkFactory::GetRakPeerInterface();
	if (isServer)
	{
		SocketDescriptor socketDescriptor(SERVER_PORT,0);
		rakPeer->Startup(1,0,&socketDescriptor, 1);
		rakPeer->SetMaximumIncomingConnections(1);
	}
	else
	{
		SocketDescriptor socketDescriptor(0,0);
		rakPeer->Startup(1,0,&socketDescriptor, 1);
	}

	printf("RakPeer started\n");
	if (isServer==false)
		rakPeer->Ping(serverIP,SERVER_PORT,false);

	//	PacketLogger pl;
	//	pl.LogHeader();
	//	rakPeer->AttachPlugin(&pl);

	RakNetTime connectionAttemptTime=0,connectionResultDeterminationTime=0,nextTestStartTime=0;
	while(1)
	{
		Packet *p;
		for (p=rakPeer->Receive(); p; rakPeer->DeallocatePacket(p), p=rakPeer->Receive())
		{
			if (p->data[0]==ID_NEW_INCOMING_CONNECTION)
			{
				printf("ID_NEW_INCOMING_CONNECTION\n");
				gotNotification=true;
			}
			else if (p->data[0]==ID_CONNECTION_REQUEST_ACCEPTED)
			{
				printf("ID_CONNECTION_REQUEST_ACCEPTED\n");
				gotNotification=true;
			}
			else if (p->data[0]==ID_PING)
			{
				printf("ID_PING\n");
				connectionAttemptTime=RakNet::GetTime()+1000;
				p->systemAddress.ToString(false,clientIP);
				clientPort=p->systemAddress.port;
				gotNotification=false;
			}
			else if (p->data[0]==ID_PONG)
			{
				printf("ID_PONG\n");
				RakNetTime sendPingTime;
				RakNet::BitStream bs(p->data,p->length,false);
				bs.IgnoreBytes(1);
				bs.Read(sendPingTime);
				RakNetTime rtt = RakNet::GetTime() - sendPingTime;
				if (rtt/2<=500)
					connectionAttemptTime=RakNet::GetTime()+1000-rtt/2;
				else
					connectionAttemptTime=RakNet::GetTime();
				gotNotification=false;
			}
		}
		if (connectionAttemptTime!=0 && RakNet::GetTime()>=connectionAttemptTime)
		{
			printf("Attemping connection\n");
			connectionAttemptTime=0;
			if (isServer)
				rakPeer->Connect(clientIP,clientPort,0,0);
			else
				rakPeer->Connect(serverIP,SERVER_PORT,0,0);
			connectionResultDeterminationTime=RakNet::GetTime()+2000;
		}
		if (connectionResultDeterminationTime!=0 && RakNet::GetTime()>=connectionResultDeterminationTime)
		{
			connectionResultDeterminationTime=0;
			if (gotNotification==false)
			{
				printf("Test failed.\n");
			}
			else
			{
				printf("Test succeeded.\n");
			}
			if (isServer==false)
			{
				SystemAddress sa;
				sa.SetBinaryAddress(serverIP);
				sa.port=SERVER_PORT;
				rakPeer->CancelConnectionAttempt(sa);
			}
			else
			{
				SystemAddress sa;
				sa.SetBinaryAddress(clientIP);
				sa.port=clientPort;
				rakPeer->CancelConnectionAttempt(sa);
			}
			rakPeer->CloseConnection(rakPeer->GetSystemAddressFromIndex(0),true,0);
			if (isServer==false)
				nextTestStartTime=RakNet::GetTime()+1000;

		}
		if (nextTestStartTime!=0 && RakNet::GetTime()>=nextTestStartTime)
		{
			rakPeer->Ping(serverIP,SERVER_PORT,false);
			nextTestStartTime=0;
		}
		RakSleep(0);

	}

	RakNetworkFactory::DestroyRakPeerInterface(rakPeer);
}
