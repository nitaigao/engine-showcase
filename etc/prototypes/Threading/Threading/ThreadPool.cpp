#include "ThreadPool.h"

ThreadPool::~ThreadPool()
{
	CloseHandle( _queueMutex );
	_queueMutex = 0;
}

ThreadPool::ThreadPool()
{
	_queueMutex = CreateMutex( 0, 0, 0 );
}

void ThreadPool::Initialize( int threadCount )
{
	for( int i = 0; i < threadCount; i++ )
	{
		ThreadWorkerParams* params = new ThreadWorkerParams( this, i );
		_beginthreadex( 0, 0, ThreadWorker, params, 0, 0 );
	}
}

void ThreadPool::ProcessNext( int threadId )
{
	WaitForSingleObject( _queueMutex, INFINITE );

	if ( !_taskQueue.empty( ) )
	{
		Task task = _taskQueue.front( );

		_taskQueue.pop( );

		ReleaseMutex( _queueMutex );

		task( 0 );
	}
}

unsigned __stdcall ThreadPool::ThreadWorker( void* param )
{
	ThreadWorkerParams* workerParams = static_cast< ThreadWorkerParams* >( param );

	ThreadPool* threadPool = workerParams->first;
	int threadId = workerParams->second;

	while( true )
	{
		threadPool->ProcessNext( threadId );
	}

	delete param;
	param = 0;

	return 0;
}