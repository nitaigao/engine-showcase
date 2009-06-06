#include "SoundScene.h"

#include "SoundSystemComponent.h"

namespace Sound
{
	ISystemComponent* SoundScene::CreateComponent( const std::string& name, const std::string& type )
	{
		ISoundSystemComponent* component = new SoundSystemComponent( name, this );
		component->SetAttribute( System::Attributes::Name, name );
		component->SetAttribute( System::Attributes::SystemType, System::Types::SOUND );
		component->SetAttribute( System::Attributes::Parent, this );
		return component;
	}

	void SoundScene::DestroyComponent( ISystemComponent* component )
	{
		component->Destroy( ); 
		delete component;
	};
}