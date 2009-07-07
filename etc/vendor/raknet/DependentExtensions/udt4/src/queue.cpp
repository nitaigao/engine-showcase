#include "RakNetDefines.h"
#ifndef _USE_RAKNET_FLOW_CONTROL

#ifdef _MSC_VER
#pragma warning( push )
#endif

#pragma warning(disable:4127)   // conditional expression is constant
#pragma warning(disable:4702)   // unreachable code


/*****************************************************************************
Copyright (c) 2001 - 2008, The Board of Trustees of the University of Illinois.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the
  above copyright notice, this list of conditions
  and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the University of Illinois
  nor the names of its contributors may be used to
  endorse or promote products derived from this
  software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

/*****************************************************************************
written by
   Yunhong Gu, last updated 12/08/2008
*****************************************************************************/

#ifdef WIN32
   #include <winsock2.h>
   #include <ws2tcpip.h>
   #include <wspiapi.h>
#endif

#include <cstring>
#include "common.h"
#include "queue.h"
#include "core.h"

using namespace std;

CUnitQueue::CUnitQueue():
m_pQEntry(NULL),
m_pCurrQueue(NULL),
m_pLastQueue(NULL),
m_iSize(0),
m_iCount(0)
{
}

CUnitQueue::~CUnitQueue()
{
   CQEntry* p = m_pQEntry;

   while (p != NULL)
   {
      //delete [] p->m_pUnit;
	   RakNet::OP_DELETE_ARRAY(p->m_pUnit,__FILE__, __LINE__);
      //delete [] p->m_pBuffer;
	  rakFree_Ex(p->m_pBuffer,__FILE__, __LINE__);

      CQEntry* q = p;
      if (p == m_pLastQueue)
         p = NULL;
      else
         p = p->m_pNext;
     // delete q;
	  RakNet::OP_DELETE(q,__FILE__,__LINE__);
   }
}

int CUnitQueue::init(const int& size, const int& mss, const int& version)
{
   CQEntry* tempq = NULL;
   CUnit* tempu = NULL;
   char* tempb = NULL;

   TRY_CUDTEXCEPTION
   {
      //tempq = new CQEntry;
      //tempu = new CUnit [size];
      //tempb = new char [size * mss];
	  tempq = RakNet::OP_NEW<CQEntry>(__FILE__, __LINE__);
	  tempu = RakNet::OP_NEW_ARRAY<CUnit>(size, __FILE__, __LINE__ );
	  tempb = (char*) rakMalloc_Ex(size * mss, __FILE__, __LINE__);
	  if (tempq==0 || tempu==0 || tempb==0)
	  {
		  //delete tempq;
		  //delete [] tempu;
		  //delete [] tempb;
			RakNet::OP_DELETE(tempq,__FILE__,__LINE__);
			RakNet::OP_DELETE_ARRAY(tempu,__FILE__, __LINE__);
			rakFree_Ex(tempb,__FILE__, __LINE__);

		  return -1;
	  }
   }
   CATCH_UDT_ELLIPSES
   {
	   //delete tempq;
	   //delete [] tempu;
	   //delete [] tempb;
	   RakNet::OP_DELETE(tempq,__FILE__,__LINE__);
	   RakNet::OP_DELETE_ARRAY(tempu,__FILE__, __LINE__);
	   rakFree_Ex(tempb,__FILE__, __LINE__);

      return -1;
   }

   for (int i = 0; i < size; ++ i)
   {
      tempu[i].m_iFlag = 0;
      tempu[i].m_Packet.m_pcData = tempb + i * mss;
   }
   tempq->m_pUnit = tempu;
   tempq->m_pBuffer = tempb;
   tempq->m_iSize = size;

   m_pQEntry = m_pCurrQueue = m_pLastQueue = tempq;
   m_pQEntry->m_pNext = m_pQEntry;

   m_pAvailUnit = m_pCurrQueue->m_pUnit;

   m_iSize = size;
   m_iMSS = mss;
   m_iIPversion = version;

   return 0;
}

