#ifndef __THREADPOOL
#define __THREADPOOL

#include <windows.h>
#include <process.h>
#include <queue>
#include <iostream>

class ThreadPool
{
	typedef unsigned ( __stdcall *Task )( void* );
	typedef std::queue< Task > TaskQueue;
	typedef std::pair< ThreadPool*, int > ThreadWorkerParams;

public:

	~ThreadPool( );

	ThreadPool( );

	void Initialize( int threadCount );

	inline void QueueTask( Task task ) { _taskQueue.push( task ); };

	void ProcessNext( int threadId );

	static unsigned __stdcall ThreadWorker( void* param );

private:

	TaskQueue _taskQueue;
	HANDLE _queueMutex;

};



#endif