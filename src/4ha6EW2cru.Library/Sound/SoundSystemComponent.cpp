#include "SoundSystemComponent.h"

using namespace Maths;

#include <luabind/luabind.hpp>
using namespace luabind;

namespace Sound
{
	AnyValue SoundSystemComponent::Message( const unsigned int& messageId, AnyValue::AnyValueKeyMap parameters )
	{
		if( messageId & System::Messages::TriggerSoundEvent  )
		{
			this->TriggerEvent( parameters[ System::Attributes::SoundEventPath ].GetValue< std::string >( ) );
		}

		if ( messageId & System::Messages::KeyOutSoundEvent )
		{
			this->KeyoutEvent( parameters[ System::Attributes::SoundEventPath ].GetValue< std::string >( ) );
		}

		if ( messageId & System::Messages::SetPosition )
		{
			m_attributes[ System::Attributes::Position ] = parameters[ System::Attributes::Position ].GetValue< MathVector3 >( );

			for ( SoundEventList::iterator i = m_activeSoundEvents.begin( ); i != m_activeSoundEvents.end( ); ++i )
			{
				FMOD_VECTOR position, velocity;

				( *i ).second->get3DAttributes( &position, &velocity );
				( *i ).second->set3DAttributes( &parameters[ System::Attributes::Position ].GetValue< MathVector3 >( ).AsFMODVector( ), &velocity );
			}
		}

		if ( messageId & System::Messages::SetOrientation )
		{
			m_attributes[ System::Attributes::Orientation ] = parameters[ System::Attributes::Orientation ].GetValue< MathQuaternion >( );
		}

		if ( messageId & System::Messages::SetPlayerPosition )
		{
			FMOD_VECTOR position, velocity, forward, up;

			m_soundSystemScene->GetSoundSystem( )->GetEventSystem( )->get3DListenerAttributes( 0, &position, &velocity, &forward, &up );
			m_soundSystemScene->GetSoundSystem( )->GetEventSystem( )->set3DListenerAttributes( 
				0, 
				&parameters[ System::Attributes::PlayerPosition ].GetValue< MathVector3 >( ).AsFMODVector( ), 
				&velocity,
				&MathVector3::Forward( ).AsFMODVector( ), 
				&MathVector3::Up( ).AsFMODVector( ) 
				);
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