int CUnitQueue::increase()
{
   // adjust/correct m_iCount
   int real_count = 0;
   CQEntry* p = m_pQEntry;
   while (p != NULL)
   {
      CUnit* u = p->m_pUnit;
      for (CUnit* end = u + p->m_iSize; u != end; ++ u)
         if (u->m_iFlag != 0)
            ++ real_count;

      if (p == m_pLastQueue)
         p = NULL;
      else
         p = p->m_pNext;
   }
   m_iCount = real_count;
   if (double(m_iCount) / m_iSize < 0.9)
      return -1;

   CQEntry* tempq = NULL;
   CUnit* tempu = NULL;
   char* tempb = NULL;

   // all queues have the same size
   int size = m_pQEntry->m_iSize;

   TRY_CUDTEXCEPTION
   {
      //tempq = new CQEntry;
      //tempu = new CUnit [size];
      //tempb = new char [size * m_iMSS];
	   tempq = RakNet::OP_NEW<CQEntry>(__FILE__, __LINE__);
	   tempu = RakNet::OP_NEW_ARRAY<CUnit>(size, __FILE__, __LINE__ );
	   tempb = (char*) rakMalloc_Ex(size * m_iMSS, __FILE__, __LINE__);

	  if (tempq==0 || tempu==0 || tempb==0)
	  {
		 // delete tempq;
		  //delete [] tempu;
		  //delete [] tempb;
		  RakNet::OP_DELETE(tempq,__FILE__,__LINE__);
		  RakNet::OP_DELETE_ARRAY(tempu,__FILE__, __LINE__);
		  rakFree_Ex(tempb,__FILE__, __LINE__);

		  return -1;
	  }
   }
   CATCH_UDT_ELLIPSES
   {
	   // delete tempq;
	   //delete [] tempu;
	   //delete [] tempb;
	   RakNet::OP_DELETE(tempq,__FILE__,__LINE__);
	   RakNet::OP_DELETE_ARRAY(tempu,__FILE__, __LINE__);
	   rakFree_Ex(tempb,__FILE__, __LINE__);

      return -1;
   }

   for (int i = 0; i < size; ++ i)
   {
      tempu[i].m_iFlag = 0;
      tempu[i].m_Packet.m_pcData = tempb + i * m_iMSS;
   }
   tempq->m_pUnit = tempu;
   tempq->m_pBuffer = tempb;
   tempq->m_iSize = size;

   m_pLastQueue->m_pNext = tempq;
   m_pLastQueue = tempq;
   m_pLastQueue->m_pNext = m_pQEntry;

   m_iSize += size;

   return 0;
}

int CUnitQueue::shrink()
{
   // currently queue cannot be shrunk.
   return -1;
}

CUnit* CUnitQueue::getNextAvailUnit()
{
   if (m_iCount * 10 > m_iSize * 9)
   {
      if (increase()==-1)
		  return NULL;
   }

   if (m_iCount >= m_iSize)
      return NULL;

   CQEntry* entrance = m_pCurrQueue;

   do
   {
      for (CUnit* sentinel = m_pCurrQueue->m_pUnit + m_pCurrQueue->m_iSize - 1; m_pAvailUnit != sentinel; ++ m_pAvailUnit)
         if (m_pAvailUnit->m_iFlag == 0)
            return m_pAvailUnit;

      if (m_pCurrQueue->m_pUnit->m_iFlag == 0)
      {
         m_pAvailUnit = m_pCurrQueue->m_pUnit;
         return m_pAvailUnit;
      }

      m_pCurrQueue = m_pCurrQueue->m_pNext;
      m_pAvailUnit = m_pCurrQueue->m_pUnit;
   } while (m_pCurrQueue != entrance);

   increase();

   return NULL;
}


CSndUList::CSndUList()
{
   m_iArrayLength = 4096;
   //m_pHeap = new CSNode*[m_iArrayLength];
   m_pHeap = RakNet::OP_NEW_ARRAY<CSNode*>(m_iArrayLength, __FILE__, __LINE__ );
   m_iLastEntry = -1;

   #ifndef WIN32
      pthread_mutex_init(&m_ListLock, NULL);
   #else
      m_ListLock = CreateMutex(NULL, false, NULL);
   #endif
}

CSndUList::~CSndUList()
{
   //delete [] m_pHeap;
	RakNet::OP_DELETE_ARRAY(m_pHeap,__FILE__, __LINE__);

   #ifndef WIN32
      pthread_mutex_destroy(&m_ListLock);
   #else
      CloseHandle(m_ListLock);
   #endif
}

void CSndUList::insert(const int64_t& ts, const CUDT* u)
{
   CGuard listguard(m_ListLock);

   // increase the heap array size if necessary
   if (m_iLastEntry == m_iArrayLength - 1)
   {
      CSNode** temp = NULL;

      TRY_CUDTEXCEPTION
      {
         //temp = new CSNode*[m_iArrayLength * 2];
		  temp = RakNet::OP_NEW_ARRAY<CSNode*>(m_iArrayLength * 2, __FILE__, __LINE__ );
		 if (temp==0)
			 return;
      }
      CATCH_UDT_ELLIPSES
      {
         return;
      }

      memcpy(temp, m_pHeap, sizeof(CSNode*) * m_iArrayLength);
      m_iArrayLength *= 2;
      //delete [] m_pHeap;
	  RakNet::OP_DELETE_ARRAY(m_pHeap,__FILE__, __LINE__);
      m_pHeap = temp;
   }

   insert_(ts, u);
}

void CSndUList::update(const CUDT* u, const bool& reschedule)
{
   CGuard listguard(m_ListLock);

   CSNode* n = u->m_pSNode;

   if (n->m_iHeapLoc >= 0)
   {
      if (!reschedule)
         return;

      if (n->m_iHeapLoc == 0)
      {
         n->m_llTimeStamp = 1;
         m_pTimer->interrupt();
         return;
      }

      remove_(u);
   }

   insert_(1, u);
}

int CSndUList::pop(sockaddr*& addr, CPacket& pkt)
{
   CGuard listguard(m_ListLock);

   if (-1 == m_iLastEntry)
      return -1;

   CUDT* u = m_pHeap[0]->m_pUDT;
   remove_(u);

   if (!u->m_bConnected || u->m_bBroken)
      return -1;

   // pack a packet from the socket
   uint64_t ts;
   if (u->packData(pkt, ts) <= 0)
      return -1;

   addr = u->m_pPeerAddr;

   // insert a new entry, ts is the next processing time
   if (ts > 0)
      insert_(ts, u);

   return 1;
}

