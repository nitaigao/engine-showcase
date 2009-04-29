#include "SoundScene.h"

#include "SoundSystemComponent.h"

namespace Sound
{
	ISystemComponent* SoundScene::CreateComponent( const std::string& name, const std::string& type )
	{
		return new SoundSystemComponent( name );
	}
}