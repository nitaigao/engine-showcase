#ifndef __LOBBY_2_MESSAGE_H
#define __LOBBY_2_MESSAGE_H

#include "BitStream.h"
#include "Lobby2ResultCode.h"
#include "RakString.h"
#include "RakAssert.h"

#if defined(_XBOX) || defined(X360)
#include "XBOX360Includes.h"
#endif

#pragma once

namespace RakNet
{

struct Lobby2Callbacks;
class Lobby2Client;
class Lobby2Server;
struct BinaryDataBlock;
struct Lobby2ServerCommand;

const unsigned int L2_MAX_BINARY_DATA_LENGTH=1000000;


// --------------------------------------------- Enumeration list of all message IDs --------------------------------------------

/// All enumerations for Lobby2Message. Each Lobby2Message has one enumeration
enum Lobby2MessageID
{
	L2MID_Platform_Startup,
	L2MID_Platform_Shutdown,
	L2MID_System_CreateDatabase,
	L2MID_System_DestroyDatabase,
	L2MID_System_CreateTitle,
	L2MID_System_DestroyTitle,
	L2MID_System_GetTitleRequiredAge,
	L2MID_System_GetTitleBinaryData,
	L2MID_System_RegisterProfanity,
	L2MID_System_BanUser,
	L2MID_System_UnbanUser,
	L2MID_CDKey_Add,
	L2MID_CDKey_GetStatus,
	L2MID_CDKey_Use,
	L2MID_CDKey_FlagStolen,
	L2MID_Client_Login,
	L2MID_Client_Logoff,
	L2MID_Client_RegisterAccount,
	L2MID_System_SetEmailAddressValidated,
	L2MID_Client_ValidateHandle,
	L2MID_System_DeleteAccount,
	L2MID_System_PruneAccounts,
	L2MID_Client_GetEmailAddress,
	L2MID_Client_GetPasswordRecoveryQuestionByHandle,
	L2MID_Client_GetPasswordByPasswordRecoveryAnswer,
	L2MID_Client_ChangeHandle,
	L2MID_Client_UpdateAccount,
	L2MID_Client_StartIgnore,
	L2MID_Client_StopIgnore,
	L2MID_Client_GetIgnoreList,
	L2MID_Friends_SendInvite,
	L2MID_Friends_AcceptInvite,
	L2MID_Friends_RejectInvite,
	L2MID_Friends_GetInvites,
	L2MID_Friends_GetFriends,
	L2MID_Friends_Remove,
	L2MID_BookmarkedUsers_Add,
	L2MID_BookmarkedUsers_Remove,
	L2MID_BookmarkedUsers_Get,
	L2MID_Emails_Send,
	L2MID_Emails_Get,
	L2MID_Emails_Delete,
	L2MID_Emails_SetStatus,
	L2MID_Ranking_SubmitMatch,
	L2MID_Ranking_GetMatches,
	L2MID_Ranking_GetMatchBinaryData,
	L2MID_Ranking_GetTotalScore,
	L2MID_Ranking_WipeScoresForPlayer,
	L2MID_Ranking_WipeMatches,
	L2MID_Ranking_PruneMatches,
	L2MID_Ranking_UpdateRating,
	L2MID_Ranking_WipeRatings,
	L2MID_Ranking_GetRating,
	L2MID_Clans_Create,
	L2MID_Clans_SetProperties,
	L2MID_Clans_GetProperties,
	L2MID_Clans_SetMyMemberProperties,
	L2MID_Clans_GrantLeader,
	L2MID_Clans_SetSubleaderStatus,
	L2MID_Clans_SetMemberRank,
	L2MID_Clans_GetMemberProperties,
	L2MID_Clans_ChangeHandle,
	L2MID_Clans_Leave,
	L2MID_Clans_Get,
	L2MID_Clans_SendJoinInvitation,
	L2MID_Clans_WithdrawJoinInvitation,
	L2MID_Clans_AcceptJoinInvitation,
	L2MID_Clans_RejectJoinInvitation,
	L2MID_Clans_DownloadInvitationList,
	L2MID_Clans_SendJoinRequest,
	L2MID_Clans_WithdrawJoinRequest,
	L2MID_Clans_AcceptJoinRequest,
	L2MID_Clans_RejectJoinRequest,
	L2MID_Clans_DownloadRequestList,
	L2MID_Clans_KickAndBlacklistUser,
	L2MID_Clans_UnblacklistUser,
	L2MID_Clans_GetBlacklist,
	L2MID_Clans_GetMembers,
	L2MID_Clans_CreateBoard,
	L2MID_Clans_DestroyBoard,
	L2MID_Clans_CreateNewTopic,
	L2MID_Clans_ReplyToTopic,
	L2MID_Clans_RemovePost,
	L2MID_Clans_GetBoards,
	L2MID_Clans_GetTopics,
	L2MID_Clans_GetPosts,
	L2MID_Console_GetServerStatus,
	L2MID_Console_GetWorldListFromServer,
	L2MID_Console_GetLobbyListFromWorld,
	L2MID_Console_JoinLobby,
	L2MID_Console_LeaveLobby,
	L2MID_Console_SendLobbyChatMessage,
	L2MID_Console_SearchRoomsInLobby,
	L2MID_Console_GetRoomDetails,
	L2MID_Console_GetLobbyMemberData,
	L2MID_Console_CreateRoom,
	L2MID_Console_JoinRoom,
	L2MID_Console_LeaveRoom,
	L2MID_Console_SendLobbyInvitationToRoom,
	L2MID_Console_SendRoomChatMessage,
	L2MID_Notification_Client_IgnoreStatus,
	L2MID_Notification_Friends_StatusChange,
	L2MID_Notification_User_ChangedHandle,
	L2MID_Notification_Friends_CreatedClan,
	L2MID_Notification_Emails_Received,
	L2MID_Notification_Clans_GrantLeader,
	L2MID_Notification_Clans_SetSubleaderStatus,
	L2MID_Notification_Clans_SetMemberRank,
	L2MID_Notification_Clans_ChangeHandle,
	L2MID_Notification_Clans_Leave,
	L2MID_Notification_Clans_PendingJoinStatus,
	L2MID_Notification_Clans_NewClanMember,
	L2MID_Notification_Clans_KickAndBlacklistUser,
	L2MID_Notification_Clans_UnblacklistUser,
	L2MID_Notification_Clans_Destroyed,
	L2MID_Notification_Console_CableDisconnected,
	L2MID_Notification_Console_ContextError,
	L2MID_Notification_Console_MemberJoinedLobby,
	L2MID_Notification_Console_MemberLeftLobby,
	L2MID_Notification_Console_LobbyDestroyed,
	L2MID_Notification_Console_LobbyMemberDataUpdated,
	L2MID_Notification_Console_LobbyGotChatMessage,
	L2MID_Notification_Console_LobbyGotRoomInvitation,
	L2MID_Notification_Console_MemberJoinedRoom,
	L2MID_Notification_Console_MemberLeftRoom,
	L2MID_Notification_Console_KickedOutOfRoom,
	L2MID_Notification_Console_RoomWasDestroyed,
	L2MID_Notification_Console_RoomOwnerChanged,
	L2MID_Notification_Console_RoomStateChanged,
	L2MID_Notification_Console_RoomChatMessage,
	L2MID_Notification_Console_RoomMessage,
	L2MID_Notification_Console_RoomMemberConnectivityUpdate,
	L2MID_Notification_Console_ChatEvent,
	L2MID_Notification_Console_MuteListChanged,
	L2MID_Notification_Console_Local_Users_Changed,
	L2MID_Notification_Console_Signaling_Result,


	L2MID_COUNT,
};

// Should match tab;e lobby2.clanMemberStates
enum ClanMemberState
{
	CMD_UNDEFINED=0,
	CMD_ACTIVE,
	CMD_BANNED,
	CMD_JOIN_INVITED,
	CMD_JOIN_REQUESTED,
};
// --------------------------------------------- Base class for all messages (functions and notifications --------------------------------------------

/// A Lobby2Message encapsulates a networked function call from the client.
/// The client should fill in the input parameters, call Lobby2Client::SendMsg(), and wait for the reply in the callback passed to Lobby2Client::SetCallbackInterface()
/// The input parameters are always serialized back from the server.
/// See resultCode for the result of the operation. L2RC_SUCCESS means success. Anything else means failure.
/// Any message may return between L2RC_NOT_LOGGED_IN and L2RC_EMAIL_ADDRESS_IS_INVALID, which indices formatting errors in the input.
/// All other return codes have the name of the message in the enumeration.
/// The system can be extended by deriving from Lobby2Message, adding your own input and output parameters, and deriving from Lobby2MessageFactory register your own class factory with RakNet::Lobby2Plugin::SetMessageFactory()
struct Lobby2Message
{
	Lobby2Message();
	virtual ~Lobby2Message() {}

	/// Every message has an ID identifying it across the network
	virtual Lobby2MessageID GetID(void) const=0;

	/// Is this message something that should only be run by a system with admin privileges?
	/// Set admin privileges with Lobby2Server::AddAdminAddress()
	virtual bool RequiresAdmin(void) const=0;

	/// Is this message something that should only be run by a system with ranking upload priviledges?
	/// Set ranking privileges with Lobby2Server::AddRankingAddress()
	virtual bool RequiresRankingPermission(void) const=0;

	/// Should this message not be processed on the server if the requesting user disconnects before it completes?
	/// This should be true for functions that only return data. False for functions that affect other users, or change the database
	virtual bool CancelOnDisconnect(void) const=0;

	/// Does this function require logging into the server before it can be executed?
	/// If true, the user id and user handle will be automatically inferred by the last login by looking up the sender's system address.
	/// If false, the message should include the username so the database query can lookup which user is performing this operation.
	virtual bool RequiresLogin(void) const=0;
	
	// Serialize data in this class. Currently just the resultCode
	void SerializeBase(bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream);
	
	/// Overridable serialization of the contents of this message. Defaults to SerializeBase()
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );
	
	/// If data members can be validated for correctness in the server's main thread, override this function and do those checks here.
	/// \return True for input OK. False if the input is bad and does not need to be further processed in the database threads.
	virtual bool PrevalidateInput(void);
	
	/// Override to do any Lobby2Client functionality when the message is returned from the server (usually nothing).
	/// \return True to call CallCallback immediately. False to defer for some reason (always true on the PC)
	virtual bool ClientImpl( Lobby2Client *client);
	
	/// This message has been processed by the server and has arrived back on the client.
	/// Call the client informing the user of this event.
	virtual void CallCallback(Lobby2Callbacks *cb)=0;
	
	/// Do any Lobby2Server	functionality when the message first arrives on the server, and after it has returned true from PrevalidateInput()
	/// If it returns true, the message has been handled, and the result is sent to the client
	/// If it returns false, the message continues to ServerDBImpl
	virtual bool ServerMemoryImpl( Lobby2Server *server, SystemAddress systemAddress );
	
