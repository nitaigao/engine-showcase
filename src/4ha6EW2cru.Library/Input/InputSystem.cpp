#include "InputSystem.h"

#include <luabind/luabind.hpp>
using namespace luabind;

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

		this->LoadMessageBindings( );
	}
	
	ISystemScene* InputSystem::CreateScene( )
	{
		InputSystemScene* inputScene = new InputSystemScene( this );
	
		m_inputScenes.push_back( inputScene );
	
		return inputScene;
	}
	
	void InputSystem::SetAttribute( const std::string& name, AnyType value )
	{
		if ( name == "inputAllowed" )
		{
			for( InputSystemSceneList::iterator j = m_inputScenes.begin( ); j != m_inputScenes.end( ); ++j )
			{
				( *j )->SetInputAllowed( value.As< bool >( ) );
			}
		}
	}

	void InputSystem::Update( const float& deltaMilliseconds )
	{
		m_mouse->capture( );
		m_keyboard->capture( );

		m_mouse->getMouseState( ).width = m_configuration->Find( System::ConfigSections::Graphics, "width" ).As< int >( );
		m_mouse->getMouseState( ).height = m_configuration->Find( System::ConfigSections::Graphics, "height" ).As< int >( );
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
	
	bool InputSystem::mouseMoved( const MouseEvent &arg )
	{
		for( InputSystemSceneList::iterator i = m_inputScenes.begin( ); i != m_inputScenes.end( ); ++i )
		{
			( *i )->MouseMoved( arg );
		}
	
		return true;
	}
	
	bool InputSystem::mousePressed( const MouseEvent &arg, MouseButtonID id )
	{
		for( InputSystemSceneList::iterator i = m_inputScenes.begin( ); i != m_inputScenes.end( ); ++i )
		{
			( *i )->MousePressed( arg, id );
		}
	
		return true;
	}
	
	bool InputSystem::mouseReleased( const MouseEvent &arg, MouseButtonID id )
	{
		for( InputSystemSceneList::iterator i = m_inputScenes.begin( ); i != m_inputScenes.end( ); ++i )
		{
			( *i )->MouseReleased( arg, id );
		}
	
		return true;
	}
	
	AnyType::AnyTypeMap InputSystem::Execute( const std::string& message, AnyType::AnyTypeMap& parameters )
	{
		AnyType::AnyTypeMap results;

		if ( message == System::Messages::RegisterScriptFunctions )
		{
			module( parameters[ System::Attributes::ScriptState ].As< lua_State* >( ) )
			[
				class_< InputMessageBinding >( InputMessageBinding::TypeName( ).c_str( ) )
					.def( "getText", &InputMessageBinding::GetText )
					.def( "getMessage", &InputMessageBinding::GetMessage )
			];
		}

		if ( message == System::Messages::GetBindingForMessage )
		{
			for ( InputMessageBinding::InputMessageBindingList::iterator i = m_messageBindings.begin( ); i != m_messageBindings.end( ); ++i )
			{
				if ( ( *i ).GetMessage( ) == parameters[ System::Attributes::Message ].As< std::string >( ) )
				{
					results[ "result" ] = ( *i );
					return results;
				}
			}

			results[ "result" ] = InputMessageBinding( parameters[ System::Attributes::Message ].As< std::string >( ), "" );
			return results;
		}

		if ( message == System::Messages::SetBindingForMessage )
		{
			// if the binding is already set, unset the old binding
			for ( InputMessageBinding::InputMessageBindingList::iterator i = m_messageBindings.begin( ); i != m_messageBindings.end( ); ++i )
			{
				if( 
					( *i ).GetFullCode( ) == parameters[ System::Attributes::Binding ].As< std::string >( ) &&
					( *i ).GetMessage( ) != parameters[ System::Attributes::Message ].As< std::string >( )
					)
				{
					m_configuration->Set( System::ConfigSections::Bindings, ( *i ).GetMessage( ), "" );
				}
			}

			m_configuration->Set( 
				System::ConfigSections::Bindings, 
				parameters[ System::Attributes::Message ].As< std::string >( ), 
				parameters[ System::Attributes::Binding ].As< std::string >( ) 
				);

			this->LoadMessageBindings( );
		}
	
		if ( message == "setInputAllowed" )
		{
			for( InputSystemSceneList::iterator i = m_inputScenes.begin( ); i != m_inputScenes.end( ); ++i )
			{
				( *i )->SetInputAllowed( parameters[ "inputAllowed" ].As< bool >( ) );
			}
		}

		if ( message == "getMessageBindings" )
		{
			results[ "result" ] = m_messageBindings;
		}
	
		return results;
	}

	void InputSystem::LoadMessageBindings( )
	{
		m_messageBindings.clear( );

		AnyType::AnyTypeMap bindings = m_configuration->FindSection( System::ConfigSections::Bindings );

		for ( AnyType::AnyTypeMap::iterator i = bindings.begin( ); i != bindings.end( ); ++i )
		{
			InputMessageBinding binding( ( *i ).first, ( *i ).second.As< std::string >( ) );

			if ( binding.GetType( ) == BINDING_KEYBOARD && !binding.GetFullCode( ).empty( ) ) 
			{
				binding.SetText( m_keyboard->getAsString( static_cast< OIS::KeyCode >( binding.GetCode( ) ) ) );
			}

			m_messageBindings.push_back( binding );
		}
	}

	void InputSystem::Message( const std::string& message, AnyType::AnyTypeMap parameters )
	{
		if ( message == System::Messages::RegisterService )
		{
			Management::GetInstance( )->GetServiceManager( )->RegisterService( this );
		}
	}
}