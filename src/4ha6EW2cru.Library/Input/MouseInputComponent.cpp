#include "MouseInputComponent.h"

using namespace OIS;

#include "../Maths/MathQuaternion.hpp"
using namespace Maths;

#include "../Events/IEvent.hpp"
using namespace Events;

#include "../Scripting/ScriptEvent.hpp"
using namespace Script;

#include "../Management/Management.h"

namespace Input
{
	void MouseInputComponent::Update( const float& deltaMilliseconds )
	{
		MouseState mouseState = _mouse->getMouseState( );

		_properties[ "yDelta" ] = mouseState.Y.rel;
		_properties[ "xDelta" ] = mouseState.X.rel;

		unsigned int changes = System::Changes::Input::Mouse_Moved;

		if ( _mouse->getMouseState( ).buttonDown( MB_Left ) )
		{
			changes |= System::Changes::Input::Fire;

			IEvent* scriptEvent = new ScriptEvent( "INPUT_MOUSE_PRESSED", MB_Left );
			Management::GetInstance( )->GetEventManager( )->TriggerEvent( scriptEvent );
		}

		this->PushChanges( changes );
	}

	void MouseInputComponent::PushChanges( const unsigned int& systemChanges )
	{
		for( ObserverList::iterator i = _observers.begin( ); i != _observers.end( ); ++i )
		{
			( *i )->Observe( this, systemChanges );
		}
	}

	void MouseInputComponent::Observe( ISubject* subject, const unsigned int& systemChanges )
	{
		ISystemComponent* compoenent = static_cast< ISystemComponent* >( subject );

		if ( System::Changes::Geometry::Orientation & systemChanges )
		{
			_orientation = compoenent->GetOrientation( );
		}

		if ( System::Changes::Geometry::Position & systemChanges )
		{
			_position = compoenent->GetPosition( );
		}
	}
}