#include "OgreSystemScene.h"

#include "OgreRenderSystem.h"
#include "OgreRenderSystemComponent.h"

#include <Ogre.h>
using namespace Ogre;

ISystemComponent* OgreSystemScene::CreateComponent( const std::string& name, const std::string& type )
{
	OgreRenderSystemComponent* component = new OgreRenderSystemComponent( name, this );

	_components.push_back( component );
	return component;
}

void OgreSystemScene::DestroyComponent( ISystemComponent* component )
{
	for( SystemComponentList::iterator i = _components.begin( ); i != _components.end( ); ++i )
	{
		if ( ( *i )->GetName( ) == component->GetName( ) )
		{
			_components.erase( i );
			component->Destroy( );
			delete component;
			return;
		}
	}
}

void OgreSystemScene::Update( float deltaMilliseconds )
{
	for( SystemComponentList::iterator i = _components.begin( ); i != _components.end( ); ++i )
	{
		OgreRenderSystemComponent* renderSystemComponent = static_cast< OgreRenderSystemComponent* > ( ( *i ) );
		renderSystemComponent->Update( deltaMilliseconds );
	}
}