#include "OgreSystemScene.h"

#include "OgreRenderSystem.h"
#include "OgreRenderSystemComponent.h"

#include <Ogre.h>
using namespace Ogre;

ISystemComponent* OgreSystemScene::CreateComponent( const std::string& name, const std::string& type )
{
	return new OgreRenderSystemComponent( name, this );
}

void OgreSystemScene::DestroyComponent( ISystemComponent* component )
{
	delete component;
}

void OgreSystemScene::Update( float deltaMilliseconds )
{
	_system->Update( deltaMilliseconds );
}