void CSndUList::remove(const CUDT* u)
{
   CGuard listguard(m_ListLock);

   remove_(u);
}

uint64_t CSndUList::getNextProcTime()
{
   CGuard listguard(m_ListLock);

   if (-1 == m_iLastEntry)
      return 0;

   return m_pHeap[0]->m_llTimeStamp;
}

void CSndUList::insert_(const int64_t& ts, const CUDT* u)
{
   CSNode* n = u->m_pSNode;

   // do not insert repeated node
   if (n->m_iHeapLoc >= 0)
      return;

   m_iLastEntry ++;
   m_pHeap[m_iLastEntry] = n;
   n->m_llTimeStamp = ts;

   int q = m_iLastEntry;
   int p = q;
   while (p != 0)
   {
      p = (q - 1) >> 1;
      if (m_pHeap[p]->m_llTimeStamp > m_pHeap[q]->m_llTimeStamp)
      {
         CSNode* t = m_pHeap[p];
         m_pHeap[p] = m_pHeap[q];
         m_pHeap[q] = t;
         t->m_iHeapLoc = q;
         q = p;
      }
      else
         break;
   }

   n->m_iHeapLoc = q;

   // first entry, activate the sending queue
   if (0 == m_iLastEntry)
   {
      #ifndef WIN32
         pthread_mutex_lock(m_pWindowLock);
         pthread_cond_signal(m_pWindowCond);
         pthread_mutex_unlock(m_pWindowLock);
      #else
         SetEvent(*m_pWindowCond);
      #endif
   }
}

void CSndUList::remove_(const CUDT* u)
{
   CSNode* n = u->m_pSNode;

   if (n->m_iHeapLoc >= 0)
   {
      // remove the node from heap
      m_pHeap[n->m_iHeapLoc] = m_pHeap[m_iLastEntry];
      m_iLastEntry --;
      m_pHeap[n->m_iHeapLoc]->m_iHeapLoc = n->m_iHeapLoc;

      int q = n->m_iHeapLoc;
      int p = q * 2 + 1;
      while (p <= m_iLastEntry)
      {
         if ((p + 1 <= m_iLastEntry) && (m_pHeap[p]->m_llTimeStamp > m_pHeap[p + 1]->m_llTimeStamp))
            p ++;

         if (m_pHeap[q]->m_llTimeStamp > m_pHeap[p]->m_llTimeStamp)
         {
            CSNode* t = m_pHeap[p];
            m_pHeap[p] = m_pHeap[q];
            m_pHeap[p]->m_iHeapLoc = p;
            m_pHeap[q] = t;
            m_pHeap[q]->m_iHeapLoc = q;

            q = p;
            p = q * 2 + 1;
         }
         else
            break;
      }

      n->m_iHeapLoc = -1;
   }
}

//
CSndQueue::CSndQueue():
m_pSndUList(NULL),
m_pChannel(NULL),
m_pTimer(NULL),
m_bClosing(false)
{
   #ifndef WIN32
      pthread_cond_init(&m_WindowCond, NULL);
      pthread_mutex_init(&m_WindowLock, NULL);
   #else
      m_WindowLock = CreateMutex(NULL, false, NULL);
      m_WindowCond = CreateEvent(NULL, false, false, NULL);
      m_ExitCond = CreateEvent(NULL, false, false, NULL);
   #endif
}

CSndQueue::~CSndQueue()
{
   m_bClosing = true;

   #ifndef WIN32
      pthread_mutex_lock(&m_WindowLock);
      pthread_cond_signal(&m_WindowCond);
      pthread_mutex_unlock(&m_WindowLock);
      if (0 != m_WorkerThread)
         pthread_join(m_WorkerThread, NULL);
      pthread_cond_destroy(&m_WindowCond);
      pthread_mutex_destroy(&m_WindowLock);
   #else
      SetEvent(m_WindowCond);
      if (NULL != m_WorkerThread)
         WaitForSingleObject(m_ExitCond, INFINITE);
      CloseHandle(m_WorkerThread);
      CloseHandle(m_WindowLock);
      CloseHandle(m_WindowCond);
   #endif

   //delete m_pSndUList;
   RakNet::OP_DELETE(m_pSndUList,__FILE__,__LINE__);
}

void CSndQueue::init(const CChannel* c, const CTimer* t)
{
   m_pChannel = (CChannel*)c;
   m_pTimer = (CTimer*)t;
//   m_pSndUList = new CSndUList;
   m_pSndUList = RakNet::OP_NEW<CSndUList>(__FILE__, __LINE__);
   m_pSndUList->m_pWindowLock = &m_WindowLock;
   m_pSndUList->m_pWindowCond = &m_WindowCond;
   m_pSndUList->m_pTimer = m_pTimer;

   #ifndef WIN32
      if (0 != pthread_create(&m_WorkerThread, NULL, CSndQueue::worker, this))
      {
         m_WorkerThread = 0;
         THROW_CUDTEXCEPTION(3, 1, -1);
      }
   #else
      DWORD threadID;
      m_WorkerThread = CreateThread(NULL, 0, CSndQueue::worker, this, 0, &threadID);
      if (NULL == m_WorkerThread)
         THROW_CUDTEXCEPTION(3, 1, -1);
   #endif
}