	/// Do any Lobby2Server	functionality when the message is processed in a database thread on the server.
	/// It is safe to do slow database calls in this function.
	/// If it returns true, the message has been handled, and the result is sent to the client
	/// If it returns false, that means ignore the message
	virtual bool ServerDBImpl( Lobby2ServerCommand *command, void *databaseInterface );

	/// The only non-printable character is space
	/// Cannot start with space
	/// Cannot end with space
	/// Cannot have two spaces in a row
	bool ValidateHandle( RakNet::RakString *handle );

	/// Binary data cannot be longer than L2_MAX_BINARY_DATA_LENGTH
	bool ValidateBinary( BinaryDataBlock *binaryDataBlock);

	/// Required text cannot be empty.
	bool ValidateRequiredText( RakNet::RakString *text );

	/// Passwords must contain at least 5 characters
	bool ValidatePassword( RakNet::RakString *text );

	/// Check email address format
	bool ValidateEmailAddress( RakNet::RakString *text );

	/// Convert the enumeration representing this message to a string, and return it. Done automatically by macros.
	virtual const char *GetName(void) const=0;
	
	/// Write the result of this message to out(). Done automatically by macros.
	virtual void DebugMsg(RakNet::RakString &out) const=0;

	/// Print the result of DebugMsg
	virtual void DebugPrintf(void) const
	{
		RakNet::RakString out; DebugMsg(out); printf(out.C_String());
	}
	
	/// Result of the operation. L2RC_SUCCESS means the result completed. Anything else means an error
	RakNet::Lobby2ResultCode resultCode;

	/// Just a number, uniquely identifying each allocation of Lobby2Message.
	/// Use it if you need to lookup queries on the callback reply
#if defined(_PS3) || defined(__PS3__) || defined(SN_TARGET_PS3)
	uint32_t requestId;
#else
	uint64_t requestId;
#endif

	/// Just a number, representing which instance of Lobby2Callbacks should process the result of this operation
	/// -1 means all
	unsigned char callbackId;

	/// Used for consoles
	int extendedResultCode;

	/// Reference count
	int refCount;

#if defined(_XBOX) || defined(X360)
	XOVERLAPPED        m_Overlapped;
#endif
};

// --------------------------------------------- Prototypes --------------------------------------------

struct Platform_Startup;
struct Platform_Shutdown;
struct System_CreateDatabase;
struct System_DestroyDatabase;
struct System_CreateTitle;
struct System_DestroyTitle;
struct System_GetTitleRequiredAge;
struct System_GetTitleBinaryData;
struct System_RegisterProfanity;
struct System_BanUser;
struct System_UnbanUser;
struct CDKey_Add;
struct CDKey_GetStatus;
struct CDKey_Use;
struct CDKey_FlagStolen;
struct Client_Login;
struct Client_Logoff;
struct Client_RegisterAccount;
struct System_SetEmailAddressValidated;
struct Client_ValidateHandle;
struct System_DeleteAccount;
struct System_PruneAccounts;
struct Client_GetEmailAddress;
struct Client_GetPasswordRecoveryQuestionByHandle;
struct Client_GetPasswordByPasswordRecoveryAnswer;
struct Client_ChangeHandle;
struct Client_UpdateAccount;
struct Client_StartIgnore;
struct Client_StopIgnore;
struct Client_GetIgnoreList;
struct Friends_SendInvite;
struct Friends_AcceptInvite;
struct Friends_RejectInvite;
struct Friends_GetInvites;
struct Friends_GetFriends;
struct Friends_Remove;
struct BookmarkedUsers_Add;
struct BookmarkedUsers_Remove;
struct BookmarkedUsers_Get;
struct Emails_Send;
struct Emails_Get;
struct Emails_Delete;
struct Emails_SetStatus;
struct Ranking_SubmitMatch;
struct Ranking_GetMatches;
struct Ranking_GetMatchBinaryData;
struct Ranking_GetTotalScore;
struct Ranking_WipeScoresForPlayer;
struct Ranking_WipeMatches;
struct Ranking_PruneMatches;
struct Ranking_UpdateRating;
struct Ranking_WipeRatings;
struct Ranking_GetRating;
struct Clans_Create;
struct Clans_SetProperties;
struct Clans_GetProperties;
struct Clans_SetMyMemberProperties;
struct Clans_GrantLeader;
struct Clans_SetSubleaderStatus;
struct Clans_SetMemberRank;
struct Clans_GetMemberProperties;
struct Clans_ChangeHandle;
struct Clans_Leave;
struct Clans_Get;
struct Clans_SendJoinInvitation;
struct Clans_WithdrawJoinInvitation;
struct Clans_AcceptJoinInvitation;
struct Clans_RejectJoinInvitation;
struct Clans_DownloadInvitationList;
struct Clans_SendJoinRequest;
struct Clans_WithdrawJoinRequest;
struct Clans_AcceptJoinRequest;
struct Clans_RejectJoinRequest;
struct Clans_DownloadRequestList;
struct Clans_KickAndBlacklistUser;
struct Clans_UnblacklistUser;
struct Clans_GetBlacklist;
struct Clans_GetMembers;
struct Clans_CreateBoard;
struct Clans_DestroyBoard;
struct Clans_CreateNewTopic;
struct Clans_ReplyToTopic;
struct Clans_RemovePost;
struct Clans_GetBoards;
struct Clans_GetTopics;
struct Clans_GetPosts;
struct Console_GetServerStatus;
struct Console_GetWorldListFromServer;
struct Console_GetLobbyListFromWorld;
struct Console_JoinLobby;
struct Console_LeaveLobby;
struct Console_SendLobbyChatMessage;
struct Console_SearchRoomsInLobby;
struct Console_GetRoomDetails;
struct Console_GetLobbyMemberData;
struct Console_CreateRoom;
struct Console_JoinRoom;
struct Console_LeaveRoom;
struct Console_SendLobbyInvitationToRoom;
struct Console_SendRoomChatMessage;
struct Notification_Client_IgnoreStatus;
struct Notification_Friends_StatusChange;
struct Notification_User_ChangedHandle;
struct Notification_Friends_CreatedClan;
struct Notification_Emails_Received;
struct Notification_Clans_GrantLeader;
struct Notification_Clans_SetSubleaderStatus;
struct Notification_Clans_SetMemberRank;
struct Notification_Clans_ChangeHandle;
struct Notification_Clans_Leave;
struct Notification_Clans_PendingJoinStatus;
struct Notification_Clans_NewClanMember;
struct Notification_Clans_KickAndBlacklistUser;
struct Notification_Clans_UnblacklistUser;
struct Notification_Clans_Destroyed;
struct Notification_Console_CableDisconnected;
struct Notification_Console_ContextError;
struct Notification_Console_MemberJoinedLobby;
struct Notification_Console_MemberLeftLobby;
struct Notification_Console_LobbyDestroyed;
struct Notification_Console_LobbyMemberDataUpdated;
struct Notification_Console_LobbyGotChatMessage;
struct Notification_Console_LobbyGotRoomInvitation;
struct Notification_Console_MemberJoinedRoom;
struct Notification_Console_MemberLeftRoom;
struct Notification_Console_KickedOutOfRoom;
struct Notification_Console_RoomWasDestroyed;
struct Notification_Console_RoomOwnerChanged;
struct Notification_Console_RoomStateChanged;
struct Notification_Console_RoomChatMessage;
struct Notification_Console_RoomMessage;
struct Notification_Console_RoomMemberConnectivityUpdate;
struct Notification_Console_ChatEvent;
struct Notification_Console_MuteListChanged;
struct Notification_Console_Local_Users_Changed;
struct Notification_Console_Signaling_Result;

// --------------------------------------------- Callback interface for all messages, notifies the user --------------------------------------------

