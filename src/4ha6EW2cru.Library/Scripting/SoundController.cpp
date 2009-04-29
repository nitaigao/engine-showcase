#include "SoundController.h"

#include "../Management/Management.h"
#include "../Service/IService.hpp"

namespace Script
{
	void SoundController::PlayMusic( const std::string& eventPath )
	{
		IService* soundService = Management::GetInstance( )->GetServiceManager( )->FindService( System::Types::SOUND );
		
		AnyValue::AnyValueMap parameters;
		parameters[ "eventPath" ] = eventPath;

		soundService->Execute( "playMusic", parameters );
	}
}