#ifndef WIN32
   void* CSndQueue::worker(void* param)
#else
   DWORD WINAPI CSndQueue::worker(LPVOID param)
#endif
{
   CSndQueue* self = (CSndQueue*)param;

   CPacket pkt;

   while (!self->m_bClosing)
   {
      uint64_t ts = self->m_pSndUList->getNextProcTime();

      if (ts > 0)
      {
         // wait until next processing time of the first socket on the list
         uint64_t currtime;
         CTimer::rdtsc(currtime);
         if (currtime < ts)
            self->m_pTimer->sleepto(ts);

         // it is time to process it, pop it out/remove from the list
         sockaddr* addr;
         CPacket pkt;
         if (self->m_pSndUList->pop(addr, pkt) < 0)
            continue;

         self->m_pChannel->sendto(addr, pkt);
      }
      else
      {
         // wait here if there is no sockets with data to be sent
         #ifndef WIN32
            pthread_mutex_lock(&self->m_WindowLock);
            if (!self->m_bClosing && (self->m_pSndUList->m_iLastEntry < 0))
               pthread_cond_wait(&self->m_WindowCond, &self->m_WindowLock);
            pthread_mutex_unlock(&self->m_WindowLock);
         #else
            WaitForSingleObject(self->m_WindowCond, INFINITE);
         #endif
      }
   }

   #ifndef WIN32
      return NULL;
   #else
      SetEvent(self->m_ExitCond);
      return 0;
   #endif
}

int CSndQueue::sendto(const sockaddr* addr, CPacket& packet)
{
   // send out the packet immediately (high priority), this is a control packet
   m_pChannel->sendto(addr, packet);

   return packet.getLength();
}


//
CRcvUList::CRcvUList():
m_pUList(NULL),
m_pLast(NULL)
{
}

CRcvUList::~CRcvUList()
{
}

void CRcvUList::insert(const CUDT* u)
{
   CRNode* n = u->m_pRNode;
   CTimer::rdtsc(n->m_llTimeStamp);

   n->m_bOnList = true;

   if (NULL == m_pUList)
   {
      // empty list, insert as the single node
      n->m_pPrev = n->m_pNext = NULL;
      m_pLast = m_pUList = n;

      return;
   }

   // always insert at the end for RcvUList
   n->m_pPrev = m_pLast;
   n->m_pNext = NULL;
   m_pLast->m_pNext = n;
   m_pLast = n;
}

void CRcvUList::remove(const CUDT* u)
{
   CRNode* n = u->m_pRNode;

   if (!n->m_bOnList)
      return;

   if (NULL == n->m_pPrev)
   {
      // n is the first node
      m_pUList = n->m_pNext;
      if (NULL == m_pUList)
         m_pLast = NULL;
      else
         m_pUList->m_pPrev = NULL;
   }
   else
   {
      n->m_pPrev->m_pNext = n->m_pNext;
      if (NULL == n->m_pNext)
      {
         // n is the last node
         m_pLast = n->m_pPrev;
      }
      else
         n->m_pNext->m_pPrev = n->m_pPrev;
   }

   n->m_pNext = n->m_pPrev = NULL;

   n->m_bOnList = false;
}

void CRcvUList::update(const CUDT* u)
{
   CRNode* n = u->m_pRNode;

   if (!n->m_bOnList)
      return;

   CTimer::rdtsc(n->m_llTimeStamp);

   // if n is the last node, do not need to change
   if (NULL == n->m_pNext)
      return;

   if (NULL == n->m_pPrev)
   {
      m_pUList = n->m_pNext;
      m_pUList->m_pPrev = NULL;
   }
   else
   {
      n->m_pPrev->m_pNext = n->m_pNext;
      n->m_pNext->m_pPrev = n->m_pPrev;
   }

   n->m_pPrev = m_pLast;
   n->m_pNext = NULL;
   m_pLast->m_pNext = n;
   m_pLast = n;
}

//
CHash::CHash():
m_pBucket(NULL),
m_iHashSize(0)
{
}

CHash::~CHash()
{
   for (int i = 0; i < m_iHashSize; ++ i)
   {
      CBucket* b = m_pBucket[i];
      while (NULL != b)
      {
         CBucket* n = b->m_pNext;
         //delete b;
		 RakNet::OP_DELETE(b,__FILE__,__LINE__);
         b = n;
      }
   }

   //delete [] m_pBucket;
   RakNet::OP_DELETE_ARRAY(m_pBucket,__FILE__, __LINE__);
}

void CHash::init(const int& size)
{
   //m_pBucket = new CBucket* [size];
	m_pBucket = RakNet::OP_NEW_ARRAY<CBucket*>(size, __FILE__, __LINE__ );

   for (int i = 0; i < size; ++ i)
      m_pBucket[i] = NULL;

   m_iHashSize = size;
}

