#include "GeometrySystem.h"

#include "GeometrySystemScene.h"

namespace Geometry
{
	ISystemScene* GeometrySystem::CreateScene( )
	{
		return new GeometrySystemScene( this );
	}
};