/// Every Lobby2Message processed with Lobby2Client::SendMsg() while connected will call the callback registered with Lobby2Client::SetCallbackInterface().
struct Lobby2Callbacks
{
	Lobby2Callbacks() {callbackId=nextCallbackId++;}
	virtual ~Lobby2Callbacks() {}
	virtual void MessageResult(Platform_Startup *message);
	virtual void MessageResult(Platform_Shutdown *message);
	virtual void MessageResult(System_CreateDatabase *message);
	virtual void MessageResult(System_DestroyDatabase *message);
	virtual void MessageResult(System_CreateTitle *message);
	virtual void MessageResult(System_DestroyTitle *message);
	virtual void MessageResult(System_GetTitleRequiredAge *message);
	virtual void MessageResult(System_GetTitleBinaryData *message);
	virtual void MessageResult(System_RegisterProfanity *message);
	virtual void MessageResult(System_BanUser *message);
	virtual void MessageResult(System_UnbanUser *message);
	virtual void MessageResult(CDKey_Add *message);
	virtual void MessageResult(CDKey_GetStatus *message);
	virtual void MessageResult(CDKey_Use *message);
	virtual void MessageResult(CDKey_FlagStolen *message);
	virtual void MessageResult(Client_Login *message);
	virtual void MessageResult(Client_Logoff *message);
	virtual void MessageResult(Client_RegisterAccount *message);
	virtual void MessageResult(System_SetEmailAddressValidated *message);
	virtual void MessageResult(Client_ValidateHandle *message);
	virtual void MessageResult(System_DeleteAccount *message);
	virtual void MessageResult(System_PruneAccounts *message);
	virtual void MessageResult(Client_GetEmailAddress *message);
	virtual void MessageResult(Client_GetPasswordRecoveryQuestionByHandle *message);
	virtual void MessageResult(Client_GetPasswordByPasswordRecoveryAnswer *message);
	virtual void MessageResult(Client_ChangeHandle *message);
	virtual void MessageResult(Client_UpdateAccount *message);
	virtual void MessageResult(Client_StartIgnore *message);
	virtual void MessageResult(Client_StopIgnore *message);
	virtual void MessageResult(Client_GetIgnoreList *message);
	virtual void MessageResult(Friends_SendInvite *message);
	virtual void MessageResult(Friends_AcceptInvite *message);
	virtual void MessageResult(Friends_RejectInvite *message);
	virtual void MessageResult(Friends_GetInvites *message);
	virtual void MessageResult(Friends_GetFriends *message);
	virtual void MessageResult(Friends_Remove *message);
	virtual void MessageResult(BookmarkedUsers_Add *message);
	virtual void MessageResult(BookmarkedUsers_Remove *message);
	virtual void MessageResult(BookmarkedUsers_Get *message);
	virtual void MessageResult(Emails_Send *message);
	virtual void MessageResult(Emails_Get *message);
	virtual void MessageResult(Emails_Delete *message);
	virtual void MessageResult(Emails_SetStatus *message);
	virtual void MessageResult(Ranking_SubmitMatch *message);
	virtual void MessageResult(Ranking_GetMatches *message);
	virtual void MessageResult(Ranking_GetMatchBinaryData *message);
	virtual void MessageResult(Ranking_GetTotalScore *message);
	virtual void MessageResult(Ranking_WipeScoresForPlayer *message);
	virtual void MessageResult(Ranking_WipeMatches *message);
	virtual void MessageResult(Ranking_PruneMatches *message);
	virtual void MessageResult(Ranking_UpdateRating *message);
	virtual void MessageResult(Ranking_WipeRatings *message);
	virtual void MessageResult(Ranking_GetRating *message);
	virtual void MessageResult(Clans_Create *message);
	virtual void MessageResult(Clans_SetProperties *message);
	virtual void MessageResult(Clans_GetProperties *message);
	virtual void MessageResult(Clans_SetMyMemberProperties *message);
	virtual void MessageResult(Clans_GrantLeader *message);
	virtual void MessageResult(Clans_SetSubleaderStatus *message);
	virtual void MessageResult(Clans_SetMemberRank *message);
	virtual void MessageResult(Clans_GetMemberProperties *message);
	virtual void MessageResult(Clans_ChangeHandle *message);
	virtual void MessageResult(Clans_Leave *message);
	virtual void MessageResult(Clans_Get *message);
	virtual void MessageResult(Clans_SendJoinInvitation *message);
	virtual void MessageResult(Clans_WithdrawJoinInvitation *message);
	virtual void MessageResult(Clans_AcceptJoinInvitation *message);
	virtual void MessageResult(Clans_RejectJoinInvitation *message);
	virtual void MessageResult(Clans_DownloadInvitationList *message);
	virtual void MessageResult(Clans_SendJoinRequest *message);
	virtual void MessageResult(Clans_WithdrawJoinRequest *message);
	virtual void MessageResult(Clans_AcceptJoinRequest *message);
	virtual void MessageResult(Clans_RejectJoinRequest *message);
	virtual void MessageResult(Clans_DownloadRequestList *message);
	virtual void MessageResult(Clans_KickAndBlacklistUser *message);
	virtual void MessageResult(Clans_UnblacklistUser *message);
	virtual void MessageResult(Clans_GetBlacklist *message);
	virtual void MessageResult(Clans_GetMembers *message);
	virtual void MessageResult(Clans_CreateBoard *message);
	virtual void MessageResult(Clans_DestroyBoard *message);
	virtual void MessageResult(Clans_CreateNewTopic *message);
	virtual void MessageResult(Clans_ReplyToTopic *message);
	virtual void MessageResult(Clans_RemovePost *message);
	virtual void MessageResult(Clans_GetBoards *message);
	virtual void MessageResult(Clans_GetTopics *message);
	virtual void MessageResult(Clans_GetPosts *message);
	virtual void MessageResult(Console_GetServerStatus *message);
	virtual void MessageResult(Console_GetWorldListFromServer *message);
	virtual void MessageResult(Console_GetLobbyListFromWorld *message);
	virtual void MessageResult(Console_JoinLobby *message);
	virtual void MessageResult(Console_LeaveLobby *message);
	virtual void MessageResult(Console_SendLobbyChatMessage *message);
	virtual void MessageResult(Console_SearchRoomsInLobby *message);
	virtual void MessageResult(Console_GetRoomDetails *message);
	virtual void MessageResult(Console_GetLobbyMemberData *message);
	virtual void MessageResult(Console_CreateRoom *message);
	virtual void MessageResult(Console_JoinRoom *message);
	virtual void MessageResult(Console_LeaveRoom *message);
	virtual void MessageResult(Console_SendLobbyInvitationToRoom *message);
	virtual void MessageResult(Console_SendRoomChatMessage *message);
	virtual void MessageResult(Notification_Client_IgnoreStatus *message);
	virtual void MessageResult(Notification_Friends_StatusChange *message);
	virtual void MessageResult(Notification_User_ChangedHandle *message);
	virtual void MessageResult(Notification_Friends_CreatedClan *message);
	virtual void MessageResult(Notification_Emails_Received *message);
	virtual void MessageResult(Notification_Clans_GrantLeader *message);
	virtual void MessageResult(Notification_Clans_SetSubleaderStatus *message);
	virtual void MessageResult(Notification_Clans_SetMemberRank *message);
	virtual void MessageResult(Notification_Clans_ChangeHandle *message);
	virtual void MessageResult(Notification_Clans_Leave *message);
	virtual void MessageResult(Notification_Clans_PendingJoinStatus *message);
	virtual void MessageResult(Notification_Clans_NewClanMember *message);
	virtual void MessageResult(Notification_Clans_KickAndBlacklistUser *message);
	virtual void MessageResult(Notification_Clans_UnblacklistUser *message);
	virtual void MessageResult(Notification_Clans_Destroyed *message);
	virtual void MessageResult(Notification_Console_CableDisconnected *message);
	virtual void MessageResult(Notification_Console_ContextError *message);
	virtual void MessageResult(Notification_Console_MemberJoinedLobby *message);
	virtual void MessageResult(Notification_Console_MemberLeftLobby *message);
	virtual void MessageResult(Notification_Console_LobbyDestroyed *message);
	virtual void MessageResult(Notification_Console_LobbyMemberDataUpdated *message);
	virtual void MessageResult(Notification_Console_LobbyGotChatMessage *message);
	virtual void MessageResult(Notification_Console_LobbyGotRoomInvitation *message);
	virtual void MessageResult(Notification_Console_MemberJoinedRoom *message);
	virtual void MessageResult(Notification_Console_MemberLeftRoom *message);
	virtual void MessageResult(Notification_Console_KickedOutOfRoom *message);
	virtual void MessageResult(Notification_Console_RoomWasDestroyed *message);
	virtual void MessageResult(Notification_Console_RoomOwnerChanged *message);
	virtual void MessageResult(Notification_Console_RoomStateChanged *message);
	virtual void MessageResult(Notification_Console_RoomChatMessage *message);
	virtual void MessageResult(Notification_Console_RoomMessage *message);
	virtual void MessageResult(Notification_Console_RoomMemberConnectivityUpdate *message);
	virtual void MessageResult(Notification_Console_ChatEvent *message);
	virtual void MessageResult(Notification_Console_MuteListChanged *message);
	virtual void MessageResult(Notification_Console_Local_Users_Changed *message);
	virtual void MessageResult(Notification_Console_Signaling_Result *message);

	virtual void ExecuteDefaultResult(Lobby2Message *message) { (void)message; }

	unsigned char callbackId;
	static unsigned char nextCallbackId;
};

/// Just print out the name of the message by default. This class is used in the sample.
struct Lobby2Printf : public Lobby2Callbacks
{
	virtual void ExecuteDefaultResult(Lobby2Message *message) {message->DebugPrintf();}
};

// --------------------------------------------- Types --------------------------------------------

struct BinaryDataBlock
{
	char *binaryData;
	unsigned int binaryDataLength;
	BinaryDataBlock() {binaryData=0; binaryDataLength=0;}
	~BinaryDataBlock() {if (binaryData) rakFree_Ex(binaryData, __FILE__, __LINE__ );}
	void Serialize(bool writeToBitstream, RakNet::BitStream *bitStream);
};
struct CreateAccountParameters
{
	/// [in] Self-apparent
	RakNet::RakString firstName;
	/// [in] Self-apparent
	RakNet::RakString middleName;
	/// [in] Self-apparent
	RakNet::RakString lastName;
	/// [in] Self-apparent
	RakNet::RakString race;
	/// [in] Self-apparent
	bool sex_male;
	/// [in] Self-apparent
	RakNet::RakString homeAddress1;
	/// [in] Self-apparent
	RakNet::RakString homeAddress2;
	/// [in] Self-apparent
	RakNet::RakString homeCity;
	/// [in] Self-apparent
	RakNet::RakString homeState;
	/// [in] Self-apparent
	RakNet::RakString homeCountry;
	/// [in] Self-apparent
	RakNet::RakString homeZipCode;
	/// [in] Self-apparent
	RakNet::RakString billingAddress1;
	/// [in] Self-apparent
	RakNet::RakString billingAddress2;
	/// [in] Self-apparent
	RakNet::RakString billingCity;
	/// [in] Self-apparent
	RakNet::RakString billingState;
	/// [in] Self-apparent
	RakNet::RakString billingCountry;
	/// [in] Self-apparent
	RakNet::RakString billingZipCode;
	/// [in] Self-apparent
	RakNet::RakString emailAddress;
	/// [in] Self-apparent
	RakNet::RakString password;
	/// [in] If the user needs to retrieve their password; you could ask them this question.
	RakNet::RakString passwordRecoveryQuestion;
	/// [in] If the user needs to retrieve their password; you could use this for the answer.
	RakNet::RakString passwordRecoveryAnswer;
	/// [in] Lobbies often allow users to set a text description of their user in some fashion.
	RakNet::RakString caption1;
	/// [in] Lobbies often allow users to set a text description of their user in some fashion.
	RakNet::RakString caption2;
	/// [in] Self-apparent
	unsigned int ageInDays;
	/// [in] binary data
	BinaryDataBlock binaryData;

	void Serialize(bool writeToBitstream, RakNet::BitStream *bitStream);
};
struct PendingInvite
{
	RakNet::RakString sender;
	RakNet::RakString subject;
	RakNet::RakString body;
	BinaryDataBlock binaryData;

	void Serialize(bool writeToBitstream, RakNet::BitStream *bitStream);
};
struct EmailResult
{
	RakNet::RakString sender;
	RakNet::RakString recipient;
	RakNet::RakString subject;
	RakNet::RakString body;
	unsigned int status;
	bool wasSendByMe;
	bool wasReadByMe;
	unsigned int emailID; // Unique ID for this email, used in Emails_Delete, etc.
	BinaryDataBlock binaryData;
	RakNet::RakString creationDate;

	void Serialize(bool writeToBitstream, RakNet::BitStream *bitStream);
};
struct MatchParticipant
{
	MatchParticipant() {}
	MatchParticipant(RakNet::RakString _handle, float _score) : handle(_handle), score(_score) {}
	RakNet::RakString handle;
	float score;

	void Serialize(bool writeToBitstream, RakNet::BitStream *bitStream);
};
struct SubmittedMatch
{
	DataStructures::List<MatchParticipant> matchParticipants;
	RakNet::RakString matchNote;
	BinaryDataBlock binaryData;
	// Use EpochTimeToString to convert to a date
	double whenSubmittedDate;
	unsigned int matchID; // Unique key, Output parameter to Ranking_GetMatches

	void Serialize(bool writeToBitstream, RakNet::BitStream *bitStream);
};
struct ClanInfo
{
	RakNet::RakString clanName;
	RakNet::RakString description;
	RakNet::RakString clanLeader;
	BinaryDataBlock binaryData;
	DataStructures::List<RakNet::RakString> clanMembersOtherThanLeader;

	void Serialize(bool writeToBitstream, RakNet::BitStream *bitStream);
};
struct OpenInvite
{
	RakNet::RakString clanHandle;
	void Serialize(bool writeToBitstream, RakNet::BitStream *bitStream);
};
struct ClanJoinRequest
{
	RakNet::RakString targetClan;
	RakNet::RakString dateSent;
	RakNet::RakString joinRequestSender;
	void Serialize(bool writeToBitstream, RakNet::BitStream *bitStream);
};

