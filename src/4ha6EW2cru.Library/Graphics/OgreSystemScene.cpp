#include "OgreSystemScene.h"

#include "OgreRenderSystem.h"
#include "OgreRenderSystemComponent.h"

#include <Ogre.h>
using namespace Ogre;

ISystemComponent* OgreSystemScene::CreateComponent( const std::string& name )
{
	Root* root = ( ( OgreRenderSystem* ) _system )->GetRoot( );
	OgreRenderSystemComponent* component = new OgreRenderSystemComponent( name, root->getSceneManager( "default" ) );
	return component;
}

void OgreSystemScene::DestroyComponent( ISystemComponent* component )
{
	OgreRenderSystemComponent* renderComponent = ( OgreRenderSystemComponent* ) component;
}