CUDT* CHash::lookup(const int32_t& id)
{
   // simple hash function (% hash table size); suitable for socket descriptors
   CBucket* b = m_pBucket[id % m_iHashSize];

   while (NULL != b)
   {
      if (id == b->m_iID)
         return b->m_pUDT;
      b = b->m_pNext;
   }

   return NULL;
}

void CHash::insert(const int32_t& id, const CUDT* u)
{
   CBucket* b = m_pBucket[id % m_iHashSize];

   // CBucket* n = new CBucket;
   CBucket* n = RakNet::OP_NEW<CBucket>(__FILE__, __LINE__);
   n->m_iID = id;
   n->m_pUDT = (CUDT*)u;
   n->m_pNext = b;

   m_pBucket[id % m_iHashSize] = n;
}

void CHash::remove(const int32_t& id)
{
   CBucket* b = m_pBucket[id % m_iHashSize];
   CBucket* p = NULL;

   while (NULL != b)
   {
      if (id == b->m_iID)
      {
         if (NULL == p)
            m_pBucket[id % m_iHashSize] = b->m_pNext;
         else
            p->m_pNext = b->m_pNext;

         //delete b;
		 RakNet::OP_DELETE(b,__FILE__,__LINE__);

         return;
      }

      p = b;
      b = b->m_pNext;
   }
}


//
CRendezvousQueue::CRendezvousQueue()
{
   #ifndef WIN32
      pthread_mutex_init(&m_RIDVectorLock, NULL);
   #else
      m_RIDVectorLock = CreateMutex(NULL, false, NULL);
   #endif

   m_vRendezvousID.clear();
}

CRendezvousQueue::~CRendezvousQueue()
{
   #ifndef WIN32
      pthread_mutex_destroy(&m_RIDVectorLock);
   #else
      CloseHandle(m_RIDVectorLock);
   #endif

   for (vector<CRL>::iterator i = m_vRendezvousID.begin(); i != m_vRendezvousID.end(); ++ i)
   {
      if (AF_INET == i->m_iIPversion)
      //   delete (sockaddr_in*)i->m_pPeerAddr;
		RakNet::OP_DELETE((sockaddr_in*)i->m_pPeerAddr,__FILE__,__LINE__);
      else
       //  delete (sockaddr_in6*)i->m_pPeerAddr;
		RakNet::OP_DELETE((sockaddr_in6*)i->m_pPeerAddr,__FILE__,__LINE__);
   }

   m_vRendezvousID.clear();
}

void CRendezvousQueue::insert(const UDTSOCKET& id, const int& ipv, const sockaddr* addr)
{
   CGuard vg(m_RIDVectorLock);

   CRL r;
   r.m_iID = id;
   r.m_iIPversion = ipv;
   //r.m_pPeerAddr = (AF_INET == ipv) ? (sockaddr*)new sockaddr_in : (sockaddr*)new sockaddr_in6;
   r.m_pPeerAddr = (AF_INET == ipv) ? (sockaddr*)RakNet::OP_NEW<sockaddr_in>(__FILE__, __LINE__) : (sockaddr*)RakNet::OP_NEW<sockaddr_in6>(__FILE__, __LINE__);
   memcpy(r.m_pPeerAddr, addr, (AF_INET == ipv) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6));

   m_vRendezvousID.insert(m_vRendezvousID.end(), r);
}

void CRendezvousQueue::remove(const UDTSOCKET& id)
{
   CGuard vg(m_RIDVectorLock);

   for (vector<CRL>::iterator i = m_vRendezvousID.begin(); i != m_vRendezvousID.end(); ++ i)
      if (i->m_iID == id)
      {
         if (AF_INET == i->m_iIPversion)
			 //   delete (sockaddr_in*)i->m_pPeerAddr;
			 RakNet::OP_DELETE((sockaddr_in*)i->m_pPeerAddr,__FILE__,__LINE__);
		 else
			 //  delete (sockaddr_in6*)i->m_pPeerAddr;
			 RakNet::OP_DELETE((sockaddr_in6*)i->m_pPeerAddr,__FILE__,__LINE__);

         m_vRendezvousID.erase(i);

         return;
      }
}

bool CRendezvousQueue::retrieve(const sockaddr* addr, UDTSOCKET& id)
{
   CGuard vg(m_RIDVectorLock);

   for (vector<CRL>::iterator i = m_vRendezvousID.begin(); i != m_vRendezvousID.end(); ++ i)
   {
      if (CIPAddress::ipcmp(addr, i->m_pPeerAddr, i->m_iIPversion) && ((0 == id) || (id == i->m_iID)))
      {
         id = i->m_iID;
         return true;
      }
   }

   return false;
}


