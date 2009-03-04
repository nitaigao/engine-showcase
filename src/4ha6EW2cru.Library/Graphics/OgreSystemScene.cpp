#include "OgreSystemScene.h"

#include "OgreRenderSystem.h"
#include "OgreRenderSystemComponent.h"

#include <Ogre.h>
using namespace Ogre;

ISystemComponent* OgreSystemScene::CreateComponent( const std::string& name )
{
	return new OgreRenderSystemComponent( name, this );
}

void OgreSystemScene::DestroyComponent( ISystemComponent* component )
{
	delete component;
}