#include "UXSystem.h"

#include "UXSystemScene.h"

namespace UX
{
	ISystemScene* UXSystem::CreateScene()
	{
		_scene = new UXSystemScene( );
		_scene->Initialize( ); 
		return _scene;
	}
};