#include <windows.h>

#include <iostream>
using namespace std;

#include "ThreadPool.h"

void _Render( )
{
	cout << "Render########################################################################\n";
	cout << "Render########################################################################\n";
	cout << "Render########################################################################\n";
	cout << "Render########################################################################\n";
	cout << "Render########################################################################\n";
	cout << "Render########################################################################\n";
	cout << "Render########################################################################\n";
	cout << "Render########################################################################\n";
	cout << "Render########################################################################\n";
	cout << "Render########################################################################\n";
	cout << "Render########################################################################\n";
	cout << "Render########################################################################\n";
	cout << "Render########################################################################\n";
	cout << "Render########################################################################\n";
	for ( int i = 0 ; i < 500000000; i++ );
}

unsigned __stdcall Render( void* param )
{
	while ( true )
	{
		_Render( );
	}

	return 0;
}

void _Physics( )
{
	cout << "Physics\n";
	for ( int i = 0 ; i < 10000; i++ );
}

unsigned __stdcall Physics( void* param )
{
	while ( true )
	{
		_Physics( );
	}

	return 0;
}

int main( )
{
	bool quit = false;

	ThreadPool threadPool;

	SYSTEM_INFO SysInfo ;
	GetSystemInfo ( & SysInfo ) ;

	int count = SysInfo.dwNumberOfProcessors;

	count = 1;

	threadPool.Initialize( count );

	threadPool.QueueTask( Render );
	threadPool.QueueTask( Physics );

	while ( !quit );

	return 0;
}