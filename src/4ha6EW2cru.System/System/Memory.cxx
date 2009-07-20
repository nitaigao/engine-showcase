#include <Windows.h>

/*void* operator new( size_t size )
{
	return HeapAlloc( GetProcessHeap( ), 0, size );
}

void* operator new[ ] ( size_t size )
{
	return operator new( size );
}

void operator delete( void* ptr )
{
	HeapFree( GetProcessHeap( ), 0, ptr );
}

void operator delete[ ]( void * ptr )
{
	delete ptr;
}*/