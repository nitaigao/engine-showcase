#include "InputSystemScene.h"

#include "InputSystemComponent.h"

using namespace OIS;

ISystemComponent* InputSystemScene::CreateComponent( const std::string& name, const std::string& type )
{
	InputSystemComponent* inputComponent = new InputSystemComponent( name );
	_inputComponents.push_back( inputComponent );
	return inputComponent;
}

void InputSystemScene::DestroyComponent( ISystemComponent* component )
{
	for( InputSystemComponentList::iterator i = _inputComponents.begin( ); i != _inputComponents.end( ); ++i  )
	{
		if ( component->GetName( ) == ( *i )->GetName( ) )
		{
			_inputComponents.erase( i );
			break;
		}
	}

	delete component;
	component = 0;
}

void InputSystemScene::Update( float deltaMilliseconds )
{
	if ( _inputAllowed )
	{
		unsigned int changes = 0;

		if ( _keyboard->isKeyDown( OIS::KC_W ) )
		{
			changes |= System::Changes::Input::Move_Forward;
		}

		if ( _keyboard->isKeyDown( OIS::KC_S ) )
		{
			changes |= System::Changes::Input::Move_Backward;
		}

		if ( _keyboard->isKeyDown( OIS::KC_A ) )
		{
			changes |= System::Changes::Input::Strafe_Left;
		}

		if ( _keyboard->isKeyDown( OIS::KC_D ) )
		{
			changes |= System::Changes::Input::Strafe_Right;
		}

		if ( _keyboard->isKeyDown( OIS::KC_ESCAPE ) )
		{
			changes |= System::Changes::Input::Pause_Game;
		}

		if ( changes > 0 )
		{
			for( InputSystemComponentList::iterator i = _inputComponents.begin( ); i != _inputComponents.end( ); ++i  )
			{
				( *i )->PushChanges( changes );
			}
		}
	}
}

bool InputSystemScene::keyPressed( const KeyEvent &arg )
{
	if ( _inputAllowed )
	{
		unsigned int changes = 0;

		if ( arg.key == OIS::KC_SPACE )
		{
			changes |= System::Changes::Input::Jump;
		}

		if ( changes > 0 )
		{
			for( InputSystemComponentList::iterator i = _inputComponents.begin( ); i != _inputComponents.end( ); ++i  )
			{
				( *i )->PushChanges( changes );
			}
		}
	}

	return true;
}

bool InputSystemScene::keyReleased( const KeyEvent &arg )
{
	if ( _inputAllowed )
	{
		for( InputSystemComponentList::iterator i = _inputComponents.begin( ); i != _inputComponents.end( ); ++i  )
		{
			( *i )->KeyUp( arg.key, _keyboard->getAsString( arg.key ) );
		}
	}

	return true;
}

/* Fired when the user moves the mouse */
bool InputSystemScene::mouseMoved( const MouseEvent &arg )
{
	if ( _inputAllowed )
	{
		for( InputSystemComponentList::iterator i = _inputComponents.begin( ); i != _inputComponents.end( ); ++i  )
		{
			InputSystemComponent* component = static_cast< InputSystemComponent* >( ( *i ) );

			SystemPropertyList existingProperties = component->GetProperties( );

			SystemPropertyList properties;
			properties[ "mouseX" ] = SystemProperty( "mouseX", arg.state.X.abs );
			properties[ "mouseXDelta" ] = SystemProperty( "mouseXDelta", arg.state.X.rel );
			properties[ "mouseY" ] = SystemProperty( "mouseY", arg.state.Y.abs );
			properties[ "mouseYDelta" ] = SystemProperty( "mouseYDelta", arg.state.Y.rel );

			component->SetProperties( properties );

			component->PushChanges( System::Changes::Input::Mouse_Moved );
		}
	}

	return true;
}

/* Fired when the user presses a button on the mouse */
bool InputSystemScene::mousePressed( const MouseEvent &arg, MouseButtonID id )
{

	return true;
}

/* Fired when the user releases a button on the mouse */
bool InputSystemScene::mouseReleased( const MouseEvent &arg, MouseButtonID id )
{

	return true;
}