struct BookmarkedUser
{
	RakNet::RakString targetHandle;
	int type;
	RakNet::RakString description;
	RakNet::RakString dateWhenAdded;
	void Serialize(bool writeToBitstream, RakNet::BitStream *bitStream);
};

// --------------------------------------------- Actual body of all messages, including DB specific implementation --------------------------------------------

#define __L2_MSG_BASE_IMPL(__NAME__) \
	virtual void CallCallback(Lobby2Callbacks *cb) {cb->MessageResult(this);}; \
	virtual Lobby2MessageID GetID(void) const {return (Lobby2MessageID) L2MID_##__NAME__;} \
	virtual const char* GetName(void) const {return #__NAME__;} \
	virtual void DebugMsg(RakNet::RakString &out) const {out.Set(#__NAME__ " result=%s\n", Lobby2ResultCodeDescription::ToEnglish(resultCode));};

// Platform specific startup. Unused on the PC
struct Platform_Startup : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Platform_Startup)
	virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual bool PrevalidateInput(void) {return true;}
	virtual bool ServerMemoryImpl( Lobby2Server *server, SystemAddress systemAddress ) { (void)server; (void)systemAddress; return true; }
};

// Platform specific startup. Unused on the PC
struct Platform_Shutdown : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Platform_Shutdown)
	virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual bool PrevalidateInput(void) {return true;}
	virtual bool ServerMemoryImpl( Lobby2Server *server, SystemAddress systemAddress ) { (void)server; (void)systemAddress; return true; }
};

/// Create all tables and stored procedures on a system that does not already have them
struct System_CreateDatabase : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(System_CreateDatabase)
	virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual bool PrevalidateInput(void) {return true;}
};
/// Destroy all tables and stored procedures created with System_CreateDatabase
struct System_DestroyDatabase : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(System_DestroyDatabase)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual bool PrevalidateInput(void) {return true;}
};
/// Each title essentially corresponds to a game. For example, the same lobby system may be used for both asteroids and Pac-man. When logging in, and for some functions, it is necessary to specify which title you are logging in under. This way users playing asteroids do not interact with users playing pac-man, where such interations are game specific (such as ranking).
struct System_CreateTitle : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(System_CreateTitle)
	virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );
	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString titleName;
	RakNet::RakString titleSecretKey;
	int requiredAge;
	BinaryDataBlock binaryData;
};
struct System_DestroyTitle : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(System_DestroyTitle)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );
	virtual bool PrevalidateInput(void) {return true;}

	// Input parameters
	RakNet::RakString titleName;

};
struct System_GetTitleRequiredAge : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(System_GetTitleRequiredAge)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );
	virtual bool PrevalidateInput(void) {return true;}

	// Input parameters
	RakNet::RakString titleName;

	// Output parameters
	int requiredAge;
};
struct System_GetTitleBinaryData : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(System_GetTitleBinaryData)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );
	virtual bool PrevalidateInput(void) {return true;}

	// Input parameters
	RakNet::RakString titleName;

	// Output parameters
	BinaryDataBlock binaryData;
};
/// Adds the input strings to a table of profanity. non-unique or empty strings can be ignored. This table will be used internally to ensure that handles and clan names do not contain profanity. Multiple calls add to the table. This table will be used for functions that take a user-defined string that is highly visible, such as clan and user names. It does not need to be checked for emails or message boards.
struct System_RegisterProfanity : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(System_RegisterProfanity)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );
	virtual bool PrevalidateInput(void)
	{
		for (unsigned int i=0; i < profanityWords.Size(); i++)
		{
			if (profanityWords[i].IsEmpty())
			{
				resultCode=L2RC_System_RegisterProfanity_CANNOT_REGISTER_EMPTY_STRINGS;
				return false;
			}
		}
		return true;
	}

	// Input parameters
	DataStructures::List<RakNet::RakString> profanityWords;

	// Output parameters

};
/// Bans a specific user (will be most likely called by a moderator). Adds the user's primary key to a ban table, along with the name of the moderator, the reason for the ban. Banning is used to prevent the banned user from logging on for some specified duration. A date column should be present and automatically filled in. When bans are expired, the ban can be deleted from the database. However, a separate table should log bans, so that even expired bans can be looked up in case.
struct System_BanUser : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(System_BanUser)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );
	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString banReason;
	unsigned int durationHours;
	RakNet::RakString userName;

	// Output parameters


};
struct System_UnbanUser : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(System_UnbanUser)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString reason;
	RakNet::RakString userName;
};
/// Adds CDKeys to the database. Duplicate CDKeys for a particular title are ignored. CDKeys can be identical for different titles.
struct CDKey_Add : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(CDKey_Add)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	DataStructures::List<RakNet::RakString> cdKeys;
	RakNet::RakString titleName;

	// Output parameters
};
/// Returns if a CD key was previously added with AddCDKey.
struct CDKey_GetStatus : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(CDKey_GetStatus)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString cdKey;
	RakNet::RakString titleName;

	// Output parameters
	bool usable;
	RakNet::RakString usedBy;
	RakNet::RakString activationDate;
	bool wasStolen;
};
/// Associates a cd key with a user, such that the cd key cannot be used again. If Client_Login() is called with check cd key as true, then this table will be checked to make sure UserCDKey() was previously called with this user and a valid key. If this user is already associated with a CD Key, add the new key, and use the most recent key. All CD Key usage should be logged in a separate table, including the date used and result.
struct CDKey_Use : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(CDKey_Use)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString cdKey;
	RakNet::RakString titleName;
	RakNet::RakString userName;

	// Output parameters
};

/// Flags one or more CD keys as stolen. Stolen CD keys will prevent Client_Login() if check cd key is true. They will also prevent these cd keys from being used with CDKey_Use. If this key is already in use by certain users for this particular title, then log this similarly to how CDKey_Use does so.
struct CDKey_FlagStolen : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(CDKey_FlagStolen)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString cdKey;
	RakNet::RakString titleName;
	bool wasStolen;

	// Output parameters
	RakNet::RakString userUsingThisKey;
};
/// Once a client creates an account with Client_RegisterAccount, the client is able to logon. The login process will check
/// 1. The CDKey associated with this user (See CDKey_Use) if checkCDKey is true
/// 2. The userPassword passed to this function
/// 3. The titleName and titleSecretKey, to ensure this title was previously created with System_CreateTitle
/// 4. If allowLoginWithoutEmailAddressValidation==false for this user (See Client_RegisterAccount) and System_SetEmailAddressValidated was not called for that email address, fail.
/// 5. If this user was banned with a ban still in effect via System_BanUser
/// If all checks pass, store in a logging table that the user has logged in at this time. No status flag needs be set, this will be done in C++.
/// \param[in] userHandle 
/// \param[in] userPassword 
/// \param[in] titleName
/// \param[in] titleSecretKey
/// \param[in] checkCDKey
/// \param[out] userPrimaryKey Only returned on success
/// \param[out] bannedReason Only returned on banned
/// \param[out] whenBanned Only returned on banned
/// \param[out] banningModeratorID Only returned on banned
struct Client_Login : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Client_Login)
	virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString userPassword;
	// Used if check 
	RakNet::RakString titleName;
	RakNet::RakString titleSecretKey;
	RakNet::RakString userName;

	// Output parameters
	RakNet::RakString bannedReason;
	RakNet::RakString whenBanned;
	RakNet::RakString bannedExpiration;
};
struct Client_Logoff : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Client_Logoff)
	virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
};
/// This creates a new account in the database, which can be used in a subsequent call to Login. Most parameters are optional. handle is not optional, and must be unique using case-insensitive compare. emailAddress should be validated to have a sensible format, including an @ sign and a period with a 3 letter extension. allowLoginWithoutEmailAddressValidation is used in Client_Login to potentially disallow logon attempts with unverified email addresses.
struct Client_RegisterAccount : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Client_RegisterAccount)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	CreateAccountParameters createAccountParameters;
	// Only used if registration requires a CD key
	RakNet::RakString cdKey;
	RakNet::RakString titleName;
	RakNet::RakString userName;

	// Output parameters
};
/// For the client with the given handle, mark a column emailAddressValidated as true or false as appropriate. This is potentially used in Client_Login
struct System_SetEmailAddressValidated : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(System_SetEmailAddressValidated)
	virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );
	virtual bool PrevalidateInput(void);

	// Input parameters
	bool validated;
	RakNet::RakString userName;

	// Output parameters
};
/// Looks up in the database if this handle is already in use, subject to the usual constraints of handles. This will be used by the user to quickly check for available handles.
struct Client_ValidateHandle : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Client_ValidateHandle)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );
	virtual bool PrevalidateInput(void);

	RakNet::RakString userName;
};
/// Flags as deleted an account registered with RegisterAccount. Accounts are not actually deleted, only tagged as deleted.
struct System_DeleteAccount : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(System_DeleteAccount)
	virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString userName;
	RakNet::RakString password;
};
/// Unused accounts are deleted. This is cascading, such that emails and other tables that reference this key are also deleted. unused accounts are defined as:
/// 1. Deleted accounts over deletedPruneTime seconds old
/// 2. Accounts which have not been logged into for over loggedInPruneTime seconds
struct System_PruneAccounts : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(System_PruneAccounts)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );
	virtual bool PrevalidateInput(void);

	// Input parameters
	unsigned int deleteAccountsNotLoggedInDays;

	// Output parameters
};
/// Returns the email address associated with a specific handle, invalid handle. This is used for password recovery.
struct Client_GetEmailAddress : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Client_GetEmailAddress)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );
	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString userName;

	// Output parameters
	RakNet::RakString emailAddress;
	bool emailAddressValidated;

};
/// Returns the passwordRecoveryQuestion associated with handle, invalid handle
struct Client_GetPasswordRecoveryQuestionByHandle : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Client_GetPasswordRecoveryQuestionByHandle)
		virtual bool RequiresAdmin(void) const {return false;}virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );
	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString userName;

	// Output parameters
	RakNet::RakString passwordRecoveryQuestion;
};
/// Returns the password associated with a handle, if the passwordRecoveryAnswer is correct
struct Client_GetPasswordByPasswordRecoveryAnswer : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Client_GetPasswordByPasswordRecoveryAnswer)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );
	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString userName;
	RakNet::RakString passwordRecoveryAnswer;

	// Output parameters
	RakNet::RakString password;
};
/// Changes the handle for a user.
struct Client_ChangeHandle : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Client_ChangeHandle)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString userName;
	RakNet::RakString newHandle;

	// Output parameters
};

/// Will update any or all of the inputs that were previously passed to Client_RegisterAccount, except handle.
/// For input parameters, see Client_RegisterAccount() createAccountParameters, EXCEPT handle
/// For output parameters, see Client_RegisterAccount()
struct Client_UpdateAccount : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Client_UpdateAccount)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	CreateAccountParameters createAccountParameters;

	// Output parameters
};

