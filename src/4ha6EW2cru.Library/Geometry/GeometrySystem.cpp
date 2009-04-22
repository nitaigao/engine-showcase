#include "GeometrySystem.h"

#include "GeometrySystemScene.h"

ISystemScene* GeometrySystem::CreateScene( )
{
	return new GeometrySystemScene( this );
}