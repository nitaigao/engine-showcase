#ifndef __KEYBOARDLISTENER_H
#define __KEYBOARDLISTENER_H

#include "OIS/OISKeyboard.h"

#include "../Events/EventManager.h"
#include "../Events/Event.h"
#include "../Events/EventData.hpp"

using OIS::KeyEvent;

/*! 
	Listens for input from the Keboard 
*/
class KeyboardListener : public OIS::KeyListener
{

public:

	/* Fired when the user presses a button on the keyboard */
	bool keyPressed( const KeyEvent &arg )
	{
		Event* event = new Event( INPUT_KEY_DOWN, new KeyEventData( arg.key ) );
		EventManager::GetInstance( )->TriggerEvent( event );

		return true;
	};

	/* Fired when the user releases a button on the keyboard */
	bool keyReleased( const KeyEvent &arg )
	{
		Event* event = new Event( INPUT_KEY_UP, new KeyEventData( arg.key ) );
		EventManager::GetInstance( )->TriggerEvent( event );

		return true;
	};

};

#endif