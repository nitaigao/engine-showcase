/// \file
/// \brief Contains code to call sqlite3_exec over a network that does not support shared file handles.
///
/// This file is part of RakNet Copyright 2003 Jenkins Software LLC
///
/// Usage of RakNet is subject to the appropriate license agreement.


#ifndef ___SQLITE_3_PLUGIN_H
#define ___SQLITE_3_PLUGIN_H

// sqlite3_exec is blocking and will therefore block other operations in the same program
// If defined, sqlite3_exec executes in a thread so that doesn't happen
// If the only thing this system is doing is running SQLite, then you'll get marginally better performance by commenting it out.
#define SQLITE3_STATEMENT_EXECUTE_THREADED

#include "RakNetTypes.h"
#include "Export.h"
#include "PluginInterface2.h"
#include "PacketPriority.h"
#include "SocketIncludes.h"
#include "DS_Multilist.h"
#include "RakString.h"
#include "sqlite3.h"

#ifdef SQLITE3_STATEMENT_EXECUTE_THREADED
#include "ThreadPool.h"
#endif

class RakPeerInterface;

namespace RakNet
{

/// Contains a result row, which is just an array of strings
struct SQLite3Row
{
	DataStructures::Multilist<ML_STACK, RakNet::RakString> entries;
};

/// Contains a result table, which is an array of column name strings, followed by an array of SQLite3Row
struct SQLite3Table
{
	SQLite3Table();
	~SQLite3Table();
	void Serialize(RakNet::BitStream *bitStream);
	void Deserialize(RakNet::BitStream *bitStream);

	DataStructures::Multilist<ML_STACK, RakNet::RakString> columnNames;
	DataStructures::Multilist<ML_STACK, SQLite3Row*> rows;
};

/// \brief Handles results of calls to SQLite3Plugin::_sqlite3_exec()
/// Results from calling SQLite3Plugin::_sqlite3_exec() are handled in this callback.
/// You should implement the callback, and let the plugin know about it via SQLite3Plugin::AddResultHandler()
/// Be sure to call SQLite3Plugin::RemoveResultHandler() or SQLite3Plugin::ClearResultHandlers() if you delete the callback
class SQLite3PluginResultInterface
{
public:

	/// Query executed, possibly returning data or an error message
	///
	/// \param[out] inputStatement Passed to SQLite3Plugin::_sqlite3_exec
	/// \param[out] queryId Returned from SQLite3Plugin::_sqlite3_exec
	/// \param[out] dbIdentifier Passed to SQLite3Plugin::_sqlite3_exec
	/// \param[out] table Result of call to _sqlite3_exec, should that statement return a result
	/// \param[out] errorMsg If _sqlite3_exec failed, then the error message is here, and table will be empty
	virtual void _sqlite3_exec(
		RakNet::RakString inputStatement,
		unsigned int queryId,
		RakNet::RakString dbIdentifier,
		const SQLite3Table &table,
		RakNet::RakString errorMsg)=0;

	/// dbIdentifier is unknown on the remote system
	///
	/// \param[out] inputStatement Passed to SQLite3Plugin::_sqlite3_exec
	/// \param[out] queryId Returned from SQLite3Plugin::_sqlite3_exec
	/// \param[out] dbIdentifier Passed to SQLite3Plugin::_sqlite3_exec
	virtual void OnUnknownDBIdentifier(
		RakNet::RakString inputStatement,
		unsigned int queryId,
		RakNet::RakString dbIdentifier)=0;
};

/// Sample callback implementation that just prints to the screen the results
class SQLite3PluginResultInterface_Printf : public SQLite3PluginResultInterface
{
	virtual void _sqlite3_exec(
		RakNet::RakString inputStatement,
		unsigned int queryId,
		RakNet::RakString dbIdentifier,
		const SQLite3Table &table,
		RakNet::RakString errorMsg);

	virtual void OnUnknownDBIdentifier(
		RakNet::RakString inputStatement,
		unsigned int queryId,
		RakNet::RakString dbIdentifier);
};

/// SQLite version 3 supports remote calls via networked file handles, but not over the regular internet
/// This plugin will serialize calls to and results from sqlite3_exec
/// That's all it does - any remote system can execute SQL queries.
/// Intended as a starting platform to derive from for more advanced functionality (security over who can query, etc).
/// Compatible as a plugin with both RakPeerInterface and PacketizedTCP
class RAK_DLL_EXPORT SQLite3Plugin : public PluginInterface2
{
public:
	SQLite3Plugin();
	~SQLite3Plugin();