//
CRcvQueue::CRcvQueue():
m_pRcvUList(NULL),
m_pHash(NULL),
m_pChannel(NULL),
m_pTimer(NULL),
m_bClosing(false),
m_pListener(NULL),
m_pRendezvousQueue(NULL)
{
   #ifndef WIN32
      pthread_mutex_init(&m_PassLock, NULL);
	  pthread_mutex_init(&m_datagramLock, NULL);
      pthread_cond_init(&m_PassCond, NULL);
      pthread_mutex_init(&m_LSLock, NULL);
      pthread_mutex_init(&m_IDLock, NULL);
   #else
      m_PassLock = CreateMutex(NULL, false, NULL);
	  m_datagramLock = CreateMutex(NULL, false, NULL);
      m_PassCond = CreateEvent(NULL, false, false, NULL);
      m_LSLock = CreateMutex(NULL, false, NULL);
      m_IDLock = CreateMutex(NULL, false, NULL);
      m_ExitCond = CreateEvent(NULL, false, false, NULL);
   #endif

   m_vNewEntry.clear();
   m_mBuffer.clear();
}

CRcvQueue::~CRcvQueue()
{
   m_bClosing = true;

   #ifndef WIN32
      if (0 != m_WorkerThread)
         pthread_join(m_WorkerThread, NULL);
      pthread_mutex_destroy(&m_PassLock);
	  pthread_mutex_destroy(&m_datagramLock);
      pthread_cond_destroy(&m_PassCond);
      pthread_mutex_destroy(&m_LSLock);
      pthread_mutex_destroy(&m_IDLock);
   #else
      if (NULL != m_WorkerThread)
         WaitForSingleObject(m_ExitCond, INFINITE);
      CloseHandle(m_WorkerThread);
      CloseHandle(m_PassLock);
	  CloseHandle(m_datagramLock);
      CloseHandle(m_PassCond);
      CloseHandle(m_LSLock);
      CloseHandle(m_IDLock);
   #endif

 //  delete m_pRcvUList;
//   delete m_pHash;
   //delete m_pRendezvousQueue;
	  RakNet::OP_DELETE(m_pRcvUList,__FILE__,__LINE__);
	  RakNet::OP_DELETE(m_pHash,__FILE__,__LINE__);
	  RakNet::OP_DELETE(m_pRendezvousQueue,__FILE__,__LINE__);

   for (map<int32_t, CPacket*>::iterator i = m_mBuffer.begin(); i != m_mBuffer.end(); ++ i)
   {
      //delete [] i->second->m_pcData;
      //delete i->second;

	   RakNet::OP_DELETE_ARRAY(i->second->m_pcData,__FILE__,__LINE__);
	  RakNet::OP_DELETE(i->second,__FILE__,__LINE__);
   }

   while (m_datagramBuffer.empty()==false)
   {
	   //delete m_datagramBuffer.front();
	   RakNet::OP_DELETE(m_datagramBuffer.front(),__FILE__,__LINE__);
	   m_datagramBuffer.pop();
   }
}

void CRcvQueue::init(const int& qsize, const int& payload, const int& version, const int& hsize, const CChannel* cc, const CTimer* t)
{
   m_iPayloadSize = payload;

   m_UnitQueue.init(qsize, payload, version);

   //m_pHash = new CHash;
   m_pHash = RakNet::OP_NEW<CHash>(__FILE__, __LINE__);
   m_pHash->init(hsize);

   m_pChannel = (CChannel*)cc;
   m_pTimer = (CTimer*)t;

   //m_pRcvUList = new CRcvUList;
   m_pRcvUList = RakNet::OP_NEW<CRcvUList>(__FILE__, __LINE__);
   //m_pRendezvousQueue = new CRendezvousQueue;
   m_pRendezvousQueue = RakNet::OP_NEW<CRendezvousQueue>(__FILE__, __LINE__);

   #ifndef WIN32
      if (0 != pthread_create(&m_WorkerThread, NULL, CRcvQueue::worker, this))
      {
         m_WorkerThread = 0;
         THROW_CUDTEXCEPTION(3, 1, -1);
      }
   #else
      DWORD threadID;
      m_WorkerThread = CreateThread(NULL, 0, CRcvQueue::worker, this, 0, &threadID);
      if (NULL == m_WorkerThread)
         THROW_CUDTEXCEPTION(3, 1, -1);
   #endif
}

#ifndef WIN32
   void* CRcvQueue::worker(void* param)
#else
   DWORD WINAPI CRcvQueue::worker(LPVOID param)
