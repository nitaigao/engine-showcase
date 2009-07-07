#include "SQLite3Plugin.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"

using namespace RakNet;

bool operator<( const DataStructures::MLKeyRef<RakNet::RakString> &inputKey, const SQLite3Plugin::NamedDBHandle &cls ) {return inputKey.Get() < cls.dbIdentifier;}
bool operator>( const DataStructures::MLKeyRef<RakNet::RakString> &inputKey, const SQLite3Plugin::NamedDBHandle &cls ) {return inputKey.Get() > cls.dbIdentifier;}
bool operator==( const DataStructures::MLKeyRef<RakNet::RakString> &inputKey, const SQLite3Plugin::NamedDBHandle &cls ) {return inputKey.Get() == cls.dbIdentifier;}

int PerRowCallback(void *userArgument, int argc, char **argv, char **azColName)
{
	SQLite3Table *outputTable = (SQLite3Table*)userArgument;
	DataStructures::DefaultIndexType idx;
	if (outputTable->columnNames.GetSize()==0)
	{
		for (idx=0; idx < (DataStructures::DefaultIndexType) argc; idx++)
			outputTable->columnNames.Push(azColName[idx]);
	}
	SQLite3Row *row = RakNet::OP_NEW<SQLite3Row>(__FILE__,__LINE__);
	outputTable->rows.Push(row,__FILE__,__LINE__);
	for (idx=0; idx < (DataStructures::DefaultIndexType) argc; idx++)
		row->entries.Push(argv[idx]);
	return 0;
}
void SQLite3PluginResultInterface_Printf::_sqlite3_exec(
	RakNet::RakString inputStatement,
	unsigned int queryId,
	RakNet::RakString dbIdentifier,
	const SQLite3Table &table,
	RakNet::RakString errorMsg)
{

	if (errorMsg.IsEmpty()==false)
	{
		printf("Error for query: %s\n", inputStatement.C_String());
		printf("%s\n", errorMsg.C_String());
		return;
	}

	DataStructures::DefaultIndexType idx;
	for (idx=0; idx < table.columnNames.GetSize(); idx++)
		printf("%s ", table.columnNames[idx].C_String());
	printf("\n");
	if (table.rows.GetSize()==0)
	{
		printf("<NO ROWS>\n");
	}
	else
	{
		for (idx=0; idx < table.rows.GetSize(); idx++)
		{
			DataStructures::DefaultIndexType idx2;
			for (idx2=0; idx2 < table.rows[idx]->entries.GetSize(); idx2++)
				printf("%s ", table.rows[idx]->entries[idx2].C_String());
			printf("\n");
		}
	}
}
void SQLite3PluginResultInterface_Printf::OnUnknownDBIdentifier(
								   RakNet::RakString inputStatement,
								   unsigned int queryId,
								   RakNet::RakString dbIdentifier)
{
	printf("Unknown DB %s\n", dbIdentifier.C_String());
}