/// Adds the specified user to an ignore list for my user. Recommended to store the primary key of the remote user, both for speed and so if the other use changes their handle it still works. The ignore list is checked for friend invites, emails, and elsewhere where indicated. Ignoring is uni-directional, so if A ignores B, A will block messages from B where appropriate, but B will not immediately block messages from A.
struct Client_StartIgnore : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Client_StartIgnore)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	virtual bool ClientImpl( Lobby2Client *client);

	// Input parameters
	RakNet::RakString targetHandle;

	// Output parameters
};

/// Removes an entry in the database such that myHandle will no longer ignore theirHandle.
struct Client_StopIgnore : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Client_StopIgnore)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	virtual bool ClientImpl( Lobby2Client *client);

	// Input parameters
	RakNet::RakString targetHandle;

	// Output parameters

};
/// Returns all users I have ignored
struct Client_GetIgnoreList : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Client_GetIgnoreList)
	virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool ClientImpl( Lobby2Client *client);

	// Input parameters

	// Output parameters
	DataStructures::List<RakNet::RakString> ignoredHandles;
};
/// Stores in the database an add friend invite from my handle to their handle. The combination of my handle and their handle must be unique, so you cannot send more than one add friend invite to a single user. Sends an email to their handle the subject, body, and binary data. Note: if myHandle is ignored by theirHandle, then the function fails. See Client_StartIgnore.
struct Friends_SendInvite : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Friends_SendInvite)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString targetHandle;
	RakNet::RakString subject;
	RakNet::RakString body;
	int emailStatus;
	BinaryDataBlock binaryData;

	// Output parameters
};

/// Stores in the database that this user is now my friend. This is bidirectional, which is to say if someone accepts an add friend invite, they are my friend, and I am their friend. Recommended to store by primary key for speed and in case the friend's handle changes. Store in the emails table from my handle to their handle the subject, body, and binary data. Note: if myHandle is ignored by theirHandle, then the function fails. See AddToIgnoreList.
struct Friends_AcceptInvite : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Friends_AcceptInvite)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString targetHandle;
	RakNet::RakString subject;
	RakNet::RakString body;
	int emailStatus;
	BinaryDataBlock binaryData;

	// Output parameters
};
/// Removes from the database the pending add friend invite. Operation completes even if ignored. Unless ignored, store in the emails table from my handle to their handle the subject, body,  binary data, and procedure type flag.
struct Friends_RejectInvite : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Friends_RejectInvite)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString targetHandle;
	RakNet::RakString subject;
	RakNet::RakString body;
	int emailStatus;
	BinaryDataBlock binaryData;

	// Output parameters
};
/// Returns all invites for this user
struct Friends_GetInvites : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Friends_GetInvites)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	// Input parameters

	// Output parameters
	/// Up to caller to deallocate binaryData if needed
	DataStructures::List<RakNet::RakString> invitesSent;
	DataStructures::List<RakNet::RakString> invitesReceived;
};
/// Getse all friends to this user
struct Friends_GetFriends : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Friends_GetFriends)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );
	// Input parameters

	// Output parameters
	DataStructures::List<RakNet::RakString> myFriends;
};
/// Ends a friendship between two users. Remove from the database the friend entry between my handle and their handle. As with accept add friend invite, this is bidirectional. Either user can terminate the friendship. Store in the emails table from my handle to their handle the subject, body, and binary data, and procedure type flag.
struct Friends_Remove : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Friends_Remove)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString targetHandle;
	RakNet::RakString subject;
	RakNet::RakString body;
	int emailStatus;
	BinaryDataBlock binaryData;

	// Output parameters
};
/// Remembers a user, with a type integer and description for you to use, if desired.
/// Can be used for recent users or other types of lists
/// The combination of targetHandle and type uniquely identifies a bookmarked user.
/// If you want more than one list of bookmarked usrs, use a different value for type
struct BookmarkedUsers_Add : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(BookmarkedUsers_Add)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString targetHandle;
	int type;
	RakNet::RakString description;

};
struct BookmarkedUsers_Remove : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(BookmarkedUsers_Remove)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString targetHandle;
	int type;
};
/// Returns all users added to BookmarkedUsers_Add
struct BookmarkedUsers_Get : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(BookmarkedUsers_Get)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	// Input parameters

	// Output parameters
	/// \param[out] recentlyMetUsers Handles of recently met users, by BookmarkedUsers_Add, subject to expirationTimeSeconds
	DataStructures::List<BookmarkedUser> bookmarkedUsers;
};
/// Adds to an emails table from myHandle (store primary key) to recipient handles (store primary key) the specified subject, body, and binary data. Emails are persistent, therefore emails should be stored in a separate table and referenced by the user. Deleting the user does not delete previously send email. Emails should have an automatic timestamp to store when they were created. Email should be flagged as sent=true (boolean), markedRead=true (boolean), deletedBySender=false (boolean), deletedByReciever=false (boolean).
struct Emails_Send : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Emails_Send)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	DataStructures::List<RakNet::RakString> recipients;
	RakNet::RakString subject;
	RakNet::RakString body;
	int status;
	BinaryDataBlock binaryData;

	// Output parameters
};
/// Returns emails as noted. Emails which were marked deleted are not returned.
struct Emails_Get : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Emails_Get)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	// Input parameters

	// Output parameters
	/// \param[out] emailResults Up to caller to deallocate binary data
	DataStructures::List<EmailResult> emailResults;
};
/// Deletes an email with a specified ID. This ID is returned in GetEmail and should uniquely identify an email (it's fine to use the primary key). Note: Emails are not actually deleted from the database in this function. This just sets the deletedBySender or deletedByReciever flags. Emails are actually stored in a log recording past emails and sender and receiver primary key. They are not truly destroyed until done so with System_PruneAccounts.
struct Emails_Delete : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Emails_Delete)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	// Input parameters
	unsigned int emailId;

	// Output parameters
};
/// Sets the status flag for an email. This is a property defined by and used by the user
struct Emails_SetStatus : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Emails_SetStatus)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );
	virtual bool PrevalidateInput(void);

	// Input parameters
	unsigned int emailId;
	bool updateStatusFlag;
	bool updateMarkedRead;
	int newStatusFlag;
	bool isNowMarkedRead;

	// Output parameters
};
/// Will record in the database the results of a match. This will store in the database the the match which is defined by the the match notes, match id, winner and loser participant primary keys, winner and loser participant scores, and binary data.
struct Ranking_SubmitMatch : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Ranking_SubmitMatch)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return true;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString titleName;
	RakNet::RakString gameType;
	SubmittedMatch submittedMatch;

	// Output parameters
};
/// Gets matches recorded with Ranking_SubmitMatch ordered from most recent to least recent. Each returned match has all columns submitted to Ranking_SubmitMatch, except binary data, which can be retrieved with Ranking_GetMatchBinaryData. Additionally, each returned match returns the primary key of each match, to be passed to Ranking_GetMatchBinaryData
struct Ranking_GetMatches : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Ranking_GetMatches)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString titleName;
	RakNet::RakString gameType;

	// Output parameters
	/// \param[out] submittedMatches (excluding binary data, up to caller to deallocate)
	DataStructures::List<SubmittedMatch> submittedMatches;
};
/// Because of the large amount of binary data potentially returned, this function is used to retrieve binary data for a particular match. 
struct Ranking_GetMatchBinaryData : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Ranking_GetMatchBinaryData)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	// Input parameters
	unsigned int matchID;

	// Output parameters
	BinaryDataBlock binaryData;
};
/// When a match is submitted with Ranking_SubmitMatch, the total running score and number of matches played for each player for each game title and game mode combination should be recorded. Because matches can be pruned wth PruneMatches(), the total score sum and number of scores submitted should be stored, rather than summed up from prior submitted matches.
struct Ranking_GetTotalScore : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Ranking_GetTotalScore)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString titleName;
	RakNet::RakString gameType;
	RakNet::RakString targetHandle;

	// Output parameters
	float scoreSum;
	unsigned int numScoresSubmitted;
};

/// Resets the sum of all submitted scores to 0, the number of scores submitted to 0
struct Ranking_WipeScoresForPlayer : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Ranking_WipeScoresForPlayer)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return true;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString titleName;
	RakNet::RakString gameType;
	RakNet::RakString targetHandle;

	// Output parameters
};
/// Deletes all matches submitted with submit match. Also deletes all scores for all players associated with this titleName and gameType (e.g. same thing that WipeScoresForPlayer does, but for all players).
struct Ranking_WipeMatches : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Ranking_WipeMatches)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return true;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString titleName;
	RakNet::RakString gameType;

	// Output parameters
};
/// Will delete all matches submitted with SubmitMatch over PruneTime days old. Will also prune matches if the total storage space of all matches exceeds PruneSizeMB megabytes in the database.
struct Ranking_PruneMatches : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Ranking_PruneMatches)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return true;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	// Input parameters
	unsigned int pruneTimeDays;

	// Output parameters
};
struct Ranking_UpdateRating : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Ranking_UpdateRating)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return true;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString titleName;
	RakNet::RakString gameType;
	RakNet::RakString targetHandle;
	float targetRating;

	// Output parameters
};
/// Deletes all ratings for all players for this combination of titleName and gameType.
struct Ranking_WipeRatings : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Ranking_WipeRatings)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return true;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString titleName;
	RakNet::RakString gameType;

	// Output parameters
};
/// Get rating for a player
struct Ranking_GetRating : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Ranking_GetRating)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString titleName;
	RakNet::RakString gameType;
	RakNet::RakString targetHandle;

	// Output parameters
	/// \param[out] currentRating Defaults to 100 if no matches submitted yet
	float currentRating;
};

/// userHandle updates the clanDescription and clanBinaryData of a clan with the specified clanHandle. userHandle must be the clan leader.
struct Clans_Create : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_Create)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString clanHandle;
	bool failIfAlreadyInClan;
	bool requiresInvitationsToJoin;
	RakNet::RakString description;
	int emailStatus;
	BinaryDataBlock binaryData;

	// Output parameters
};

/// userHandle updates the clanDescription and clanBinaryData of a clan with the specified clanHandle. userHandle must be the clan leader.
struct Clans_SetProperties : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_SetProperties)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString clanHandle;
	RakNet::RakString description;
	BinaryDataBlock binaryData;
};
/// Returns clanDescription and clanBinaryData for the given clan.
struct Clans_GetProperties : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_GetProperties)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString clanHandle;

	// Output parameters
	RakNet::RakString description;
	BinaryDataBlock binaryData;
};

/// Each member of each clan has the the properties clanMemberDescription and clanMemberBinaryData which default to empty. These properties can be set here, and retrieved via GetClanMemberProperties
struct Clans_SetMyMemberProperties : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_SetMyMemberProperties)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString clanHandle;
	RakNet::RakString description;
	BinaryDataBlock binaryData;

	// Output parameters
};
/// myPrimaryKey becomes a subleader. newLeaderHandle becomes the leader. An email is sent with Emails_Send() to all members with the specified subject and body
struct Clans_GrantLeader : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_GrantLeader)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString clanHandle;
	RakNet::RakString targetHandle;
};
/// Promotes a clan member to a subleader, or demotes a subleader to a regular member. On promotion, email is sent to all members from myPrimary key with the specified subject and body. On demotion, email is sent to all leaders from myPrimary key with the specified subject and body.
struct Clans_SetSubleaderStatus : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_SetSubleaderStatus)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString clanHandle;
	RakNet::RakString targetHandle;
	bool setToSubleader;
};

