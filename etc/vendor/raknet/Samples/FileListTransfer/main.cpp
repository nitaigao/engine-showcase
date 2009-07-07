#include "RakPeerInterface.h"
#include "FileListTransfer.h"
#include "RakSleep.h"
#include "RakNetworkFactory.h"
#include "MessageIdentifiers.h"
#include "FileListTransferCBInterface.h"
#include "FileOperations.h"
#include "SuperFastHash.h"
#include "RakAssert.h"
#include "BitStream.h"
#include "IncrementalReadInterface.h"
#include "PacketizedTCP.h"
#include <stdio.h>

RakNet::RakString file;
RakNet::RakString fileCopy;

//const char *file = "c:/temp/unittest.vcproj";
//const char *fileCopy = "c:/temp/unittest_copy.vcproj";

#define USE_TCP

class TestCB : public FileListTransferCBInterface
{
public:
	bool OnFile(
		OnFileStruct *onFileStruct)
	{
		printf("%i. (100%%) %i/%i %s %ib->%ib / %ib->%ib\n", onFileStruct->setID, onFileStruct->fileIndex+1, onFileStruct->setCount, onFileStruct->fileName, onFileStruct->compressedTransmissionLength, onFileStruct->finalDataLength, onFileStruct->setTotalCompressedTransmissionLength, onFileStruct->setTotalFinalLength);


		FILE *fp = fopen(fileCopy.C_String(), "wb");
		fwrite(onFileStruct->fileData, onFileStruct->finalDataLength, 1, fp);
		fclose(fp);

		// Make sure it worked
		unsigned int hash1 = SuperFastHashFile(file.C_String());
		if (RakNet::BitStream::DoEndianSwap())
			RakNet::BitStream::ReverseBytesInPlace((unsigned char*) &hash1, sizeof(hash1));
		unsigned int hash2 = SuperFastHashFile(fileCopy.C_String());
		if (RakNet::BitStream::DoEndianSwap())
			RakNet::BitStream::ReverseBytesInPlace((unsigned char*) &hash2, sizeof(hash2));
		RakAssert(hash1==hash2);

		// Return true to have RakNet delete the memory allocated to hold this file.
		// False if you hold onto the memory, and plan to delete it yourself later
		return true;
	}

	virtual void OnFileProgress(OnFileStruct *onFileStruct,unsigned int partCount,unsigned int partTotal,unsigned int partLength, char *firstDataChunk)
	{
		printf("%i (%i%%) %i/%i %s %ib->%ib / %ib->%ib\n", onFileStruct->setID, (int) (100.0*(double)partCount/(double)partTotal), onFileStruct->fileIndex+1, onFileStruct->setCount, onFileStruct->fileName, onFileStruct->compressedTransmissionLength, onFileStruct->finalDataLength, onFileStruct->setTotalCompressedTransmissionLength, onFileStruct->setTotalFinalLength, firstDataChunk);
	}

	virtual bool OnDownloadComplete(void)
	{
		printf("Download complete.\n");

		// Returning false automatically deallocates the automatically allocated handler that was created by DirectoryDeltaTransfer
		return false;
	}

} transferCallback;

// Sender progress notification
class TestFileListProgress : public FileListProgress
{
	virtual void OnFilePush(const char *fileName, unsigned int fileLengthBytes, unsigned int offset, unsigned int bytesBeingSent, bool done, SystemAddress targetSystem)
	{
		printf("Sending %s bytes=%i offset=%i\n", fileName, bytesBeingSent, offset);
	}
} testFileListProgress;

