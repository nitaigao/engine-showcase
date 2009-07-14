#include "SoundFacade.h"

#include "Management/Management.h"

#include "Service/IService.hpp"
using namespace Services;

using namespace luabind;

namespace Script
{
	scope SoundFacade::RegisterFunctions( )
	{
		return
			class_< SoundFacade >( "SoundFacade" )
				.def( constructor< IScriptComponent* >( ) )
				.def( "triggerEvent", &SoundFacade::TriggerEvent )
				.def( "keyOutEvent", &SoundFacade::KeyOutEvent );
	}

	void SoundFacade::PlayMusic( const std::string& eventPath )
	{
		IService* soundService = Management::Get( )->GetServiceManager( )->FindService( System::Types::SOUND );
		
		AnyType::AnyTypeMap parameters;
		parameters[ "eventPath" ] = eventPath;

		soundService->Execute( "playMusic", parameters );
	}

	void SoundFacade::TriggerEvent( const std::string& eventPath )
	{
		AnyType::AnyTypeMap parameters;
		parameters[ System::Parameters::SoundEventPath ] = eventPath;

		m_component->PushMessage( System::Messages::TriggerSoundEvent, parameters );
	}

	void SoundFacade::KeyOutEvent( const std::string& eventPath )
	{
		AnyType::AnyTypeMap parameters;
		parameters[ System::Parameters::SoundEventPath ] = eventPath;

		m_component->PushMessage( System::Messages::KeyOutSoundEvent, parameters );
	}
}