/// Lets the clan leader set the rank property for a clan member
struct Clans_SetMemberRank : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_SetMemberRank)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString clanHandle;
	RakNet::RakString targetHandle;
	unsigned int newRank;
};
/// Returns properties for a clan member of a given clan
struct Clans_GetMemberProperties : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_GetMemberProperties)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString clanHandle;
	RakNet::RakString targetHandle;

	// Output parameters
	RakNet::RakString description;
	BinaryDataBlock binaryData;
	unsigned int rank;
	bool isSubleader;
	ClanMemberState clanMemberState;
	RakNet::RakString banReason;
};
/// Renames the clan. Note that this may be called asynchronously, in which case the stored procedure should account for this occuring at the same time as another function that uses the old clan handle.
struct Clans_ChangeHandle : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_ChangeHandle)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString oldClanHandle;
	RakNet::RakString newClanHandle;

	// Output parameters
};
/// Remove user identified by myPrimaryKey from clan identified by clanHandle.
/// If this user is the leader of the clan, and dissolveIfClanLeader is true, then also destroy the clan and remove all members from the clan, as well as all data associated with the clan (clan boards, join requests, etc). If the clan is automatically destroyed in this way,  use Emails_Send() to each clan member with clanDissolvedSubject and clanDissolvedBody. The sender of the email should be the clan leader. If the clan is not destroyed, then leadership passes to the oldest subleader. If no subleaders exist, leadership passes to the oldest member. If no other members exist, the clan is destroyed.
struct Clans_Leave : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_Leave)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString clanHandle;
	bool dissolveIfClanLeader;
	RakNet::RakString subject;
	RakNet::RakString body;
	int emailStatus;
	BinaryDataBlock binaryData;

	// Output parameters
	bool wasDissolved;
	RakNet::RakString newClanLeader; // If not dissolved	
};
/// Returns all clans that userHandle is a member of. Clans and clan members should be sorted by name, using ascending or descending sort as specified.
struct Clans_Get : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_Get)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );


	// Input parameters

	// Output parameters
	DataStructures::List<ClanInfo> clans;
};
/// if myPrimaryKey is a leader or subleader of clanHandle, and invitedUserHandle is a valid user not already invited to this clan, add this user to the invite table. The invite table contains the clan, who send the invite, and who the invite was sent to, and when it was sent. Invites expire after expiration time in seconds. Also, use Emails_Send() to send an email from myPrimaryKey to invitedUserHandle with the specified subject and body.
struct Clans_SendJoinInvitation : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_SendJoinInvitation)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString clanHandle;
	RakNet::RakString targetHandle;
	RakNet::RakString subject;
	RakNet::RakString body;
	int emailStatus;
	BinaryDataBlock binaryData;

	// Output parameters
};
/// if myPrimaryKey is a leader or subleader of clanHandle, and invitedUserHandle is a valid user with an invite to this clan, remove this invite.  Also, use Emails_Send() to send an email from myPrimaryKey to invitedUserHandle with the specified subject and body.
struct Clans_WithdrawJoinInvitation : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_WithdrawJoinInvitation)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString clanHandle;
	RakNet::RakString targetHandle;
	RakNet::RakString subject;
	RakNet::RakString body;
	int emailStatus;
	BinaryDataBlock binaryData;
};
/// If myPrimaryKey has an invitation to the specified clan, add him to the clan. Fail on specified output parameters. Use Emails_Send() to send an email from myPrimaryKey to all clan members with the specified subject and body.
struct Clans_AcceptJoinInvitation : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_AcceptJoinInvitation)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString clanHandle;
	RakNet::RakString subject;
	RakNet::RakString body;
	int emailStatus;
	BinaryDataBlock binaryData;
	bool failIfAlreadyInClan;

	// Output parameters
};
/// If we have an open clan invitation, reject it (just delete it from the database).
struct Clans_RejectJoinInvitation : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_RejectJoinInvitation)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString clanHandle;
	RakNet::RakString subject;
	RakNet::RakString body;
	int emailStatus;
	BinaryDataBlock binaryData;

	// Output parameters
};
/// Returns all invites sent by Clans_SendJoinInvitation that were not yet acted upon (withdrawn, accepted, rejected).
struct Clans_DownloadInvitationList : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_DownloadInvitationList)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	// Output parameters
	DataStructures::List<OpenInvite> invitations;
};
/// Function has two forms:
/// If requiresInvitationsToJoin==true when CreateClan() was called, will join the specifeid clan immediately. Sends subject and body to all other members in the clan.
/// If requiresInvitationsToJoin==false when CreateClan() was called, send a join request to the specified clan, if we don't have one already. Join request expires after expiration time in seconds. Also, use Emails_Send() to send an email from myPrimaryKey to the clan leader and all subleaders with the specified subject and body.
struct Clans_SendJoinRequest : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_SendJoinRequest)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString clanHandle;
	RakNet::RakString subject;
	RakNet::RakString body;
	int emailStatus;
	BinaryDataBlock binaryData;

	// Output parameters
	bool clanJoined;
};
/// Withdraws a previously sent clan join request via SendClanJoinRequest.  Use Emails_Send() to send an email from myPrimaryKey to the clan leader and all subleaders with the specified subject and body.
struct Clans_WithdrawJoinRequest : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_WithdrawJoinRequest)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString clanHandle;
	RakNet::RakString subject;
	RakNet::RakString body;
	int emailStatus;
	BinaryDataBlock binaryData;

	// Output parameters

};
/// A clan leader or subleader accepts a join request from requestingUserHandle to this clan. requestingUserHandle joins the clan as a regular member.  Use Emails_Send() to send an email from requestingUserHandle to all clan members with the specified subject and body.
struct Clans_AcceptJoinRequest : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_AcceptJoinRequest)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString clanHandle;
	RakNet::RakString subject;
	RakNet::RakString body;
	int emailStatus;
	BinaryDataBlock binaryData;
	RakNet::RakString requestingUserHandle;
	bool failIfAlreadyInClan;	

	// Output parameters
};
/// Rejects a clan join request from requestingUserHandle. Send an email from myPrimaryKey to requestingUserHandle with the specified subject and body.
struct Clans_RejectJoinRequest : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_RejectJoinRequest)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString clanHandle;
	RakNet::RakString subject;
	RakNet::RakString body;
	int emailStatus;
	BinaryDataBlock binaryData;
	RakNet::RakString requestingUserHandle;

	// Output parameters
};
/// Returns all open requests this user has sent to clans, that have not yet acted upon (withdrawn, accepted, rejected, expired).
struct Clans_DownloadRequestList : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_DownloadRequestList)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );


	// Input parameters

	// Output parameters
	// joinRequestsToMyClan is only filled out for clans you are a leader or subleader in
	DataStructures::List<ClanJoinRequest> joinRequestsToMyClan, joinRequestsFromMe;
};
/// Kicks a user from the clan and/or blacklists a user so they cannot join. Only a clan leader or subleader can perform this operation. The operation can only be performed on members of lower status (leader can perform on subleader or regular member or nonmember, subleader on regular members or nonmember). If a member is banned, they are added to the banned table which contains the member's primary key, which user banned them, and the reason. Email is sent from myPrimaryKey to all leaders if a clan member is banned. Emails is furthermore sent to all clan members if successfully kicked. 
struct Clans_KickAndBlacklistUser : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_KickAndBlacklistUser)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString clanHandle;
	RakNet::RakString subject;
	RakNet::RakString body;
	int emailStatus;
	BinaryDataBlock binaryData;
	RakNet::RakString targetHandle;
	bool kick;
	bool blacklist;
	RakNet::RakString reason;
};
/// Removes a user from the blacklist for this clan.
struct Clans_UnblacklistUser : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_UnblacklistUser)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString clanHandle;
	RakNet::RakString targetHandle;
	RakNet::RakString subject;
	RakNet::RakString body;
	int emailStatus;
	BinaryDataBlock binaryData;
};
/// Returns a list of all members  blacklisted from this clan. Each element in the list contains the handle of the user that did the ban, who was banned, when the user was banned, and the reason passed to ClanKickAndBlacklistUser
struct Clans_GetBlacklist : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_GetBlacklist)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString clanHandle;

	// Output parameters
	DataStructures::List<RakNet::RakString> blacklistedUsers;
};
/// Returns all clan members for this clan. Each entry returned contains handle, description, binary data, status (leader, regular member, subleader).
struct Clans_GetMembers : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_GetMembers)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString clanHandle;

	// Output parameters
	RakNet::RakString clanLeader;
	DataStructures::List<RakNet::RakString> clanMembersOtherThanLeader;
};
/// Creates a new clan board for clan members to post in using AddPostToClanBoard. Clan boards are unique, and are destroyed when the clan is destroyed, or if DestroyClanBoard is called.
struct Clans_CreateBoard : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_CreateBoard)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString clanHandle;
	RakNet::RakString clanBoardName;
	bool allowPublicReads;
	bool allowPublicWrites;
	RakNet::RakString description;
	BinaryDataBlock binaryData;

	// Output parameters
};
/// Success, invalid parameter syntax, myPrimaryKey is not the leader or subleader of clanHandle, unknown myPrimaryKey, unknown clanHandle, unknown clanBoardName
struct Clans_DestroyBoard : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_DestroyBoard)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString clanHandle;
	RakNet::RakString clanBoardName;

	// Output parameters
};
/// Each clan has a clan board that only clan members can post to. This adds a topic to the clan board. Posts should reference the primary key of the poster, so that even if the poster chagnes his or her handle, the post author is updated properly. Each post automatically stores the timestamp when it was created. Banned users may not add new posts to the clan board.
struct Clans_CreateNewTopic : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_CreateNewTopic)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString clanHandle;
	RakNet::RakString clanBoardName;
	RakNet::RakString body;
	RakNet::RakString subject;
	BinaryDataBlock binaryData;

	// Output parameters
	unsigned int postId; // (unique for clanHandle)
};
/// Replies to a topic created with Clans_CreateTopic(). If postId references a post within a topic, just add the reply to the last post.  Banned users may not add new posts to the clan board.
struct Clans_ReplyToTopic : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_ReplyToTopic)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	unsigned int postId; // returned from Clans_CreateTopic()
	RakNet::RakString subject;
	RakNet::RakString body;
	BinaryDataBlock binaryData;
};
/// The clan leader or subleaders may remove posts or topics from a clan board.
struct Clans_RemovePost : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_RemovePost)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	// Input parameters
	unsigned int postId; // returned from Clans_CreateTopic()
	bool removeEntireTopic;


};

