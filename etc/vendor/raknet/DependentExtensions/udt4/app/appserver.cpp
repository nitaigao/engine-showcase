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
#include "cc.h"
#include "SocketLayer.h"

using namespace std;

#ifndef WIN32
void* recvdata(void*);
#else
DWORD WINAPI recvdata(LPVOID);
#endif

static const int UDPMESSAGESIZE=100000000;

addrinfo hints;
SOCKET rakNetSocket;

int main(int argc, char* argv[])
{
	if ((1 != argc) && ((2 != argc) || (0 == atoi(argv[1]))))
	{
		cout << "usage: appserver [server_port]" << endl;
		return 0;
	}

	// use this function to initialize the UDT library
	UDT::startup();

	addrinfo* res;

	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	//hints.ai_socktype = SOCK_STREAM;
	hints.ai_socktype = SOCK_DGRAM;

	string service("9000");
	if (2 == argc)
		service = argv[1];

	if (UDT::ERROR == getaddrinfo(NULL, service.c_str(), &hints, &res))
	{
		cout << "illegal port number or port is busy.\n" << endl;
		return 0;
	}

	UDTSOCKET serv = UDT::socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	int *sockOptInt = new int;
#if defined(_XBOX) || defined(X360)
	*sockOptInt=1264;
#elif defined(WIN32)
	*sockOptInt=1052;
#endif

	rakNetSocket = SocketLayer::Instance()->CreateBoundSocket(atoi(service.c_str()),true,0);
	if (UDT::setsockopt(serv, 0, UDT_MSS, sockOptInt, sizeof(int))<0)
		delete sockOptInt;

	if (1)
	{
		if (UDT::ERROR == UDT::bind(serv, rakNetSocket))
		{
			cout << "bind: " << UDT::getlasterror().getErrorMessage() << endl;
			return 0;
		}
	}	
	else
	{

		if (UDT::setsockopt(serv, 0, UDT_MSS, sockOptInt, sizeof(int))<0)
			delete sockOptInt;

		if (UDT::ERROR == UDT::bind(serv, res->ai_addr, res->ai_addrlen))
		{
			cout << "bind: " << UDT::getlasterror().getErrorMessage() << endl;
			return 0;
		}

	}


	// UDT Options
	//UDT::setsockopt(serv, 0, UDT_CC, new CCCFactory<CUDPBlast>, sizeof(CCCFactory<CUDPBlast>));
	//UDT::setsockopt(serv, 0, UDT_MSS, new int(9000), sizeof(int));
	//UDT::setsockopt(serv, 0, UDT_RCVBUF, new int(10000000), sizeof(int));
	//UDT::setsockopt(serv, 0, UDP_RCVBUF, new int(10000000), sizeof(int));


	if (hints.ai_socktype == SOCK_DGRAM)
	{
		//		UDT::setsockopt(serv, 0, UDT_SNDBUF, new int(UDPMESSAGESIZE), sizeof(int));
		//		UDT::setsockopt(serv, 0, UDT_RCVBUF, new int(UDPMESSAGESIZE), sizeof(int));
		UDT::setsockopt(serv, 0, UDT_RCVSYN, new bool(false), sizeof(bool)); // nonblocking recv
	}


	freeaddrinfo(res);


	cout << "server is ready at port: " << service << endl;

	if (UDT::ERROR == UDT::listen(serv, 10))
	{
		cout << "listen: " << UDT::getlasterror().getErrorMessage() << endl;
		return 0;
	}


	sockaddr_storage clientaddr;
	int addrlen = sizeof(clientaddr);

	UDTSOCKET recver;

	while (true)
	{
		CDatagram* datagram;
		do 
		{
			datagram = UDT::recvDatagram(rakNetSocket);
			if (datagram)
			{
				cout << "Got datagram not originating from UDT with length " << datagram->byteLength << endl;
				datagram->Dealloc();
			}
		} while (datagram);

		recver = UDT::accept(serv, (sockaddr*)&clientaddr, &addrlen);
//		if (UDT::INVALID_SOCK == recver)
//		{
//			cout << "accept: " << UDT::getlasterror().getErrorMessage() << endl;
//			return 0;
//		}

		if (recver!=UDT::INVALID_SOCK)
		{
			char clienthost[NI_MAXHOST];
			char clientservice[NI_MAXSERV];
			getnameinfo((sockaddr *)&clientaddr, addrlen, clienthost, sizeof(clienthost), clientservice, sizeof(clientservice), NI_NUMERICHOST|NI_NUMERICSERV);
			cout << "new connection: " << clienthost << ":" << clientservice << endl;

#ifndef WIN32
			pthread_t rcvthread;
			pthread_create(&rcvthread, NULL, recvdata, new UDTSOCKET(recver));
			pthread_detach(rcvthread);
#else
			CreateThread(NULL, 0, recvdata, new UDTSOCKET(recver), 0, NULL);
#endif
		}

		Sleep(100);
		
	}

	UDT::close(serv);

	// use this function to release the UDT library
	UDT::cleanup();

	return 1;
}

#ifndef WIN32
void* recvdata(void* usocket)
#else
DWORD WINAPI recvdata(LPVOID usocket)
#endif
{
	UDTSOCKET recver = *(UDTSOCKET*)usocket;
	delete (UDTSOCKET*)usocket;

	char* data;
	int size;
	if ( hints.ai_socktype == SOCK_DGRAM )
		size=UDPMESSAGESIZE;
	else
		size=10000000;
	data = new char[size];

	if ( hints.ai_socktype == SOCK_DGRAM )
	{
		int rs;
		int rsize = 0;
		while (rsize < UDPMESSAGESIZE)
		{
			do 
			{
				rs = UDT::recvmsg(recver, data + rsize, size - rsize);
				if (UDT::ERROR == rs)
				{
					cout << "recvmsg:" << UDT::getlasterror().getErrorMessage() << endl;
					break;
				}
				rsize += rs;
			} while (rs>0);

			CDatagram* datagram;
			do 
			{
				datagram = UDT::recvDatagram(rakNetSocket);
				if (datagram)
				{
					cout << "Got datagram not originating from UDT with length " << datagram->byteLength << endl;
					datagram->Dealloc();
				}
			} while (datagram);
			
			Sleep(30);
		}
	}

	while (true)
	{
		int rs;
		int rsize = 0;
		while (rsize < size)
		{
			if (UDT::ERROR == (rs = UDT::recv(recver, data + rsize, size - rsize, 0)))
			{
				cout << "recv:" << UDT::getlasterror().getErrorMessage() << endl;
				break;
			}

			rsize += rs;
		}

		if (rsize < size)
			break;
	}

	delete [] data;

	UDT::close(recver);

#ifndef WIN32
	return NULL;
#else
	return 0;
#endif
}
