#include "RendererSystemScene.h"

#include "RendererSystem.h"
#include "RendererSystemComponent.h"
#include "RendererSystemCameraComponent.h"

#include <Ogre.h>
using namespace Ogre;

ISystemComponent* RendererSystemScene::CreateComponent( const std::string& name, const std::string& type )
{
	RendererSystemComponent* component = 0;

	if( type == "camera" )
	{
		component = new RendererSystemCameraComponent( name, this );
	}
	else
	{
		component = new RendererSystemComponent( name, this );
	}
	
	_components.push_back( component );
	return component;
}

void RendererSystemScene::DestroyComponent( ISystemComponent* component )
{
	for( RendererSystemComponentList::iterator i = _components.begin( ); i != _components.end( ); ++i )
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

void RendererSystemScene::Update( float deltaMilliseconds )
{
	for( RendererSystemComponentList::iterator i = _components.begin( ); i != _components.end( ); ++i )
	{
		RendererSystemComponent* renderSystemComponent = static_cast< RendererSystemComponent* > ( ( *i ) );
		renderSystemComponent->Update( deltaMilliseconds );
	}
}

IRendererSystemComponent* RendererSystemScene::GetComponent( const std::string& name )
{
	for( RendererSystemComponentList::iterator i = _components.begin( ); i != _components.end( ); ++i )
	{
		if ( ( *i )->GetName( ) == name )
		{
			return ( *i );
		}
	}
}