#endif
{
   CRcvQueue* self = (CRcvQueue*)param;

//   sockaddr* addr = (AF_INET == self->m_UnitQueue.m_iIPversion) ? (sockaddr*) new sockaddr_in : (sockaddr*) new sockaddr_in6;
   sockaddr* addr = (AF_INET == self->m_UnitQueue.m_iIPversion) ? (sockaddr*)RakNet::OP_NEW<sockaddr_in>(__FILE__, __LINE__) : (sockaddr*)RakNet::OP_NEW<sockaddr_in6>(__FILE__, __LINE__);
   CUDT* u = NULL;
   int32_t id;

   while (!self->m_bClosing)
   {
      #ifdef NO_BUSY_WAITING
         self->m_pTimer->tick();
      #endif

      // check waiting list, if new socket, insert it to the list
      if (self->ifNewEntry())
      {
         CUDT* ne = self->getNewEntry();
         if (NULL != ne)
         {
            self->m_pRcvUList->insert(ne);
            self->m_pHash->insert(ne->m_SocketID, ne);
         }
      }

      // find next available slot for incoming packet
      CUnit* unit = self->m_UnitQueue.getNextAvailUnit();
      if (NULL == unit)
      {
         // no space, skip this packet
         CUnit temp;
         //temp.m_Packet.m_pcData = new char[self->m_iPayloadSize];
		 temp.m_Packet.m_pcData = (char*) rakMalloc_Ex(self->m_iPayloadSize, __FILE__, __LINE__);
         self->m_pChannel->recvfrom(addr, temp.m_Packet);
         //delete [] temp.m_Packet.m_pcData;
		 rakFree_Ex(temp.m_Packet.m_pcData,__FILE__, __LINE__);
         goto TIMER_CHECK;
      }

      unit->m_Packet.setLength(self->m_iPayloadSize);

      // reading next incoming packet
      if (self->m_pChannel->recvfrom(addr, unit->m_Packet) <= 0)
         goto TIMER_CHECK;

//	  sockaddr_in *saInDebug = (sockaddr_in*)addr;
//	  if ((((*saInDebug).sin_addr).S_un).S_addr!=3028017368)
//	  {
//		  printf("RakNet=%i UDT=%i\n", unit->m_Packet.m_PacketVector[0].iov_base[0], ntohl(unit->m_Packet.m_iID));
//	  }

	  // self->m_pChannel->SetPacketHeaderToHostOrder(unit->m_Packet);

     // id = unit->m_Packet.m_iID;
	  id = ntohl(unit->m_Packet.m_iID);

      // ID 0 is for connection request, which should be passed to the listening socket or rendezvous sockets
      if (0 == id)
	  {
		  // If the message is directed to UDT, munge the header
		  self->m_pChannel->SetPacketHeaderToHostOrder(unit->m_Packet);

         if (NULL != self->m_pListener)
            ((CUDT*)self->m_pListener)->listen(addr, unit->m_Packet);
         else if (self->m_pRendezvousQueue->retrieve(addr, id))
            self->storePkt(id, unit->m_Packet.clone());
      }
      else if (id > 0)
      {
         if (NULL != (u = self->m_pHash->lookup(id)))
         {
			 // If the message is directed to UDT, munge the header
			 self->m_pChannel->SetPacketHeaderToHostOrder(unit->m_Packet);

            if (CIPAddress::ipcmp(addr, u->m_pPeerAddr, u->m_iIPversion))
            {
               if (u->m_bConnected && !u->m_bBroken && !u->m_bClosing)
               {
                  if (0 == unit->m_Packet.getFlag())
                     u->processData(unit);
                  else
                     u->processCtrl(unit->m_Packet);

                  u->checkTimers();
                  self->m_pRcvUList->update(u);
               }
            }
         }
         else if (self->m_pRendezvousQueue->retrieve(addr, id))
		 {
			 // If the message is directed to UDT, munge the header
			 self->m_pChannel->SetPacketHeaderToHostOrder(unit->m_Packet);

            self->storePkt(id, unit->m_Packet.clone());
		}
		 else
			 self->storeDatagram(addr, &unit->m_Packet, (int) self->m_pChannel->GetSocket());
      }
	  else
		  self->storeDatagram(addr, &unit->m_Packet, (int) self->m_pChannel->GetSocket());

TIMER_CHECK:
      // take care of the timing event for all UDT sockets

      CRNode* ul = self->m_pRcvUList->m_pUList;
      uint64_t currtime;
      CTimer::rdtsc(currtime);
      uint64_t ctime = currtime - 100000 * CTimer::getCPUFrequency();

      while ((NULL != ul) && (ul->m_llTimeStamp < ctime))
      {
         CUDT* u = ul->m_pUDT;

         if (u->m_bConnected && !u->m_bBroken && !u->m_bClosing)
         {
            u->checkTimers();
            self->m_pRcvUList->update(u);
         }
         else
         {
            // the socket must be removed from Hash table first, then RcvUList
            self->m_pHash->remove(u->m_SocketID);
            self->m_pRcvUList->remove(u);
         }

         ul = self->m_pRcvUList->m_pUList;
      }
   }

   if (AF_INET == self->m_UnitQueue.m_iIPversion)
      //delete (sockaddr_in*)addr;
	  RakNet::OP_DELETE((sockaddr_in*)addr,__FILE__,__LINE__);
   else
      //delete (sockaddr_in6*)addr;
	  RakNet::OP_DELETE((sockaddr_in6*)addr,__FILE__,__LINE__);

   #ifndef WIN32
      return NULL;
   #else
      SetEvent(self->m_ExitCond);
      return 0;
   #endif
}

