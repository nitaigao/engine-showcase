#ifndef __LOBBY_2_PLUGIN_H
#define __LOBBY_2_PLUGIN_H

#include "Lobby2Message.h"
#include "PluginInterface2.h"
#include "PacketPriority.h"
#include "RakPeerInterface.h"

namespace RakNet
{

enum ServerErrors
{
	// Class factory could not create a message of the given type
	// Followed by 4 bytes, with the message number
	L2SE_UNKNOWN_MESSAGE_ID,
	// Client is trying to run a function that requires admin access. Use Lobby2Server::AddAdminAddress() to add this client.
	L2SE_REQUIRES_ADMIN,
};

struct Lobby2MessageFactory;

/// Both the server and pc client plugins derive from this class.
class RAK_DLL_EXPORT Lobby2Plugin : public PluginInterface2
{
public:
	Lobby2Plugin();
	~Lobby2Plugin();
	
	/// Ordering channel to use with RakPeer::Send()
	void SetOrderingChannel(char oc);

	/// Send priority to use with RakPeer::Send()
	void SetSendPriority(PacketPriority pp);

	// Creates messages from message IDs
	// Server should get a factory that creates messages with database functionality
	// Client can use the base class
	void SetMessageFactory(Lobby2MessageFactory *f);
	Lobby2MessageFactory* GetMessageFactory(void) const;

	/// Set the callback to receive the results of operations via SendMsg()
	virtual void SetCallbackInterface(Lobby2Callbacks *cb);	
	virtual void AddCallbackInterface(Lobby2Callbacks *cb);	
	virtual void RemoveCallbackInterface(Lobby2Callbacks *cb);
	virtual void ClearCallbackInterfaces();
	
protected:
		
	char orderingChannel;
	PacketPriority packetPriority;	
	Lobby2MessageFactory *msgFactory;

	DataStructures::List<Lobby2Callbacks*> callbacks;
};

}; // namespace RakNet

#endif
