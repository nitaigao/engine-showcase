#ifndef __MOCKLOGAPPENDER_H
#define __MOCKLOGAPPENDER_H

#include "mockpp/mockpp.h"
#include "mockpp/MockObject.h"
#include "mockpp/ExpectationCounter.h"

USING_NAMESPACE_MOCKPP

class MockLogAppender : public IAppender, public MockObject
{

public:

	MockLogAppender( )
		: MockObject( "MockLogAppender", 0 )
		, append_count( "MockLogAppender/Append", 0 )
	{ };

	ExpectationCounter append_count;

	bool Append( const std::string const message ) const
	{
		MockLogAppender* appenderMock = const_cast< MockLogAppender* >( this );
		appenderMock->append_count.inc( );
		return true;
	}

private:

	MockLogAppender & operator = ( const MockLogAppender & copy ) { return *this; };
};


#endif