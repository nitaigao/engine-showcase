#include "AISystem.h"

#include "AISystemScene.h"

ISystemScene* AISystem::CreateScene()
{
	AISystemScene* scene = new AISystemScene( );
	scene->Initialize( );
	return scene;
}