/// Gets clan boards created for clanHandle. Boards are returned if we are a clan member, or if allowPublicReads in Clans_CreateBoard() was passed as false. However, if we are banned from this clan, no boards are returned.
struct Clans_GetBoards : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_GetBoards)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString clanHandle;

	// Output parameters
	DataStructures::List<RakNet::RakString> clanBoardsNames;


};
/// Gets topics (posts that are not replies to other posts, created with Clans_CreateTopic()) for the specified clanHandle and clanBoardName. If we are not a clan member and the clan was created with allowPublicReads==false, then the user is not allowed to read topics
struct Clans_GetTopics : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_GetTopics)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	RakNet::RakString clanHandle;
	RakNet::RakString clanBoardName;
};
/// Gets all posts for a particular topic. If postId is not a topic but is instead a post in a topic, treat it as if the topic postId was passed. If we are not a clan member and the clan was created with allowPublicReads==false, then the user is not allowed to read topics
struct Clans_GetPosts : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Clans_GetPosts)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	virtual bool PrevalidateInput(void);

	// Input parameters
	unsigned int postId;
};

// Call the function to get the list of servers available.
// Does nothing on the PC.
struct Console_GetServerStatus : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Console_GetServerStatus)
	virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
};

// Given a server, get the list of worlds
// Does nothing on the PC.
struct Console_GetWorldListFromServer : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Console_GetWorldListFromServer)
	virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
};

// Given a world, get the list of lobbies
// Does nothing on the PC.
struct Console_GetLobbyListFromWorld : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Console_GetLobbyListFromWorld)
	virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
};

// Given a lobby, join that lobby
// Does nothing on the PC.
struct Console_JoinLobby : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Console_JoinLobby)
	virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
};

// Given a lobby, leave that lobby.
// Does nothing on the PC.
struct Console_LeaveLobby : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Console_LeaveLobby)
	virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
};

// Send a message to a lobby that you have joined
// Does nothing on the PC.
struct Console_SendLobbyChatMessage : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Console_SendLobbyChatMessage)
	virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
	
	// Input
	RakNet::RakString message;
};

// Search rooms in the lobby
// Does nothing on the PC.
struct Console_SearchRoomsInLobby : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Console_SearchRoomsInLobby)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
};


// Get information about a room
// Does nothing on the PC.
struct Console_GetRoomDetails : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Console_GetRoomDetails)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
};


// Send a chat message to the lobby
// Does nothing on the PC.
struct Console_GetLobbyMemberData : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Console_GetLobbyMemberData)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
};

struct Console_CreateRoom : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Console_CreateRoom)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}

	int publicSlots;
	int privateSlots;
};

struct Console_JoinRoom : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Console_JoinRoom)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
};

struct Console_LeaveRoom : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Console_LeaveRoom)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
};

struct Console_SendLobbyInvitationToRoom : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Console_SendLobbyInvitationToRoom)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}
};

struct Console_SendRoomChatMessage : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Console_SendRoomChatMessage)
		virtual bool RequiresAdmin(void) const {return false;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return true;}
	virtual bool RequiresLogin(void) const {return true;}

	RakNet::RakString message;
};

struct Notification_Client_IgnoreStatus : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Client_IgnoreStatus)
	virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	bool nowIgnored;
	RakNet::RakString otherHandle;
};
struct Notification_Friends_StatusChange : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Friends_StatusChange)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );
	enum Status
	{
		FRIEND_LOGGED_IN,
		FRIEND_LOGGED_OFF,
		FRIEND_ACCOUNT_WAS_DELETED,
		YOU_WERE_REMOVED_AS_A_FRIEND,
		GOT_INVITATION_TO_BE_FRIENDS,
		THEY_ACCEPTED_OUR_INVITATION_TO_BE_FRIENDS,
		THEY_REJECTED_OUR_INVITATION_TO_BE_FRIENDS,
	} op;
	RakNet::RakString otherHandle;
};
struct Notification_User_ChangedHandle : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_User_ChangedHandle)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	RakNet::RakString oldHandle, newHandle;
};
struct Notification_Friends_CreatedClan : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Friends_CreatedClan)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	RakNet::RakString otherHandle;
	RakNet::RakString clanName;	
};
struct Notification_Emails_Received : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Emails_Received)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	RakNet::RakString sender;
	RakNet::RakString subject;
	unsigned int emailId;
};
struct Notification_Clans_GrantLeader : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Clans_GrantLeader)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	RakNet::RakString clanHandle;
	RakNet::RakString newLeader;
	RakNet::RakString oldLeader;
};
struct Notification_Clans_SetSubleaderStatus : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Clans_SetSubleaderStatus)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	RakNet::RakString clanHandle;
	RakNet::RakString targetHandle;
	RakNet::RakString leaderHandle;
	bool setToSubleader;
};
struct Notification_Clans_SetMemberRank : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Clans_SetMemberRank)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	// Input parameters
	RakNet::RakString clanHandle;
	RakNet::RakString targetHandle;
	RakNet::RakString leaderHandle;
	unsigned int newRank;
};
struct Notification_Clans_ChangeHandle : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Clans_ChangeHandle)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	// Input parameters
	RakNet::RakString oldClanHandle;
	RakNet::RakString newClanHandle;
	RakNet::RakString leaderHandle;
};
struct Notification_Clans_Leave : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Clans_Leave)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	RakNet::RakString clanHandle;
	RakNet::RakString targetHandle;
};
struct Notification_Clans_PendingJoinStatus : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Clans_PendingJoinStatus)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	RakNet::RakString clanHandle;
	RakNet::RakString sourceHandle;
	RakNet::RakString targetHandle;
	// clanMemberHandle is is the clan member that is withdrawing an invitation, or rejecting a request
	RakNet::RakString clanMemberHandle;

	// The combination of major and minor op describe this notification
	// For example, JOIN_CLAN_INVITATION + JOIN_WITHDRAWN means that an invitation to join the clan was withdrawn by targetHandle
	// JOIN_CLAN_REQUEST + JOIN_REJECTED means that our request to join this clan was rejected by targetHandle
	enum MajorOp
	{
		JOIN_CLAN_INVITATION,
		JOIN_CLAN_REQUEST,
	} majorOp;
	enum MinorOp
	{
		JOIN_SENT,
		JOIN_WITHDRAWN,
		JOIN_REJECTED,
	} minorOp;
};
struct Notification_Clans_NewClanMember : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Clans_NewClanMember)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	RakNet::RakString clanHandle;
	RakNet::RakString targetHandle;
};
struct Notification_Clans_KickAndBlacklistUser : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Clans_KickAndBlacklistUser)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	RakNet::RakString clanHandle;
	RakNet::RakString targetHandle;
	RakNet::RakString blacklistingUserHandle;
	// If true, they were both kicked and blacklisted. If false, they were only blacklisted (not currently in the clan)
	bool targetHandleWasKicked;
	RakNet::RakString reason;
};
struct Notification_Clans_UnblacklistUser : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Clans_UnblacklistUser)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	RakNet::RakString clanHandle;
	RakNet::RakString targetHandle;
	// Currently this is always the clan leader, since subleaders cannot unblacklist
	RakNet::RakString unblacklistingUserHandle;
};
struct Notification_Clans_Destroyed : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Clans_Destroyed)
	virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
	virtual void Serialize( bool writeToBitstream, bool serializeOutput, RakNet::BitStream *bitStream );

	RakNet::RakString clanHandle;
	RakNet::RakString oldClanLeader;
};
struct Notification_Console_CableDisconnected : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Console_CableDisconnected)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
};
struct Notification_Console_ContextError : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Console_ContextError)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
};

struct Notification_Console_MemberJoinedLobby : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Console_MemberJoinedLobby)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}

	RakNet::RakString targetHandle;
};

struct Notification_Console_MemberLeftLobby : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Console_MemberLeftLobby)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}

	RakNet::RakString targetHandle;
};

struct Notification_Console_LobbyDestroyed : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Console_LobbyDestroyed)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
};

struct Notification_Console_LobbyMemberDataUpdated : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Console_LobbyMemberDataUpdated)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
};

struct Notification_Console_LobbyGotChatMessage : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Console_LobbyGotChatMessage)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}

	RakNet::RakString sender;
	RakNet::RakString message;
};

struct Notification_Console_LobbyGotRoomInvitation : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Console_LobbyGotRoomInvitation)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}

	RakNet::RakString sender;
};

struct Notification_Console_MemberJoinedRoom : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Console_MemberJoinedRoom)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}

	RakNet::RakString memberName;
};

struct Notification_Console_MemberLeftRoom : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Console_MemberLeftRoom)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}

	RakNet::RakString memberName;
};

struct Notification_Console_KickedOutOfRoom : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Console_KickedOutOfRoom)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
};

struct Notification_Console_RoomWasDestroyed : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Console_RoomWasDestroyed)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
};

struct Notification_Console_RoomOwnerChanged : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Console_RoomOwnerChanged)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
};

struct Notification_Console_RoomStateChanged : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Console_RoomStateChanged)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
};

struct Notification_Console_RoomChatMessage : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Console_RoomChatMessage)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}

	RakNet::RakString sender;
	RakNet::RakString message;
};

struct Notification_Console_RoomMessage : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Console_RoomMessage)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}


	RakNet::RakString sender;
	RakNet::RakString message;
};

struct Notification_Console_RoomMemberConnectivityUpdate : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Console_RoomMemberConnectivityUpdate)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}

	// Out
	SystemAddress systemAddress;
};

struct Notification_Console_ChatEvent : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Console_ChatEvent)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
};

struct Notification_Console_MuteListChanged : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Console_MuteListChanged)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
};

struct Notification_Console_Local_Users_Changed : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Console_Local_Users_Changed)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
};

struct Notification_Console_Signaling_Result : public Lobby2Message
{
	__L2_MSG_BASE_IMPL(Notification_Console_Signaling_Result)
		virtual bool RequiresAdmin(void) const {return true;}
	virtual bool RequiresRankingPermission(void) const {return false;}
	virtual bool CancelOnDisconnect(void) const {return false;}
	virtual bool RequiresLogin(void) const {return false;}
};

