#include "InputSystem.h"

#include "../Logging/Logger.h"
using namespace Logging;

#include "../Management/Management.h"

#include "../Exceptions/IntializeFailedException.hpp"
#include "../Exceptions/OutOfRangeException.hpp"
#include "../Exceptions/UnInitializedException.hpp"
#include "../Exceptions/AlreadyInitializedException.hpp"

using namespace OIS;

namespace Input
{
	InputSystem::~InputSystem( )
	{
		m_inputManager->destroyInputSystem( m_inputManager );
	}
	
	void InputSystem::Initialize( ) 
	{ 	
		Logger::Info( "Initializing Input System" );
	
		m_inputManager = OIS::InputManager::createInputSystem( Management::GetInstance( )->GetPlatformManager( )->GetWindowId( ) );
	
		m_keyboard = static_cast< OIS::Keyboard* >( m_inputManager->createInputObject( OIS::OISKeyboard, true ) );
		m_keyboard->setEventCallback( this );
	
		m_mouse = static_cast< OIS::Mouse* >( m_inputManager->createInputObject( OIS::OISMouse, true ) );
		m_mouse->setEventCallback( this );
	
		Management::GetInstance( )->GetServiceManager( )->RegisterService( this );
	}
	
	ISystemScene* InputSystem::CreateScene( )
	{
		InputSystemScene* inputScene = new InputSystemScene( m_mouse, m_keyboard );
	
		m_inputScenes.push_back( inputScene );
	
		return inputScene;
	}
	
	void InputSystem::SetAttribute( const std::string& name, AnyValue value )
	{
		if ( name == "inputAllowed" )
		{
			for( InputSystemSceneList::iterator j = m_inputScenes.begin( ); j != m_inputScenes.end( ); ++j )
			{
				( *j )->SetInputAllowed( value.GetValue< bool >( ) );
			}
		}
	}
	
	
	void InputSystem::Update( const float& deltaMilliseconds )
	{
		m_mouse->capture( );
		m_keyboard->capture( );

		m_mouse->getMouseState( ).width = m_configuration->Find( "Graphics", "width" ).GetValue< int >( );
		m_mouse->getMouseState( ).height = m_configuration->Find( "Graphics", "height" ).GetValue< int >( );
	}
	
	bool InputSystem::keyPressed( const KeyEvent &arg )
	{
		for( InputSystemSceneList::iterator i = m_inputScenes.begin( ); i != m_inputScenes.end( ); ++i )
		{
			( *i )->KeyPressed( arg );
		}
	
		return true;
	}
	
	bool InputSystem::keyReleased( const KeyEvent &arg )
	{
		for( InputSystemSceneList::iterator i = m_inputScenes.begin( ); i != m_inputScenes.end( ); ++i )
		{
			( *i )->KeyReleased( arg );
		}
	
		return true;
	}
	
	/* Fired when the user moves the mouse */
	bool InputSystem::mouseMoved( const MouseEvent &arg )
	{
		for( InputSystemSceneList::iterator i = m_inputScenes.begin( ); i != m_inputScenes.end( ); ++i )
		{
			( *i )->MouseMoved( arg );
		}
	
		return true;
	}
	
	/* Fired when the user presses a button on the mouse */
	bool InputSystem::mousePressed( const MouseEvent &arg, MouseButtonID id )
	{
		for( InputSystemSceneList::iterator i = m_inputScenes.begin( ); i != m_inputScenes.end( ); ++i )
		{
			( *i )->MousePressed( arg, id );
		}
	
		return true;
	}
	
	/* Fired when the user releases a button on the mouse */
	bool InputSystem::mouseReleased( const MouseEvent &arg, MouseButtonID id )
	{
		for( InputSystemSceneList::iterator i = m_inputScenes.begin( ); i != m_inputScenes.end( ); ++i )
		{
			( *i )->MouseReleased( arg, id );
		}
	
		return true;
	}
	
	AnyValue::AnyValueMap InputSystem::Execute( const std::string& actionName, AnyValue::AnyValueMap& parameters )
	{
		AnyValue::AnyValueMap results;
	
		if ( actionName == "setInputAllowed" )
		{
			for( InputSystemSceneList::iterator i = m_inputScenes.begin( ); i != m_inputScenes.end( ); ++i )
			{
				( *i )->SetInputAllowed( parameters[ "inputAllowed" ].GetValue< bool >( ) );
			}
		}
	
		return results;
	}
}