	/// Associate identifier with dbHandle, so when we get calls to operate on identifier, we use dbhandle
	/// If SQLITE3_STATEMENT_EXECUTE_THREADED is defined, will start the execution thread the first time a dbHandle is added.
	/// \return true on success, false on dbIdentifier empty, or already in use
	bool AddDBHandle(RakNet::RakString dbIdentifier, sqlite3 *dbHandle);

	/// Stop using a dbHandle, lookup either by identifier or by pointer.
	/// If SQLITE3_STATEMENT_EXECUTE_THREADED is defined, do not call this while processing commands, since the commands run in a thread and might be using the dbHandle
	/// Call before closing the handle or else SQLite3Plugin won't know that it was closed, and will continue using it
	void RemoveDBHandle(RakNet::RakString dbIdentifier);
	void RemoveDBHandle(sqlite3 *dbHandle);

	/// Add an interface to get callbacks for results
	/// Up to user to make sure the pointer is valid through the lifetime of use
	void AddResultHandler(SQLite3PluginResultInterface *res);
	void RemoveResultHandler(SQLite3PluginResultInterface *res);
	void ClearResultHandlers(void);

	/// Execute a statement on the remote system
	/// \note Don't forget to escape your input strings. RakString::SQLEscape() is available for this.
	/// \param[in] dbIdentifier Which database to use, added with AddDBHandle()
	/// \param[in] inputStatement SQL statement to execute
	/// \param[in] priority See RakPeerInterface::Send()
	/// \param[in] reliability See RakPeerInterface::Send()
	/// \param[in] orderingChannel See RakPeerInterface::Send()
	/// \param[in] systemAddress See RakPeerInterface::Send()
	/// \return Query ID. Will be returned in _sqlite3_exec
	unsigned int _sqlite3_exec(RakNet::RakString dbIdentifier, RakNet::RakString inputStatement,
		PacketPriority priority, PacketReliability reliability, char orderingChannel, SystemAddress systemAddress);

	/// \internal For plugin handling
	virtual PluginReceiveResult OnReceive(Packet *packet);
	virtual void OnAttach(void);
	virtual void OnDetach(void);

	/// \internal
	struct NamedDBHandle
	{
		RakNet::RakString dbIdentifier;
		sqlite3 *dbHandle;
	};

#ifdef SQLITE3_STATEMENT_EXECUTE_THREADED
	virtual void Update(void);
	/// \internal
	struct ExecThreadInput
	{
		char *data;
		unsigned int length;
		SystemAddress sender;
		sqlite3 *dbHandle;
	};

	/// \internal
	struct ExecThreadOutput
	{
		char *data;
		unsigned int length;
		SystemAddress sender;
	};
#endif // SQLITE3_STATEMENT_EXECUTE_THREADED

protected:
	void StopThreads(void);

	// List of databases added with AddDBHandle()
	DataStructures::Multilist<ML_ORDERED_LIST, NamedDBHandle, RakNet::RakString> dbHandles;
	// List of result handlers added with AddResultHandler()
	DataStructures::Multilist<ML_STACK, SQLite3PluginResultInterface *> resultHandlers;
	// Each query returns a numeric id if you want it. This tracks what id to assign next. Increments sequentially.
	unsigned int nextQueryId;

#ifdef SQLITE3_STATEMENT_EXECUTE_THREADED
	// The point of the threadPool is so that SQL queries, which are blocking, happen in the thread and don't slow down the rest of the application
	// The threadPool has a queue for incoming processing requests.  As systems disconnect their pending requests are removed from the list.
	ThreadPool<ExecThreadInput, ExecThreadOutput> threadPool;
#endif
};

};

extern bool operator<( const DataStructures::MLKeyRef<RakNet::RakString> &inputKey, const RakNet::SQLite3Plugin::NamedDBHandle &cls );
extern bool operator>( const DataStructures::MLKeyRef<RakNet::RakString> &inputKey, const RakNet::SQLite3Plugin::NamedDBHandle &cls );
extern bool operator==( const DataStructures::MLKeyRef<RakNet::RakString> &inputKey, const RakNet::SQLite3Plugin::NamedDBHandle &cls );

#endif
