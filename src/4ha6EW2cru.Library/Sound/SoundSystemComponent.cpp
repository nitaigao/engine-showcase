#include "SoundSystemComponent.h"

#include <luabind/luabind.hpp>
using namespace luabind;

namespace Sound
{
	void SoundSystemComponent::Initialize( AnyValue::AnyValueMap& properties )
	{

	}

	AnyValue SoundSystemComponent::Message( const std::string& message, AnyValue::AnyValueMap parameters )
	{
		if( message == "triggerEvent" )
		{
			this->TriggerEvent( parameters[ "eventPath" ].GetValue< std::string >( ) );
		}

		if ( message == "keyOutEvent" )
		{
			this->KeyoutEvent( parameters[ "eventPath" ].GetValue< std::string >( ) );
		}

		return true;
	}

	void SoundSystemComponent::TriggerEvent( const std::string& eventPath )
	{
		FMOD::Event* event = 0;
		FMOD_RESULT result = m_soundSystemScene->GetSoundSystem( )->GetEventSystem( )->getEvent( eventPath.c_str( ), FMOD_EVENT_DEFAULT, &event );

		if ( result == FMOD_OK )
		{
			event->setCallback( &SoundSystemComponent::OnSoundEventStarted, this );
			result = event->start( );
			m_activeSoundEvents.insert( std::make_pair( eventPath, event ) );
		}
	}

	void SoundSystemComponent::KeyoutEvent( const std::string& eventPath )
	{
		if ( m_activeSoundEvents.find( eventPath ) != m_activeSoundEvents.end( ) )
		{
			FMOD::Event* event = m_activeSoundEvents[ eventPath ];

			int numParameters = 0;
			event->getNumParameters( &numParameters );

			for ( int index = 0; index < numParameters; index++ )
			{
				FMOD::EventParameter* eventParameter = 0;
				event->getParameterByIndex( index, &eventParameter );
				eventParameter->keyOff( );
			}

			m_activeSoundEvents.erase( eventPath );
		}
	}

	FMOD_RESULT F_CALLBACK SoundSystemComponent::OnSoundEventStarted( FMOD_EVENT* event, FMOD_EVENT_CALLBACKTYPE type, void* param1, void* param2, void* userdata )
	{
		FMOD::Event* soundEvent = reinterpret_cast< FMOD::Event* >( event );
		SoundSystemComponent* component = reinterpret_cast< SoundSystemComponent* >( userdata ); 

		switch( type )
		{

		case FMOD_EVENT_CALLBACKTYPE_EVENTSTARTED:

			break;

		case FMOD_EVENT_CALLBACKTYPE_EVENTFINISHED:

			break;
		}

		return FMOD_OK;
	}

}