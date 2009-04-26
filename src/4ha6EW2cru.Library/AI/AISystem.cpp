#include "AISystem.h"

#include "AISystemScene.h"

namespace AI
{
	ISystemScene* AISystem::CreateScene( )
	{
		AISystemScene* scene = new AISystemScene( );
		scene->Initialize( );
		return scene;
	}
}