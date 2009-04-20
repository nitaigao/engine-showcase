#include "ScriptSystem.h"

#include "ScriptSystemScene.h"

ISystemScene* ScriptSystem::CreateScene( )
{
	ScriptSystemScene* scriptScene = new ScriptSystemScene( _configuration );
	scriptScene->Initialize( );
	return scriptScene;
}