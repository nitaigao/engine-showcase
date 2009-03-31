#include "ScriptSystem.h"

#include "ScriptSystemScene.h"

ISystemScene* ScriptSystem::CreateScene( )
{
	return new ScriptSystemScene( this, _configuration );
}