SQLite3Table::SQLite3Table()
{

}
SQLite3Table::~SQLite3Table()
{
	rows.ClearPointers(true,__FILE__,__LINE__);
}
void SQLite3Table::Serialize(RakNet::BitStream *bitStream)
{
	bitStream->Write(columnNames.GetSize());
	DataStructures::DefaultIndexType idx1, idx2;
	for (idx1=0; idx1 < columnNames.GetSize(); idx1++)
		bitStream->Write(columnNames[idx1]);
	bitStream->Write(rows.GetSize());
	for (idx1=0; idx1 < rows.GetSize(); idx1++)
	{
		for (idx2=0; idx2 < rows[idx1]->entries.GetSize(); idx2++)
		{
			bitStream->Write(rows[idx1]->entries[idx2]);
		}
	}
}
void SQLite3Table::Deserialize(RakNet::BitStream *bitStream)
{
	rows.ClearPointers(true,__FILE__,__LINE__);
	columnNames.Clear();

	DataStructures::DefaultIndexType numColumns, numRows;
	bitStream->Read(numColumns);
	DataStructures::DefaultIndexType idx1,idx2;
	RakNet::RakString inputStr;
	for (idx1=0; idx1 < numColumns; idx1++)
	{
		bitStream->Read(inputStr);
		columnNames.Push(inputStr);
	}
	bitStream->Read(numRows);
	for (idx1=0; idx1 < numRows; idx1++)
	{
		SQLite3Row *row = RakNet::OP_NEW<SQLite3Row>(__FILE__,__LINE__);
		rows.Push(row,__FILE__,__LINE__);
		for (idx2=0; idx2 < numColumns; idx2++)
		{
			bitStream->Read(inputStr);
			row->entries.Push(inputStr);
		}
	}
}
SQLite3Plugin::SQLite3Plugin()
{
	nextQueryId=0;
}
SQLite3Plugin::~SQLite3Plugin()
{
	StopThreads();
}
bool SQLite3Plugin::AddDBHandle(RakNet::RakString dbIdentifier, sqlite3 *dbHandle)
{
	if (dbIdentifier.IsEmpty())
		return false;
	DataStructures::DefaultIndexType idx = dbHandles.GetInsertionIndex(dbIdentifier);
	if (idx==(DataStructures::DefaultIndexType)-1)
		return false;
	NamedDBHandle ndbh;
	ndbh.dbHandle=dbHandle;
	ndbh.dbIdentifier=dbIdentifier;
	dbHandles.InsertAtIndex(ndbh,idx,__FILE__,__LINE__);
	
#ifdef SQLITE3_STATEMENT_EXECUTE_THREADED
	if (threadPool.WasStarted()==false)
		threadPool.StartThreads(1,0);
#endif

	return true;
}
void SQLite3Plugin::RemoveDBHandle(RakNet::RakString dbIdentifier)
{
	DataStructures::DefaultIndexType idx = dbHandles.GetIndexOf(dbIdentifier);
	if (idx!=(DataStructures::DefaultIndexType)-1)
		dbHandles.RemoveAtIndex(idx,__FILE__,__LINE__);
#ifdef SQLITE3_STATEMENT_EXECUTE_THREADED
	if (dbHandles.GetSize()==0)
		StopThreads();
#endif // SQLITE3_STATEMENT_EXECUTE_THREADED
}
void SQLite3Plugin::RemoveDBHandle(sqlite3 *dbHandle)
{
	DataStructures::DefaultIndexType idx;
	for (idx=0; idx < dbHandles.GetSize(); idx++)
	{
		if (dbHandles[idx].dbHandle==dbHandle)
		{
			dbHandles.RemoveAtIndex(idx,__FILE__,__LINE__);
#ifdef SQLITE3_STATEMENT_EXECUTE_THREADED
			if (dbHandles.GetSize()==0)
				StopThreads();
#endif // SQLITE3_STATEMENT_EXECUTE_THREADED
			return;
		}
	}
}
void SQLite3Plugin::AddResultHandler(SQLite3PluginResultInterface *res)
{
	resultHandlers.Push(res,__FILE__,__LINE__);
}
void SQLite3Plugin::RemoveResultHandler(SQLite3PluginResultInterface *res)
{
	DataStructures::DefaultIndexType idx = resultHandlers.GetIndexOf(res);
	if (idx!=-1)
		resultHandlers.RemoveAtIndex(idx,__FILE__,__LINE__);
}
void SQLite3Plugin::ClearResultHandlers(void)
{
	resultHandlers.Clear(true,__FILE__,__LINE__);
}
unsigned int SQLite3Plugin::_sqlite3_exec(RakNet::RakString dbIdentifier, RakNet::RakString inputStatement,
										  PacketPriority priority, PacketReliability reliability, char orderingChannel, SystemAddress systemAddress)
{
	RakNet::BitStream bsOut;
	bsOut.Write((MessageID)ID_SQLLITE3_EXEC);
	bsOut.Write(nextQueryId);
	bsOut.Write(dbIdentifier);
	bsOut.Write(inputStatement);
	bsOut.Write(true);
	SendUnified(&bsOut, priority,reliability,orderingChannel,systemAddress,false);
	++nextQueryId;
	return nextQueryId-1;
}

#ifdef SQLITE3_STATEMENT_EXECUTE_THREADED
void SQLite3Plugin::Update(void)
{
	ExecThreadOutput output;
	while (threadPool.HasOutputFast() && threadPool.HasOutput())
	{
		output = threadPool.GetOutput();
		RakNet::BitStream bsOut((unsigned char*) output.data, output.length,false);
		SendUnified(&bsOut, MEDIUM_PRIORITY,RELIABLE_ORDERED,0,output.sender,false);
		rakFree_Ex(output.data,__FILE__,__LINE__);
	}
}
SQLite3Plugin::ExecThreadOutput ExecStatementThread(SQLite3Plugin::ExecThreadInput threadInput, bool *returnOutput, void* perThreadData)
{
	unsigned int queryId;
	RakNet::RakString dbIdentifier;
	RakNet::RakString inputStatement;
	RakNet::BitStream bsIn((unsigned char*) threadInput.data, threadInput.length, false);
	bsIn.IgnoreBytes(sizeof(MessageID));
	bsIn.Read(queryId);
	bsIn.Read(dbIdentifier);
	bsIn.Read(inputStatement);
	// bool isRequest;
	// bsIn.Read(isRequest);
	bsIn.IgnoreBits(1);

	char *errorMsg;
	RakNet::RakString errorMsgStr;
	SQLite3Table outputTable;					
	sqlite3_exec(threadInput.dbHandle, inputStatement.C_String(), PerRowCallback, &outputTable, &errorMsg);
	if (errorMsg)
	{
		errorMsgStr=errorMsg;
		sqlite3_free(errorMsg);
	}

	RakNet::BitStream bsOut;
	bsOut.Write((MessageID)ID_SQLLITE3_EXEC);
	bsOut.Write(queryId);
	bsOut.Write(dbIdentifier);
	bsOut.Write(inputStatement);
	bsOut.Write(false);
	bsOut.Write(errorMsgStr);
	outputTable.Serialize(&bsOut);

	// Free input data
	rakFree_Ex(threadInput.data,__FILE__,__LINE__);

	// Copy to output data
	SQLite3Plugin::ExecThreadOutput threadOutput;
	threadOutput.data=(char*) rakMalloc_Ex(bsOut.GetNumberOfBytesUsed(),__FILE__,__LINE__);
	memcpy(threadOutput.data,bsOut.GetData(),bsOut.GetNumberOfBytesUsed());
	threadOutput.length=bsOut.GetNumberOfBytesUsed();
	threadOutput.sender=threadInput.sender;	
	// SendUnified(&bsOut, MEDIUM_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
	return threadOutput;
}
#endif // SQLITE3_STATEMENT_EXECUTE_THREADED

