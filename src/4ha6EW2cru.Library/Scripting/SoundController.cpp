#include "SoundController.h"

#include "../Management/Management.h"
#include "../Service/IService.hpp"

namespace Script
{
	void SoundController::PlayMusic( const std::string& eventPath )
	{
		IService* soundService = Management::GetInstance( )->GetServiceManager( )->FindService( System::Types::SOUND );
		
		AnyType::AnyTypeMap parameters;
		parameters[ "eventPath" ] = eventPath;

		soundService->Execute( "playMusic", parameters );
	}

	void SoundController::TriggerEvent( const std::string& eventPath )
	{
		AnyType::AnyTypeMap parameters;
		parameters[ System::Attributes::SoundEventPath ] = eventPath;

		m_scriptComponent->PushMessage( System::Messages::TriggerSoundEvent, parameters );
	}

	void SoundController::KeyOutEvent( const std::string& eventPath )
	{
		AnyType::AnyTypeMap parameters;
		parameters[ System::Attributes::SoundEventPath ] = eventPath;

		m_scriptComponent->PushMessage( System::Messages::KeyOutSoundEvent, parameters );
	}
}