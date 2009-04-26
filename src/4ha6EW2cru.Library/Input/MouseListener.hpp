#ifndef __MOUSELISTENER_H
#define __MOUSELISTENER_H

#include "ois/OISMouse.h"

#include "../Events/Event.h"
#include "../Events/EventData.hpp"

#include "../Management/Management.h"

using OIS::MouseEvent;
using OIS::MouseButtonID;

/*!
	Listens for input from the Mouse
*/
class MouseListener : public OIS::MouseListener
{

public:

	virtual ~MouseListener( ) { }

	MouseListener( ) { };

	/* Fired when the user moves the mouse */
	bool mouseMoved( const MouseEvent &arg )
	{
		Events::Event* event = new Events::Event( Events::INPUT_MOUSE_MOVED, new Events::MouseEventData( arg.state, OIS::MB_Left ) );
		Management::GetInstance( )->GetEventManager( )->TriggerEvent( event );

		return true;
	}

	/* Fired when the user presses a button on the mouse */
	bool mousePressed( const MouseEvent &arg, MouseButtonID id )
	{
		Events::Event* event = new Events::Event( Events::INPUT_MOUSE_PRESSED, new Events::MouseEventData( arg.state, id ) );
		Management::GetInstance( )->GetEventManager( )->TriggerEvent( event );

		return true;
	}

	/* Fired when the user releases a button on the mouse */
	bool mouseReleased( const MouseEvent &arg, MouseButtonID id )
	{
		Events::Event* event = new Events::Event( Events::INPUT_MOUSE_RELEASED, new Events::MouseEventData( arg.state, id ) );
		Management::GetInstance( )->GetEventManager( )->TriggerEvent( event );

		return true;
	}

private:

	MouseListener( const MouseListener & copy ) { };
	MouseListener & operator = ( const MouseListener & copy ) { return *this; };

};

#endif