void main(void)
{
	printf("This sample demonstrates incrementally sending a file with\n");
	printf("the FileListTransferPlugin. Incremental sends will read and send the.\n");
	printf("file only as needed, rather than putting the whole file in memory.\n");
	printf("This is to support sending large files to many users at the same time.\n");
	printf("Difficulty: Intermediate\n\n");

	TestCB testCB;
	FileListTransfer flt1, flt2;
#ifdef USE_TCP
	PacketizedTCP tcp1, tcp2;
	tcp1.Start(60000,1);
	tcp2.Start(60001,1);
	tcp2.Connect("127.0.0.1",60000,false);
	tcp1.AttachPlugin(&flt1);
	tcp2.AttachPlugin(&flt2);
#else
	RakPeerInterface *peer1 = RakNetworkFactory::GetRakPeerInterface();
	RakPeerInterface *peer2 = RakNetworkFactory::GetRakPeerInterface();
	peer1->Startup(1,0,&SocketDescriptor(60000,0),1);
	peer2->Startup(1,0,&SocketDescriptor(60001,0),1);
	peer1->SetMaximumIncomingConnections(1);
	peer2->Connect("127.0.0.1",60000,0,0,0);
	peer1->AttachPlugin(&flt1);
	peer2->AttachPlugin(&flt2);
	peer1->SetSplitMessageProgressInterval(9);
	peer2->SetSplitMessageProgressInterval(9);
#endif
	// Print sending progress notifications
	flt1.SetCallback(&testFileListProgress);
	FileList fileList;
	IncrementalReadInterface incrementalReadInterface;
	printf("Enter complete filename with path to test:\n");
	char str[256];
	gets(str);
	if (str[0]==0)
		strcpy(str, "c:\\temp\\wow.jpg");
	file=str;
	fileCopy=file+"_copy";
	// Reference this file, rather than add it in memory. Will send 1000 byte chunks. The reason to do this is so the whole file does not have to be in memory at once
	unsigned int fileLength = GetFileLength(file.C_String());
	if (fileLength==0)
	{
		printf("Test file %s not found.\n", file.C_String());

#ifdef USE_TCP
#else
		RakNetworkFactory::DestroyRakPeerInterface(peer1);
		RakNetworkFactory::DestroyRakPeerInterface(peer1);
#endif
		return;
	}
	fileList.AddFile(file.C_String(), file.C_String(), 0, fileLength, fileLength, FileListNodeContext(0,0), true);
	// Wait for the connection
	RakSleep(100);
	Packet *packet1, *packet2;
	while (1)
	{
#ifdef USE_TCP
		SystemAddress sa;
		sa=tcp2.HasCompletedConnectionAttempt();
		if (sa!=UNASSIGNED_SYSTEM_ADDRESS)
		{
			flt2.SetupReceive(&testCB, false, sa);
			break;
		}
#else
		// Wait for the connection request to be accepted
		packet2=peer2->Receive();
		if (packet2->data[0]==ID_CONNECTION_REQUEST_ACCEPTED)
		{
			flt2.SetupReceive(&testCB, false, packet2->systemAddress);
			peer2->DeallocatePacket(packet2);
			break;
		}
		peer2->DeallocatePacket(packet2);
#endif
		RakSleep(30);
	}

	// When connected, send the file. Since the file is a reference, it will be sent incrementally
	while (1)
	{
#ifdef USE_TCP
		packet1=tcp1.Receive();
		packet2=tcp2.Receive();
		SystemAddress sa;
		sa = tcp1.HasNewIncomingConnection();
		if (sa!=UNASSIGNED_SYSTEM_ADDRESS)
			flt1.Send(&fileList,0,sa,0,HIGH_PRIORITY,0,false, &incrementalReadInterface, 50000);
		tcp1.DeallocatePacket(packet1);
		tcp2.DeallocatePacket(packet2);
#else
		packet1=peer1->Receive();
		packet2=peer2->Receive();
		if (packet1 && packet1->data[0]==ID_NEW_INCOMING_CONNECTION)
			flt1.Send(&fileList,peer1,packet1->systemAddress,0,HIGH_PRIORITY,0,false, &incrementalReadInterface, 50000);
		peer1->DeallocatePacket(packet1);
		peer2->DeallocatePacket(packet2);

#endif
		RakSleep(30);
	}
	
#ifdef USE_TCP
#else
	RakNetworkFactory::DestroyRakPeerInterface(peer1);
	RakNetworkFactory::DestroyRakPeerInterface(peer1);
#endif
}