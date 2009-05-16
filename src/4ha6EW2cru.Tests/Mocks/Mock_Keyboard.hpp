#ifndef MOCK_KEYBOARD_H
#define MOCK_KEYBOARD_H

#include "mockpp/mockpp.h"
#include "mockpp/MockObject.h"
#include "mockpp/ExpectationCounter.h"

USING_NAMESPACE_MOCKPP

#include <ois/OISKeyboard.h>


class Mock_Keyboard : public OIS::Keyboard, public MockObject
{

public:

	~Mock_Keyboard( ) { };

	Mock_Keyboard( )
		: MockObject( "Mock_Keyboard", 0 )
		, OIS::Keyboard( "", false, 0, 0 )
	{

	}

	virtual const std::string& getAsString( OIS::KeyCode kc )
	{
		return "";
	}

	virtual void copyKeyStates( char keys[256] ) { };
	virtual bool isKeyDown( OIS::KeyCode key ) { return false; };

	virtual void setBuffered(bool buffered) { };

	virtual void capture() { };
	virtual OIS::Interface* queryInterface(OIS::Interface::IType type) { return 0; };
	virtual void _initialize() { };


private:

	Mock_Keyboard & operator = ( const Mock_Keyboard & copy ) { return *this; };

};

#endif