PluginReceiveResult SQLite3Plugin::OnReceive(Packet *packet)
{
	switch (packet->data[0])
	{
	case ID_SQLLITE3_EXEC:
		{
			unsigned int queryId;
			RakNet::RakString dbIdentifier;
			RakNet::RakString inputStatement;
			RakNet::BitStream bsIn(packet->data, packet->length, false);
			bsIn.IgnoreBytes(sizeof(MessageID));
			bsIn.Read(queryId);
			bsIn.Read(dbIdentifier);
			bsIn.Read(inputStatement);
			bool isRequest;
			bsIn.Read(isRequest);
			if (isRequest)
			{
				DataStructures::DefaultIndexType idx = dbHandles.GetIndexOf(dbIdentifier);
				if (idx==-1)
				{
					RakNet::BitStream bsOut;
					bsOut.Write((MessageID)ID_SQLLITE3_UNKNOWN_DB);
					bsOut.Write(queryId);
					bsOut.Write(dbIdentifier);
					bsOut.Write(inputStatement);
					SendUnified(&bsOut, MEDIUM_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
				}
				else
				{
#ifdef SQLITE3_STATEMENT_EXECUTE_THREADED
					// Push to the thread
					ExecThreadInput input;
					input.data=(char*) rakMalloc_Ex(packet->length, __FILE__,__LINE__);
					memcpy(input.data,packet->data,packet->length);
					input.dbHandle=dbHandles[idx].dbHandle;
					input.length=packet->length;
					input.sender=packet->systemAddress;
					threadPool.AddInput(ExecStatementThread, input);
#else
					char *errorMsg;
					RakNet::RakString errorMsgStr;
					SQLite3Table outputTable;					
					sqlite3_exec(dbHandles[idx].dbHandle, inputStatement.C_String(), PerRowCallback, &outputTable, &errorMsg);
					if (errorMsg)
					{
						errorMsgStr=errorMsg;
						sqlite3_free(errorMsg);
					}
					RakNet::BitStream bsOut;
					bsOut.Write((MessageID)ID_SQLLITE3_EXEC);
					bsOut.Write(queryId);
					bsOut.Write(dbIdentifier);
					bsOut.Write(inputStatement);
					bsOut.Write(false);
					bsOut.Write(errorMsgStr);
					outputTable.Serialize(&bsOut);
					SendUnified(&bsOut, MEDIUM_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
#endif
				}
			}
			else
			{
				RakNet::RakString errorMsgStr;
				SQLite3Table inputTable;		

				// Read it
				bsIn.Read(errorMsgStr);
				inputTable.Deserialize(&bsIn);

				DataStructures::DefaultIndexType idx;
				for (idx=0; idx < resultHandlers.GetSize(); idx++)
					resultHandlers[idx]->_sqlite3_exec(inputStatement, queryId, dbIdentifier, inputTable,errorMsgStr);
			}

			return RR_STOP_PROCESSING_AND_DEALLOCATE;
		}
		break;
	case ID_SQLLITE3_UNKNOWN_DB:
		{
			unsigned int queryId;
			RakNet::RakString dbIdentifier;
			RakNet::RakString inputStatement;
			RakNet::BitStream bsIn(packet->data, packet->length, false);
			bsIn.IgnoreBytes(sizeof(MessageID));
			bsIn.Read(queryId);
			bsIn.Read(dbIdentifier);
			bsIn.Read(inputStatement);
			DataStructures::DefaultIndexType idx;
			for (idx=0; idx < resultHandlers.GetSize(); idx++)
				resultHandlers[idx]->OnUnknownDBIdentifier(inputStatement, queryId, dbIdentifier);
		}
	}

	return RR_CONTINUE_PROCESSING;
}

void SQLite3Plugin::OnAttach(void)
{
}
void SQLite3Plugin::OnDetach(void)
{
	StopThreads();
}
void SQLite3Plugin::StopThreads(void)
{
#ifdef SQLITE3_STATEMENT_EXECUTE_THREADED
	threadPool.StopThreads();
	unsigned int i;
	for (i=0; i < threadPool.InputSize(); i++)
	{
		RakNet::OP_DELETE(threadPool.GetInputAtIndex(i).data, __FILE__, __LINE__);
	}
	threadPool.ClearInput();
	for (i=0; i < threadPool.OutputSize(); i++)
	{
		RakNet::OP_DELETE(threadPool.GetOutputAtIndex(i).data, __FILE__, __LINE__);
	}
	threadPool.ClearOutput();
#endif
}
