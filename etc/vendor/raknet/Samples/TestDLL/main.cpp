#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "Router.h"
#include "ConnectionGraph.h"
#include "FileOperations.h"
#include "RakMemoryOverride.h"
#include "DS_List.h"

void* MyMalloc (size_t size)
{
	return malloc(size);
}

void* MyRealloc (void *p, size_t size)
{
	return realloc(p,size);
}

void MyFree (void *p)
{
	free(p);
}



// This project is used to test the DLL system to make sure necessary classes are exported
void main(void)
{
	// Just test allocation and deallocation across the DLL.  If it crashes it failed, otherwise it worked.
	ConsoleServer* a=RakNetworkFactory::GetConsoleServer( );
	ReplicaManager* b=RakNetworkFactory::GetReplicaManager( );
	LogCommandParser* c=RakNetworkFactory::GetLogCommandParser( );
	PacketLogger* d=RakNetworkFactory::GetPacketLogger( );
	RakNetCommandParser* e=RakNetworkFactory::GetRakNetCommandParser( );
	RakPeerInterface * f=RakNetworkFactory::GetRakPeerInterface( );
	Router *g=RakNetworkFactory::GetRouter( );
	ConnectionGraph *h=RakNetworkFactory::GetConnectionGraph( );

	// See RakMemoryOverride.h
	SetMalloc(MyMalloc);
	SetRealloc(MyRealloc);
	SetFree(MyFree);

	char *cArray = RakNet::OP_NEW_ARRAY<char>(10,__FILE__,__LINE__);
	RakNet::OP_DELETE_ARRAY(cArray,__FILE__,__LINE__);

	DataStructures::List<int> intList;
	intList.Push(5);
	
	f->GetMTUSize(UNASSIGNED_SYSTEM_ADDRESS);
	SystemAddress p1;
	SystemAddress p2;
	p1=p2;
	g->Update();

	RakNetworkFactory::DestroyConsoleServer(a);
	RakNetworkFactory::DestroyReplicaManager(b);
	RakNetworkFactory::DestroyLogCommandParser(c);
	RakNetworkFactory::DestroyPacketLogger(d);
	RakNetworkFactory::DestroyRakNetCommandParser(e);
	RakNetworkFactory::DestroyRakPeerInterface(f);
	RakNetworkFactory::DestroyRouter(g);
	RakNetworkFactory::DestroyConnectionGraph(h);
}

