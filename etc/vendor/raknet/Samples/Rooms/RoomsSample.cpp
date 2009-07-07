#include "RoomsPlugin.h"
#include "RakPeerInterface.h"
#include "RakNetworkFactory.h"
#include "ProfanityFilter.h"
#include "MessageIdentifiers.h"
#include "Kbhit.h"
#include <ctype.h>

struct SampleCallbacks : public RakNet::RoomsCallback
{
	// Results of calls
	virtual void CreateRoom_Callback( SystemAddress senderAddress, RakNet::CreateRoom_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void EnterRoom_Callback( SystemAddress senderAddress, RakNet::EnterRoom_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void JoinByFilter_Callback( SystemAddress senderAddress, RakNet::JoinByFilter_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void LeaveRoom_Callback( SystemAddress senderAddress, RakNet::LeaveRoom_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void GetInvitesToParticipant_Callback( SystemAddress senderAddress, RakNet::GetInvitesToParticipant_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void SendInvite_Callback( SystemAddress senderAddress, RakNet::SendInvite_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void AcceptInvite_Callback( SystemAddress senderAddress, RakNet::AcceptInvite_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void StartSpectating_Callback( SystemAddress senderAddress, RakNet::StartSpectating_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void StopSpectating_Callback( SystemAddress senderAddress, RakNet::StopSpectating_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void GrantModerator_Callback( SystemAddress senderAddress, RakNet::GrantModerator_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void ChangeSlotCounts_Callback( SystemAddress senderAddress, RakNet::ChangeSlotCounts_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void SetCustomRoomProperties_Callback( SystemAddress senderAddress, RakNet::SetCustomRoomProperties_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void GetRoomProperties_Callback( SystemAddress senderAddress, RakNet::GetRoomProperties_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void ChangeRoomName_Callback( SystemAddress senderAddress, RakNet::ChangeRoomName_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void SetHiddenFromSearches_Callback( SystemAddress senderAddress, RakNet::SetHiddenFromSearches_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void SetDestroyOnModeratorLeave_Callback( SystemAddress senderAddress, RakNet::SetDestroyOnModeratorLeave_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void SetReadyStatus_Callback( SystemAddress senderAddress, RakNet::SetReadyStatus_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void GetReadyStatus_Callback( SystemAddress senderAddress, RakNet::GetReadyStatus_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void SetRoomLockState_Callback( SystemAddress senderAddress, RakNet::SetRoomLockState_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void GetRoomLockState_Callback( SystemAddress senderAddress, RakNet::GetRoomLockState_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void AreAllMembersReady_Callback( SystemAddress senderAddress, RakNet::AreAllMembersReady_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void KickMember_Callback( SystemAddress senderAddress, RakNet::KickMember_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void UnbanMember_Callback( SystemAddress senderAddress, RakNet::UnbanMember_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void GetBanReason_Callback( SystemAddress senderAddress, RakNet::GetBanReason_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void AddUserToQuickJoin_Callback( SystemAddress senderAddress, RakNet::AddUserToQuickJoin_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void RemoveUserFromQuickJoin_Callback( SystemAddress senderAddress, RakNet::RemoveUserFromQuickJoin_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void IsInQuickJoin_Callback( SystemAddress senderAddress, RakNet::IsInQuickJoin_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void SearchByFilter_Callback( SystemAddress senderAddress, RakNet::SearchByFilter_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void ChangeHandle_Callback( SystemAddress senderAddress, RakNet::ChangeHandle_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	virtual void Chat_Callback( SystemAddress senderAddress, RakNet::Chat_Func *callResult) {(void) senderAddress; callResult->PrintResult();}
	// Notifications due to other room members
	virtual void QuickJoinExpired_Callback( SystemAddress senderAddress, RakNet::QuickJoinExpired_Notification *notification) {(void) senderAddress; notification->PrintResult();}
	virtual void QuickJoinEnteredRoom_Callback( SystemAddress senderAddress, RakNet::QuickJoinEnteredRoom_Notification *notification) {(void) senderAddress; notification->PrintResult();}
	virtual void RoomMemberStartedSpectating_Callback( SystemAddress senderAddress, RakNet::RoomMemberStartedSpectating_Notification *notification) {(void) senderAddress; notification->PrintResult();}
	virtual void RoomMemberStoppedSpectating_Callback( SystemAddress senderAddress, RakNet::RoomMemberStoppedSpectating_Notification *notification) {(void) senderAddress; notification->PrintResult();}
	virtual void ModeratorChanged_Callback( SystemAddress senderAddress, RakNet::ModeratorChanged_Notification *notification) {(void) senderAddress; notification->PrintResult();}
	virtual void SlotCountsSet_Callback( SystemAddress senderAddress, RakNet::SlotCountsSet_Notification *notification) {(void) senderAddress; notification->PrintResult();}
	virtual void CustomRoomPropertiesSet_Callback( SystemAddress senderAddress, RakNet::CustomRoomPropertiesSet_Notification *notification) {(void) senderAddress; notification->PrintResult();}
	virtual void RoomNameSet_Callback( SystemAddress senderAddress, RakNet::RoomNameSet_Notification *notification) {(void) senderAddress; notification->PrintResult();}
	virtual void HiddenFromSearchesSet_Callback( SystemAddress senderAddress, RakNet::HiddenFromSearchesSet_Notification *notification) {(void) senderAddress; notification->PrintResult();}
	virtual void RoomMemberReadyStatusSet_Callback( SystemAddress senderAddress, RakNet::RoomMemberReadyStatusSet_Notification *notification) {(void) senderAddress; notification->PrintResult();}
	virtual void RoomLockStateSet_Callback( SystemAddress senderAddress, RakNet::RoomLockStateSet_Notification *notification) {(void) senderAddress; notification->PrintResult();}
	virtual void RoomMemberKicked_Callback( SystemAddress senderAddress, RakNet::RoomMemberKicked_Notification *notification) {(void) senderAddress; notification->PrintResult();}
	virtual void RoomMemberHandleSet_Callback( SystemAddress senderAddress, RakNet::RoomMemberHandleSet_Notification *notification) {(void) senderAddress; notification->PrintResult();}
	virtual void RoomMemberLeftRoom_Callback( SystemAddress senderAddress, RakNet::RoomMemberLeftRoom_Notification *notification) {(void) senderAddress; notification->PrintResult();}
	virtual void RoomMemberJoinedRoom_Callback( SystemAddress senderAddress, RakNet::RoomMemberJoinedRoom_Notification *notification) {(void) senderAddress; notification->PrintResult();}
	virtual void RoomInvitationSent_Callback( SystemAddress senderAddress, RakNet::RoomInvitationSent_Notification *notification) {(void) senderAddress; notification->PrintResult();}
	virtual void RoomInvitationWithdrawn_Callback( SystemAddress senderAddress, RakNet::RoomInvitationWithdrawn_Notification *notification) {(void) senderAddress; notification->PrintResult();}
	virtual void RoomDestroyedOnModeratorLeft_Callback( SystemAddress senderAddress, RakNet::RoomDestroyedOnModeratorLeft_Notification *notification) {(void) senderAddress; notification->PrintResult();}
	virtual void Chat_Callback( SystemAddress senderAddress, RakNet::Chat_Notification *notification) {(void) senderAddress; notification->PrintResult(); printf("Chat=%s\nFiltered=%s\n", notification->chatMessage.C_String(), notification->filteredChatMessage.C_String());}
};

static const char *GAME_IDENTIFIER="My Game";
static const char *ROOM_NAME="My Room";

void GetUserName(RakNet::RoomsPluginFunc *func)
{
	printf("Which user? 1=User1, 2=User2, 3=User3: ");
	char ch = getche();
	if (ch=='1')
		func->userName="User1";
	else if (ch=='2')
		func->userName="User2";
	else
		func->userName="User3";
	printf("\n");
}
void GetRoomName(RakNet::RakString &dest)
{
	char buff[256];
	printf("Enter room name, or enter for default: ");
	gets(buff);
	if (buff[0]==0)
		dest=ROOM_NAME;
	else
		dest=buff;
}
void GetTargetName(RakNet::RakString *target)
{
	printf("Which target user? 1=User1, 2=User2, 3=User3: ");
	char ch = getche();
	if (ch=='1')
		*target="User1";
	else if (ch=='2')
		*target="User2";
	else
		*target="User3";
	printf("\n");
}
void main(void)
{
	printf("A system for creating rooms for players to meet in before starting games.\n");
	printf("Difficulty: Intermediate\n\n");

	// Do the unit test to make sure the core functionality is correct. The plugin just does networking
	//RakNet::AllGamesRoomsContainer::UnitTest();

	RakPeerInterface *client, *server;
	RakNet::RoomsPlugin roomsPluginClient, roomsPluginServer;
	client = RakNetworkFactory::GetRakPeerInterface();
	server = RakNetworkFactory::GetRakPeerInterface();
	client->Startup(1,0,&SocketDescriptor(0,0), 1);
	server->Startup(1,0,&SocketDescriptor(1234,0), 1);
	server->SetMaximumIncomingConnections(1);
	client->AttachPlugin(&roomsPluginClient);
	server->AttachPlugin(&roomsPluginServer);
	ProfanityFilter profanityFilter;
	profanityFilter.AddWord("Crapola");
	roomsPluginServer.SetProfanityFilter(&profanityFilter);
	roomsPluginServer.roomsContainer.AddTitle(GAME_IDENTIFIER);
	SampleCallbacks sampleCallbacks;
	SystemAddress localAddress;
	localAddress.SetBinaryAddress("127.0.0.1");
	localAddress.port=1234;
	roomsPluginClient.SetServerAddress(localAddress);
	roomsPluginClient.SetRoomsCallback(&sampleCallbacks);
	client->Connect("127.0.0.1", 1234, 0, 0, 0);

	printf("A. CreateRoom\n");
	printf("B. EnterRoom\n");
	printf("C. JoinByFilter\n");
	printf("D. LeaveRoom\n");
	printf("E. GetInvitesToParticipant\n");
	printf("F. SendInvite\n");
	printf("G. AcceptInvite\n");
	printf("H. StartSpectating\n");
	printf("I. StopSpectating\n");
	printf("J. GrantModerator\n");
	printf("K. ChangeSlotCounts\n");
	printf("L. SetCustomRoomProperties\n");
	printf("M. ChangeRoomName\n");
	printf("N. SetHiddenFromSearches\n");
	printf("O. SetDestroyOnModeratorLeave\n");
	printf("P. SetReadyStatus\n");
	printf("Q. GetReadyStatus\n");
	printf("R. SetRoomLockState\n");
	printf("S. GetRoomLockState\n");
	printf("T. AreAllMembersReady\n");
	printf("U. KickMember\n");
	printf("V. UnbanMember\n");
	printf("W. GetBanReason\n");
	printf("X. AddUserToQuickJoin\n");
	printf("Y. RemoveUserFromQuickJoin\n");
	printf("Z. IsInQuickJoin\n");
	printf("1. SearchByFilter\n");
	printf("2. ChangeHandle\n");
	printf("3. RoomChat\n");
	printf("4. GetRoomProperties\n");

	Packet *p;
	char ch;
	while (1)
	{
		p=client->Receive();
		if (p)
		{
			if (p->data[0]==ID_CONNECTION_REQUEST_ACCEPTED)
				printf("ID_CONNECTION_REQUEST_ACCEPTED\n");
			else if (p->data[0]==ID_CONNECTION_ATTEMPT_FAILED)
				printf("ID_CONNECTION_ATTEMPT_FAILED\n");
			else if (p->data[0]==ID_NO_FREE_INCOMING_CONNECTIONS)
				printf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
			client->DeallocatePacket(p);
		}
		p=server->Receive();
		if (p)
		{
			if (p->data[0]==ID_NEW_INCOMING_CONNECTION)
			{
				roomsPluginServer.LoginRoomsParticipant("User1", p->systemAddress, p->guid, UNASSIGNED_SYSTEM_ADDRESS);
				roomsPluginServer.LoginRoomsParticipant("User2", p->systemAddress, p->guid, UNASSIGNED_SYSTEM_ADDRESS);
				roomsPluginServer.LoginRoomsParticipant("User3", p->systemAddress, p->guid, UNASSIGNED_SYSTEM_ADDRESS);
			}
			server->DeallocatePacket(p);
		}
		
		if (kbhit())
		{
			ch = getch();
			switch (toupper(ch))
			{
			case 'A':
				{
					printf("CreateRoom\n");
					RakNet::CreateRoom_Func func;
					GetUserName(&func);
					GetRoomName(func.networkedRoomCreationParameters.roomName);
					func.networkedRoomCreationParameters.slots.publicSlots=1;
					func.gameIdentifier=GAME_IDENTIFIER;
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case 'B':
				{
					printf("EnterRoom\n");
					RakNet::EnterRoom_Func func;
					GetUserName(&func);
					// Create or join the specified room name
					GetRoomName(func.networkedRoomCreationParameters.roomName);
					func.query.AddQuery_STRING(
						DefaultRoomColumns::GetColumnName(DefaultRoomColumns::TC_ROOM_NAME),
						func.networkedRoomCreationParameters.roomName.C_String());
					func.networkedRoomCreationParameters.slots.publicSlots=2;
					func.roomMemberMode=RMM_PUBLIC;
					func.gameIdentifier=GAME_IDENTIFIER;
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case 'C':
				{
					printf("JoinByFilter\n");
					RakNet::JoinByFilter_Func func;
					GetUserName(&func);
					RakNet::RakString roomName;
					GetRoomName(roomName);
					func.query.AddQuery_STRING(
						DefaultRoomColumns::GetColumnName(DefaultRoomColumns::TC_ROOM_NAME),
						roomName.C_String());
					func.gameIdentifier=GAME_IDENTIFIER;
					func.roomMemberMode=RMM_PUBLIC;
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case 'D':
				{
					printf("LeaveRoom\n");
					RakNet::LeaveRoom_Func func;
					GetUserName(&func);
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case 'E':
				{
					printf("GetInvitesToParticipant\n");
					RakNet::GetInvitesToParticipant_Func func;
					GetUserName(&func);
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case 'F':
				{
					printf("SendInvite\n");
					RakNet::SendInvite_Func func;
					GetUserName(&func);
					GetTargetName(&func.inviteeName);
					func.inviteToSpectatorSlot=false;
					func.subject="SendInviteSubject";
					func.body="SendInviteSubject";
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case 'G':
				{
					printf("AcceptInvite\n");
					RakNet::AcceptInvite_Func func;
					GetUserName(&func);
					GetTargetName(&func.inviteSender);
					func.roomId=1;
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case 'H':
				{
					printf("StartSpectating\n");
					RakNet::StartSpectating_Func func;
					GetUserName(&func);
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case 'I':
				{
					printf("StopSpectating\n");
					RakNet::StopSpectating_Func func;
					GetUserName(&func);
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case 'J':
				{
					printf("GrantModerator\n");
					RakNet::GrantModerator_Func func;
					GetUserName(&func);
					GetTargetName(&func.newModerator);
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case 'K':
				{
					printf("ChangeSlotCounts\n");
					RakNet::ChangeSlotCounts_Func func;
					GetUserName(&func);
					func.slots.publicSlots=1;
					func.slots.reservedSlots=0;
					func.slots.spectatorSlots=1;
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case 'L':
				{
					printf("SetCustomRoomProperties\n");
					RakNet::SetCustomRoomProperties_Func func;
					GetUserName(&func);
					DataStructures::Table customRoomProperties;
					func.table.AddColumn("Custom Column 1", DataStructures::Table::STRING);
					func.table.AddColumn("Custom Column 2", DataStructures::Table::NUMERIC);
					DataStructures::Table::Row* row = func.table.AddRow(0);
					row->cells[0]->Set("Custom Column 1 value");
					row->cells[1]->Set(12345);
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case 'M':
				{
					printf("ChangeRoomName\n");
					RakNet::ChangeRoomName_Func func;
					GetUserName(&func);
					func.newRoomName="New room name";
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case 'N':
				{
					printf("SetHiddenFromSearches\n");
					RakNet::SetHiddenFromSearches_Func func;
					GetUserName(&func);
					func.hiddenFromSearches=true;
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case 'O':
				{
					printf("SetDestroyOnModeratorLeave\n");
					RakNet::SetDestroyOnModeratorLeave_Func func;
					GetUserName(&func);
					func.destroyOnModeratorLeave=true;
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case 'P':
				{
					printf("SetReadyStatus\n");
					RakNet::SetReadyStatus_Func func;
					GetUserName(&func);
					func.isReady=true;
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case 'Q':
				{
					printf("GetReadyStatus\n");
					RakNet::GetReadyStatus_Func func;
					GetUserName(&func);
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case 'R':
				{
					printf("SetRoomLockState\n");
					RakNet::SetRoomLockState_Func func;
					GetUserName(&func);
					func.roomLockState=RakNet::RLS_ALL_LOCKED;
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case 'S':
				{
					printf("GetRoomLockState\n");
					RakNet::GetRoomLockState_Func func;
					GetUserName(&func);
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case 'T':
				{
					printf("AreAllMembersReady\n");
					RakNet::AreAllMembersReady_Func func;
					GetUserName(&func);
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case 'U':
				{
					printf("KickMember\n");
					RakNet::KickMember_Func func;
					GetUserName(&func);
					GetTargetName(&func.kickedMember);
					func.reason="KickMemberReason";
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case 'V':
				{
					printf("UnbanMember\n");
					RakNet::UnbanMember_Func func;
					GetUserName(&func);
					GetTargetName(&func.bannedMemberName);
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case 'W':
				{
					printf("GetBanReason\n");
					RakNet::GetBanReason_Func func;
					GetUserName(&func);
					func.roomId=1;
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case 'X':
				{
					printf("AddUserToQuickJoin\n");
					RakNet::AddUserToQuickJoin_Func func;
					GetUserName(&func);
					func.networkedQuickJoinUser.timeout=5000;
					func.gameIdentifier=GAME_IDENTIFIER;
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case 'Y':
				{
					printf("RemoveUserFromQuickJoin\n");
					RakNet::RemoveUserFromQuickJoin_Func func;
					GetUserName(&func);
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case 'Z':
				{
					printf("IsInQuickJoin\n");
					RakNet::IsInQuickJoin_Func func;
					GetUserName(&func);
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case '1':
				{
					printf("SearchByFilter\n");
					RakNet::SearchByFilter_Func func;
					GetUserName(&func);
					func.gameIdentifier=GAME_IDENTIFIER;
					func.onlyJoinable=true;
					// Not specifying any search parameters returns all rooms
				//	RakNet::RakString roomName;
				//	GetRoomName(roomName);
				//	func.roomQuery.AddQuery_STRING(
				//		DefaultRoomColumns::GetColumnName(DefaultRoomColumns::TC_ROOM_NAME),
				//		roomName.C_String());
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case '2':
				{
					printf("ChangeHandle\n");
					RakNet::ChangeHandle_Func func;
					GetUserName(&func);
					func.newHandle="Crapola";
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case '3':
				{
					printf("RoomChat\n");
					RakNet::Chat_Func func;
					GetUserName(&func);
					func.chatMessage="Hello world. This is Crapola";
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			case '4':
				{
					printf("GetRoomProperties\n");
					RakNet::GetRoomProperties_Func func;
					GetUserName(&func);
					GetRoomName(func.roomName);
					roomsPluginClient.ExecuteFunc(&func);
				}
				break;
			}
		}
	}

	RakNetworkFactory::DestroyRakPeerInterface(client);
	RakNetworkFactory::DestroyRakPeerInterface(server);
}