int CRcvQueue::recvfrom(const int32_t& id, CPacket& packet)
{
   CGuard bufferlock(m_PassLock);

   map<int32_t, CPacket*>::iterator i = m_mBuffer.find(id);

   if (i == m_mBuffer.end())
   {
      #ifndef WIN32
         uint64_t now = CTimer::getTime();
         timespec timeout;

         timeout.tv_sec = now / 1000000 + 1;
         timeout.tv_nsec = (now % 1000000) * 1000;

         pthread_cond_timedwait(&m_PassCond, &m_PassLock, &timeout);
      #else
         ReleaseMutex(m_PassLock);
         WaitForSingleObject(m_PassCond, 1000);
         WaitForSingleObject(m_PassLock, INFINITE);
      #endif

      i = m_mBuffer.find(id);
      if (i == m_mBuffer.end())
      {
         packet.setLength(-1);
         return -1;
      }
   }

   if (packet.getLength() < i->second->getLength())
   {
      packet.setLength(-1);
      return -1;
   }

   memcpy(packet.m_nHeader, i->second->m_nHeader, CPacket::m_iPktHdrSize);
   memcpy(packet.m_pcData, i->second->m_pcData, i->second->getLength());
   packet.setLength(i->second->getLength());

   //delete [] i->second->m_pcData;
   rakFree_Ex(i->second->m_pcData,__FILE__, __LINE__);
   //delete i->second;
   RakNet::OP_DELETE(i->second,__FILE__,__LINE__);
   m_mBuffer.erase(i);

   return packet.getLength();
}
CDatagram* CRcvQueue::recvDatagram()
{
	CDatagram *dg;
	CGuard bufferlock(m_datagramLock);
	if (m_datagramBuffer.empty()==false)
	{
		dg=m_datagramBuffer.front();
		m_datagramBuffer.pop();
		return dg;
	}
	return 0;
}
int CRcvQueue::setListener(const CUDT* u)
{
   CGuard lslock(m_LSLock);

   if (NULL != m_pListener)
      return -1;

   m_pListener = (CUDT*)u;
   return 1;
}

void CRcvQueue::removeListener(const CUDT* u)
{
   CGuard lslock(m_LSLock);

   if (u == m_pListener)
      m_pListener = NULL;
}

void CRcvQueue::setNewEntry(CUDT* u)
{
   CGuard listguard(m_IDLock);
   m_vNewEntry.insert(m_vNewEntry.end(), u);
}

bool CRcvQueue::ifNewEntry()
{
   return !(m_vNewEntry.empty());
}

CUDT* CRcvQueue::getNewEntry()
{
   CGuard listguard(m_IDLock);

   if (m_vNewEntry.empty())
      return NULL;

   CUDT* u = (CUDT*)*(m_vNewEntry.begin());
   m_vNewEntry.erase(m_vNewEntry.begin());

   return u;
}

void CRcvQueue::storePkt(const int32_t& id, CPacket* pkt)
{
   #ifndef WIN32
      pthread_mutex_lock(&m_PassLock);
   #else
      WaitForSingleObject(m_PassLock, INFINITE);
   #endif

   map<int32_t, CPacket*>::iterator i = m_mBuffer.find(id);

   if (i == m_mBuffer.end())
      m_mBuffer[id] = pkt;
   else
   {
      //delete [] i->second->m_pcData;
	   rakFree_Ex(i->second->m_pcData,__FILE__, __LINE__);
      //delete i->second;
	  RakNet::OP_DELETE(i->second,__FILE__,__LINE__);
      i->second = pkt;
   }

   #ifndef WIN32
      pthread_mutex_unlock(&m_PassLock);
      pthread_cond_signal(&m_PassCond);
   #else
      ReleaseMutex(m_PassLock);
      SetEvent(m_PassCond);
   #endif
}
void CRcvQueue::storeDatagram(sockaddr* addr, CPacket* pkt, int udpSocket)
{
	bool isIPv4 = AF_INET == m_UnitQueue.m_iIPversion;
	// IPv4 uses sockaddr_in
	// IPv6 uses sockaddr_in6

	//CDatagram *datagram = new CDatagram;
	CDatagram *datagram = RakNet::OP_NEW<CDatagram>(__FILE__, __LINE__);

	datagram->byteLength = pkt->m_PacketVector[0].iov_len + pkt->m_PacketVector[1].iov_len;
	datagram->datagramMsg = (char*) rakMalloc_Ex(datagram->byteLength, __FILE__, __LINE__);
	datagram->udpSocket = udpSocket;
	if (isIPv4)
	{
		datagram->addrFrom = (char*) rakMalloc_Ex(sizeof(sockaddr_in), __FILE__, __LINE__);
		memcpy(datagram->addrFrom, addr, sizeof(sockaddr_in));
	}
	else
	{
		datagram->addrFrom = (char*) rakMalloc_Ex(sizeof(sockaddr_in6), __FILE__, __LINE__);
		memcpy(datagram->addrFrom, addr, sizeof(sockaddr_in6));
	}
	memcpy(datagram->datagramMsg, pkt->m_PacketVector[0].iov_base, pkt->m_PacketVector[0].iov_len);
	memcpy(datagram->datagramMsg+pkt->m_PacketVector[0].iov_len, pkt->m_PacketVector[1].iov_base, pkt->m_PacketVector[1].iov_len);

#ifndef WIN32
	pthread_mutex_lock(&m_datagramLock);
#else
	WaitForSingleObject(m_datagramLock, INFINITE);
#endif

	// Push
	m_datagramBuffer.push(datagram);

#ifndef WIN32
	pthread_mutex_unlock(&m_datagramLock);
#else
		ReleaseMutex(m_datagramLock);
#endif

}

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif // #ifndef _USE_RAKNET_FLOW_CONTROL
