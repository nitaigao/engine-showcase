#include "SoundFacade.h"

#include "../Management/Management.h"
#include "../Service/IService.hpp"

#include <luabind/luabind.hpp>
using namespace luabind;

namespace Script
{
	void SoundFacade::RegisterFunctions( lua_State* state )
	{
		module( state )
		[
			class_< SoundFacade >( "SoundFacade" )
				.def( "triggerEvent", &SoundFacade::TriggerEvent )
				.def( "keyOutEvent", &SoundFacade::KeyOutEvent )
		];
	}

	void SoundFacade::PlayMusic( const std::string& eventPath )
	{
		IService* soundService = Management::GetServiceManager( )->FindService( System::Types::SOUND );
		
		AnyType::AnyTypeMap parameters;
		parameters[ "eventPath" ] = eventPath;

		soundService->Execute( "playMusic", parameters );
	}

	void SoundFacade::TriggerEvent( const std::string& eventPath )
	{
		AnyType::AnyTypeMap parameters;
		parameters[ System::Parameters::SoundEventPath ] = eventPath;

		m_scriptComponent->PushMessage( System::Messages::TriggerSoundEvent, parameters );
	}

	void SoundFacade::KeyOutEvent( const std::string& eventPath )
	{
		AnyType::AnyTypeMap parameters;
		parameters[ System::Parameters::SoundEventPath ] = eventPath;

		m_scriptComponent->PushMessage( System::Messages::KeyOutSoundEvent, parameters );
	}
}