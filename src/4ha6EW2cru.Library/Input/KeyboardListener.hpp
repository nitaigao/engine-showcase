#ifndef __KEYBOARDLISTENER_H
#define __KEYBOARDLISTENER_H

#include "OIS/OISKeyboard.h"

#include "../Events/Event.h"
#include "../Events/EventData.hpp"

#include "../System/Management.h"
#include "InputSystemComponent.h"

using OIS::KeyEvent;

/*!
	Listens for input from the Keyboard
*/
class KeyboardListener : public OIS::KeyListener
{

	typedef std::vector< InputSystemComponent* > InputSystemComponentList;

public:

	virtual ~KeyboardListener( ) { };

	KeyboardListener( OIS::Keyboard* keyboard )
		: _keyboard( keyboard )
	{

	};

	void AddInputComponent( InputSystemComponent* inputComponent )
	{
		_inputComponents.push_back( inputComponent );
	}

	/* Fired when the user presses a button on the keyboard */
	bool keyPressed( const KeyEvent &arg )
	{
		for( InputSystemComponentList::iterator i = _inputComponents.begin( ); i != _inputComponents.end( ); ++i  )
		{
			( *i )->KeyDown( arg.key, _keyboard->getAsString( arg.key ) );
		}

		Event* event = new Event( INPUT_KEY_DOWN, new KeyEventData( arg.key, _keyboard->getAsString( arg.key ) ) );
		Management::GetInstance( )->GetEventManager( )->TriggerEvent( event );

		return true;
	};

	/* Fired when the user releases a button on the keyboard */
	bool keyReleased( const KeyEvent &arg )
	{
		for( InputSystemComponentList::iterator i = _inputComponents.begin( ); i != _inputComponents.end( ); ++i  )
		{
			( *i )->KeyUp( arg.key, _keyboard->getAsString( arg.key ) );
		}

		Event* event = new Event( INPUT_KEY_UP, new KeyEventData( arg.key, _keyboard->getAsString( arg.key ) ) );
		Management::GetInstance( )->GetEventManager( )->TriggerEvent( event );

		return true;
	};

private:

	OIS::Keyboard* _keyboard;

	InputSystemComponentList _inputComponents;

	KeyboardListener( ) { };
	KeyboardListener( const KeyboardListener & copy ) { };
	KeyboardListener & operator = ( const KeyboardListener & copy ) { return *this; };

};

#endif
