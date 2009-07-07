#include "NetworkSystemScene.h"

#include "NetworkSystemComponent.h"

namespace Network
{

	ISystemComponent* NetworkSystemScene::CreateComponent( const std::string& name, const std::string& type )
	{
		return new NetworkSystemComponent( );
	}
}