// --------------------------------------------- Base interface of factory class for all messages --------------------------------------------
#define __L2_MSG_FACTORY_BASE(__NAME__) {case L2MID_##__NAME__ : Lobby2Message *m = RakNet::OP_NEW< __NAME__ >( __FILE__, __LINE__ ) ; RakAssert(m->GetID()==L2MID_##__NAME__ ); m->requestId=nextRequestId++; return m;}
struct Lobby2MessageFactory
{
	Lobby2MessageFactory() {nextRequestId=0;}
	virtual ~Lobby2MessageFactory() {}
	virtual Lobby2Message *Alloc(Lobby2MessageID id)
	{
		switch (id)
		{
			__L2_MSG_FACTORY_BASE(Platform_Startup);
			__L2_MSG_FACTORY_BASE(Platform_Shutdown);
			__L2_MSG_FACTORY_BASE(System_CreateDatabase);
			__L2_MSG_FACTORY_BASE(System_DestroyDatabase);
			__L2_MSG_FACTORY_BASE(System_CreateTitle);
			__L2_MSG_FACTORY_BASE(System_DestroyTitle);
			__L2_MSG_FACTORY_BASE(System_GetTitleRequiredAge);
			__L2_MSG_FACTORY_BASE(System_GetTitleBinaryData);
			__L2_MSG_FACTORY_BASE(System_RegisterProfanity);
			__L2_MSG_FACTORY_BASE(System_BanUser);
			__L2_MSG_FACTORY_BASE(System_UnbanUser);
			__L2_MSG_FACTORY_BASE(CDKey_Add);
			__L2_MSG_FACTORY_BASE(CDKey_GetStatus);
			__L2_MSG_FACTORY_BASE(CDKey_Use);
			__L2_MSG_FACTORY_BASE(CDKey_FlagStolen);
			__L2_MSG_FACTORY_BASE(Client_Login);
			__L2_MSG_FACTORY_BASE(Client_Logoff);
			__L2_MSG_FACTORY_BASE(Client_RegisterAccount);
			__L2_MSG_FACTORY_BASE(System_SetEmailAddressValidated);
			__L2_MSG_FACTORY_BASE(Client_ValidateHandle);
			__L2_MSG_FACTORY_BASE(System_DeleteAccount);
			__L2_MSG_FACTORY_BASE(System_PruneAccounts);
			__L2_MSG_FACTORY_BASE(Client_GetEmailAddress);
			__L2_MSG_FACTORY_BASE(Client_GetPasswordRecoveryQuestionByHandle);
			__L2_MSG_FACTORY_BASE(Client_GetPasswordByPasswordRecoveryAnswer);
			__L2_MSG_FACTORY_BASE(Client_ChangeHandle);
			__L2_MSG_FACTORY_BASE(Client_UpdateAccount);
			__L2_MSG_FACTORY_BASE(Client_StartIgnore);
			__L2_MSG_FACTORY_BASE(Client_StopIgnore);
			__L2_MSG_FACTORY_BASE(Client_GetIgnoreList);
			__L2_MSG_FACTORY_BASE(Friends_SendInvite);
			__L2_MSG_FACTORY_BASE(Friends_AcceptInvite);
			__L2_MSG_FACTORY_BASE(Friends_RejectInvite);
			__L2_MSG_FACTORY_BASE(Friends_GetInvites);
			__L2_MSG_FACTORY_BASE(Friends_GetFriends);
			__L2_MSG_FACTORY_BASE(Friends_Remove);
			__L2_MSG_FACTORY_BASE(BookmarkedUsers_Add);
			__L2_MSG_FACTORY_BASE(BookmarkedUsers_Remove);
			__L2_MSG_FACTORY_BASE(BookmarkedUsers_Get);
			__L2_MSG_FACTORY_BASE(Emails_Send);
			__L2_MSG_FACTORY_BASE(Emails_Get);
			__L2_MSG_FACTORY_BASE(Emails_Delete);
			__L2_MSG_FACTORY_BASE(Emails_SetStatus);
			__L2_MSG_FACTORY_BASE(Ranking_SubmitMatch);
			__L2_MSG_FACTORY_BASE(Ranking_GetMatches);
			__L2_MSG_FACTORY_BASE(Ranking_GetMatchBinaryData);
			__L2_MSG_FACTORY_BASE(Ranking_GetTotalScore);
			__L2_MSG_FACTORY_BASE(Ranking_WipeScoresForPlayer);
			__L2_MSG_FACTORY_BASE(Ranking_WipeMatches);
			__L2_MSG_FACTORY_BASE(Ranking_PruneMatches);
			__L2_MSG_FACTORY_BASE(Ranking_UpdateRating);
			__L2_MSG_FACTORY_BASE(Ranking_WipeRatings);
			__L2_MSG_FACTORY_BASE(Ranking_GetRating);
			__L2_MSG_FACTORY_BASE(Clans_Create);
			__L2_MSG_FACTORY_BASE(Clans_SetProperties);
			__L2_MSG_FACTORY_BASE(Clans_GetProperties);
			__L2_MSG_FACTORY_BASE(Clans_SetMyMemberProperties);
			__L2_MSG_FACTORY_BASE(Clans_GrantLeader);
			__L2_MSG_FACTORY_BASE(Clans_SetSubleaderStatus);
			__L2_MSG_FACTORY_BASE(Clans_SetMemberRank);
			__L2_MSG_FACTORY_BASE(Clans_GetMemberProperties);
			__L2_MSG_FACTORY_BASE(Clans_ChangeHandle);
			__L2_MSG_FACTORY_BASE(Clans_Leave);
			__L2_MSG_FACTORY_BASE(Clans_Get);
			__L2_MSG_FACTORY_BASE(Clans_SendJoinInvitation);
			__L2_MSG_FACTORY_BASE(Clans_WithdrawJoinInvitation);
			__L2_MSG_FACTORY_BASE(Clans_AcceptJoinInvitation);
			__L2_MSG_FACTORY_BASE(Clans_RejectJoinInvitation);
			__L2_MSG_FACTORY_BASE(Clans_DownloadInvitationList);
			__L2_MSG_FACTORY_BASE(Clans_SendJoinRequest);
			__L2_MSG_FACTORY_BASE(Clans_WithdrawJoinRequest);
			__L2_MSG_FACTORY_BASE(Clans_AcceptJoinRequest);
			__L2_MSG_FACTORY_BASE(Clans_RejectJoinRequest);
			__L2_MSG_FACTORY_BASE(Clans_DownloadRequestList);
			__L2_MSG_FACTORY_BASE(Clans_KickAndBlacklistUser);
			__L2_MSG_FACTORY_BASE(Clans_UnblacklistUser);
			__L2_MSG_FACTORY_BASE(Clans_GetBlacklist);
			__L2_MSG_FACTORY_BASE(Clans_GetMembers);
			__L2_MSG_FACTORY_BASE(Clans_CreateBoard);
			__L2_MSG_FACTORY_BASE(Clans_DestroyBoard);
			__L2_MSG_FACTORY_BASE(Clans_CreateNewTopic);
			__L2_MSG_FACTORY_BASE(Clans_ReplyToTopic);
			__L2_MSG_FACTORY_BASE(Clans_RemovePost);
			__L2_MSG_FACTORY_BASE(Clans_GetBoards);
			__L2_MSG_FACTORY_BASE(Clans_GetTopics);
			__L2_MSG_FACTORY_BASE(Clans_GetPosts);
			__L2_MSG_FACTORY_BASE(Console_GetServerStatus);
			__L2_MSG_FACTORY_BASE(Console_GetWorldListFromServer);
			__L2_MSG_FACTORY_BASE(Console_GetLobbyListFromWorld);
			__L2_MSG_FACTORY_BASE(Console_JoinLobby);
			__L2_MSG_FACTORY_BASE(Console_LeaveLobby);
			__L2_MSG_FACTORY_BASE(Console_SendLobbyChatMessage);
			__L2_MSG_FACTORY_BASE(Console_SearchRoomsInLobby);
			__L2_MSG_FACTORY_BASE(Console_GetRoomDetails);
			__L2_MSG_FACTORY_BASE(Console_GetLobbyMemberData);
			__L2_MSG_FACTORY_BASE(Console_CreateRoom);
			__L2_MSG_FACTORY_BASE(Console_JoinRoom);
			__L2_MSG_FACTORY_BASE(Console_LeaveRoom);
			__L2_MSG_FACTORY_BASE(Console_SendLobbyInvitationToRoom);
			__L2_MSG_FACTORY_BASE(Console_SendRoomChatMessage);
			__L2_MSG_FACTORY_BASE(Notification_Client_IgnoreStatus);
			__L2_MSG_FACTORY_BASE(Notification_Friends_StatusChange);
			__L2_MSG_FACTORY_BASE(Notification_User_ChangedHandle);
			__L2_MSG_FACTORY_BASE(Notification_Friends_CreatedClan);
			__L2_MSG_FACTORY_BASE(Notification_Emails_Received);
			__L2_MSG_FACTORY_BASE(Notification_Clans_GrantLeader);
			__L2_MSG_FACTORY_BASE(Notification_Clans_SetSubleaderStatus);
			__L2_MSG_FACTORY_BASE(Notification_Clans_SetMemberRank);
			__L2_MSG_FACTORY_BASE(Notification_Clans_ChangeHandle);
			__L2_MSG_FACTORY_BASE(Notification_Clans_Leave);
			__L2_MSG_FACTORY_BASE(Notification_Clans_PendingJoinStatus);
			__L2_MSG_FACTORY_BASE(Notification_Clans_NewClanMember);
			__L2_MSG_FACTORY_BASE(Notification_Clans_KickAndBlacklistUser);
			__L2_MSG_FACTORY_BASE(Notification_Clans_UnblacklistUser);
			__L2_MSG_FACTORY_BASE(Notification_Clans_Destroyed);
			__L2_MSG_FACTORY_BASE(Notification_Console_MemberJoinedLobby);
			__L2_MSG_FACTORY_BASE(Notification_Console_MemberLeftLobby);
			__L2_MSG_FACTORY_BASE(Notification_Console_LobbyDestroyed);
			__L2_MSG_FACTORY_BASE(Notification_Console_LobbyMemberDataUpdated);
			__L2_MSG_FACTORY_BASE(Notification_Console_LobbyGotChatMessage);
			__L2_MSG_FACTORY_BASE(Notification_Console_LobbyGotRoomInvitation);
			__L2_MSG_FACTORY_BASE(Notification_Console_MemberJoinedRoom);
			__L2_MSG_FACTORY_BASE(Notification_Console_MemberLeftRoom);
			__L2_MSG_FACTORY_BASE(Notification_Console_KickedOutOfRoom);
			__L2_MSG_FACTORY_BASE(Notification_Console_RoomWasDestroyed);
			__L2_MSG_FACTORY_BASE(Notification_Console_RoomOwnerChanged);
			__L2_MSG_FACTORY_BASE(Notification_Console_RoomStateChanged);
			__L2_MSG_FACTORY_BASE(Notification_Console_RoomChatMessage);
			__L2_MSG_FACTORY_BASE(Notification_Console_RoomMessage);
			__L2_MSG_FACTORY_BASE(Notification_Console_RoomMemberConnectivityUpdate);
			__L2_MSG_FACTORY_BASE(Notification_Console_ChatEvent);
			__L2_MSG_FACTORY_BASE(Notification_Console_MuteListChanged);
			__L2_MSG_FACTORY_BASE(Notification_Console_Local_Users_Changed);
			__L2_MSG_FACTORY_BASE(Notification_Console_Signaling_Result);

		default:
			return 0;
		};
	}
	void Dealloc(Lobby2Message *msg) {
		if (--msg->refCount==0)
			RakNet::OP_DELETE<Lobby2Message>(msg, __FILE__, __LINE__ );
	}

	unsigned int nextRequestId;
};

} // namespace RakNet

#endif
