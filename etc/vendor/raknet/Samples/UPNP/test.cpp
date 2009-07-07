#include "upnpnat.h"

// KevinJ:
#include "SocketLayer.h"
#include "RakPeerInterface.h"
#include "RakNetworkFactory.h"

int main (int argc,char * argv[]){
	
	UPNPNAT nat;
	nat.init();

	if(!nat.discovery()){
		printf("discovery error is %s\n",nat.get_last_error());
		return -1;
	}

	// KevinJ: Get our own IP address
	RakPeerInterface *rakPeer = RakNetworkFactory::GetRakPeerInterface();
	SystemAddress mySystemAddress = rakPeer->GetInternalID(UNASSIGNED_SYSTEM_ADDRESS);

	// KevinJ: Incoming port will be mySystemAddress.port
	if(!nat.add_port_mapping("test",mySystemAddress.ToString(false),mySystemAddress.port,mySystemAddress.port,"UDP")){
		printf("add_port_mapping error is %s\n",nat.get_last_error());
		return -1;
	}

//	if(!nat.add_port_mapping("test","192.168.12.117",1234,1234,"TCP")){
//		printf("add_port_mapping error is %s\n",nat.get_last_error());
//		return -1;
//	}
	
	printf("add port mapping succ.\n");
	RakNetworkFactory::DestroyRakPeerInterface(rakPeer);

	return 0;
}
