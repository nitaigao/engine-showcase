#ifndef WIN32
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#include <wspiapi.h>
#endif
#include <iostream>
#include <udt.h>
#include <assert.h>
#include "cc.h"
#include "SocketLayer.h"

using namespace std;

#ifndef WIN32
void* monitor(void*);
#else
DWORD WINAPI monitor(LPVOID);
#endif

struct addrinfo hints, *local, *peer;

// Note to self: SOCK_DGRAM is MUCH slower than SOCK_STREAM
static const int UDPMESSAGESIZE=100000000;

int main(int argc, char* argv[])
{
	if ((4 != argc) || (0 == atoi(argv[2])))
	{
		cout << "usage: appclient server_ip server_port localPort" << endl;
		return 0;
	}

	// use this function to initialize the UDT library
	UDT::startup();


	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
//	hints.ai_socktype = SOCK_STREAM;
	  hints.ai_socktype = SOCK_DGRAM;

	if (UDT::ERROR == getaddrinfo(NULL, "9000", &hints, &local))
	{
		cout << "incorrect network address.\n" << endl;
		return 0;
	}

	UDTSOCKET client = UDT::socket(local->ai_family, local->ai_socktype, local->ai_protocol);
	int *sockOptInt = new int;
#if defined(_XBOX) || defined(X360)
	*sockOptInt=1264;
#elif defined(WIN32)
	*sockOptInt=1052;
#endif

	SOCKET rakNetSocket = SocketLayer::Instance()->CreateBoundSocket(atoi(argv[3]),true,0);
	if (UDT::setsockopt(client, 0, UDT_MSS, sockOptInt, sizeof(int))<0)
		delete sockOptInt;

	if (UDT::ERROR == UDT::bind(client, rakNetSocket))
	{
		cout << "bind: " << UDT::getlasterror().getErrorMessage() << endl;
		return 0;
	}


	// UDT Options
	//UDT::setsockopt(client, 0, UDT_CC, new CCCFactory<CUDPBlast>, sizeof(CCCFactory<CUDPBlast>));
	//UDT::setsockopt(client, 0, UDT_MSS, new int(9000), sizeof(int));
	//UDT::setsockopt(client, 0, UDT_SNDBUF, new int(10000000), sizeof(int));
	//UDT::setsockopt(client, 0, UDP_SNDBUF, new int(10000000), sizeof(int));


	/*
	if (hints.ai_socktype == SOCK_DGRAM)
	{
		UDT::setsockopt(client, 0, UDT_SNDBUF, new int(UDPMESSAGESIZE), sizeof(int));
		UDT::setsockopt(client, 0, UDT_RCVBUF, new int(UDPMESSAGESIZE), sizeof(int));
	}
	*/

	bool useRendezvous=false;

	// for rendezvous connection, enable the code below
	if (useRendezvous)
	{
		UDT::setsockopt(client, 0, UDT_RENDEZVOUS, new bool(true), sizeof(bool));
	}

	freeaddrinfo(local);

	if (UDT::ERROR == getaddrinfo(argv[1], argv[2], &hints, &peer))
	{
		cout << "incorrect server/peer address. " << argv[1] << ":" << argv[2] << endl;
		return 0;
	}

	// connect to the server, implict bind
	if (UDT::ERROR == UDT::connect(client, peer->ai_addr, peer->ai_addrlen))
	{
		cout << "connect: " << UDT::getlasterror().getErrorMessage() << endl;
		return 0;
	}

	freeaddrinfo(peer);

	// using CC method
	//CUDPBlast* cchandle = NULL;
	//int temp;
	//UDT::getsockopt(client, 0, UDT_CC, &cchandle, &temp);
	//if (NULL != cchandle)
	//   cchandle->setRate(500);

	int size;
	if ( hints.ai_socktype == SOCK_DGRAM )
		size=UDPMESSAGESIZE;
	else
		size=100000;
	char* data = new char[size];


#ifndef WIN32
	pthread_create(new pthread_t, NULL, monitor, &client);
#else
	CreateThread(NULL, 0, monitor, &client, 0, NULL);
#endif

	if (hints.ai_socktype==SOCK_DGRAM)
	{
		int ssize = 0;
		int ss;

		while (ssize+1052 < UDPMESSAGESIZE)
		{
			/*
			if (UDT::ERROR == (ss = UDT::sendmsg(client, data + ssize, UDT::MAX_SEND_MSG_PAYLOAD_WITHOUT_FRAGMENTATION, -1, false)))
			{
				cout << "send:" << UDT::getlasterror().getErrorMessage() << endl;
			}
			assert(ss==UDT::MAX_SEND_MSG_PAYLOAD_WITHOUT_FRAGMENTATION);
			ssize+=UDT::MAX_SEND_MSG_PAYLOAD_WITHOUT_FRAGMENTATION;
			*/
			if (UDT::ERROR == (ss = UDT::sendmsg(client, data + ssize, 1008, -1, false)))
			{
				cout << "send:" << UDT::getlasterror().getErrorMessage() << endl;
			}
			assert(ss==1008);
			ssize+=1008;
		}

		Sleep(30000);
	}
	else
	{
		for (int i = 0; i < 1000000; i ++)
		{
			int ssize = 0;
			int ss;
			while (ssize < size)
			{
				if (UDT::ERROR == (ss = UDT::send(client, data + ssize, size - ssize, 0)))
				{
					cout << "send:" << UDT::getlasterror().getErrorMessage() << endl;
					break;
				}

				ssize += ss;
			}

			if (ssize < size)
				break;
		}
	}

	UDT::close(client);

	delete [] data;

	// use this function to release the UDT library
	UDT::cleanup();

	return 1;
}

#ifndef WIN32
void* monitor(void* s)
#else
DWORD WINAPI monitor(LPVOID s)
#endif
{
	UDTSOCKET u = *(UDTSOCKET*)s;

	UDT::TRACEINFO perf;

	cout << "SendRate(Mb/s)\tRTT(ms)\tCWnd\tPktSndPeriod(us)\tRecvACK\tRecvNAK" << endl;

	while (true)
	{
#ifndef WIN32
		sleep(1);
#else
		Sleep(1000);
#endif

		if (UDT::ERROR == UDT::perfmon(u, &perf))
		{
			cout << "perfmon: " << UDT::getlasterror().getErrorMessage() << endl;
			break;
		}

		cout << perf.mbpsSendRate << "\t\t" 
			<< perf.msRTT << "\t" 
			<< perf.pktCongestionWindow << "\t" 
			<< perf.usPktSndPeriod << "\t\t\t" 
			<< perf.pktRecvACK << "\t" 
			<< perf.pktRecvNAK << endl;

		
	}

#ifndef WIN32
	return NULL;
#else
	return 0;
#endif
}
