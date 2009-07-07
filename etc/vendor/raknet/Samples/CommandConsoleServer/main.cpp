/// \file
/// \brief Test the command console implementations
///
/// This file is part of RakNet Copyright 2003 Jenkins Software LLC
///
/// Usage of RakNet is subject to the appropriate license agreement.


#include "TCPInterface.h"
#include "ConsoleServer.h"
#include "RakNetCommandParser.h"
#include "TelnetTransport.h"
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "LogCommandParser.h"
#include "GetTime.h"
#include "RakNetTransport2.h"
#include "LinuxStrings.h"
#include <stdio.h>

#ifdef _COMPATIBILITY_1
#include "Compatibility1Includes.h"
#define printf(x,y) 
#elif defined(_WIN32)
#include "WindowsIncludes.h" // Sleep
#else
#include <unistd.h> // usleep
#include <stdio.h>
#endif


void TestTCPInterface(void);
void TestCommandServer(TransportInterface *ti, unsigned short port, RakPeerInterface *rakPeer);

int main(void)
{
	// TelnetTransport tt;
	// TestCommandServer(&tt, 23); // Uncomment to use Telnet as a client.  Telnet uses port 23 by default.

	RakNetTransport2 rt2;
	RakPeerInterface *rakPeer = RakNetworkFactory::GetRakPeerInterface();
	rakPeer->Startup(128,30,&SocketDescriptor(60000,0),1);
	rakPeer->SetMaximumIncomingConnections(128);
	rakPeer->AttachPlugin(&rt2);
	TestCommandServer(&rt2, 60000,rakPeer); // Uncomment to use RakNet as a client
	return 1;
}

void TestCommandServer(TransportInterface *ti, unsigned short port, RakPeerInterface *rakPeer)
{
    ConsoleServer consoleServer;
	RakNetCommandParser rcp;
	LogCommandParser lcp;
	RakNetTime lastLog=0;

	printf("This sample demonstrates the command console server, which can be.\n");
	printf("a standalone application or part of your game server.  It allows you to\n");
	printf("easily parse text strings sent from a client using either secure RakNet\n");
	printf("or Telnet.\n");
	printf("See the 'CommandConsoleClient' project for the RakNet client.\n");
	printf("Difficulty: Intermediate\n\n");

	printf("Command server started on port %i.\n", port);
	consoleServer.AddCommandParser(&rcp);
	consoleServer.AddCommandParser(&lcp);
	consoleServer.SetTransportProvider(ti, port);
	consoleServer.SetPrompt("> "); // Show this character when waiting for user input
	rcp.SetRakPeerInterface(rakPeer);
	lcp.AddChannel("TestChannel");
	while (1)
	{
		consoleServer.Update();
		// Ignore raknet packets for this sample.
		rakPeer->DeallocatePacket(rakPeer->Receive());

		if (RakNet::GetTime() > lastLog + 4000)
		{
			lcp.WriteLog("TestChannel", "Test of logger");
			lastLog=RakNet::GetTime();
		}

#ifdef _WIN32
		Sleep( 30 );
#else
		usleep( 30 * 1000 );
#endif
	}	
}