#include "RendererSystemScene.h"

#include "RendererSystemComponent.h"
#include "RendererSystemCameraComponent.h"

namespace Renderer
{
	ISystemComponent* RendererSystemScene::CreateComponent( const std::string& name, const std::string& type )
	{
		IRendererSystemComponent* component = 0;

		if( type == "camera" )
		{
			component = new RendererSystemCameraComponent( name, this );
		}
		else
		{
			component = new RendererSystemComponent( name, this );
		}

		_components[ name ] = component;

		return component;
	}

	void RendererSystemScene::DestroyComponent( ISystemComponent* component )
	{
		_components.erase( component->GetName( ) );
		component->Destroy( );
		delete component;
		component = 0;
	}

	void RendererSystemScene::Update( const float& deltaMilliseconds )
	{
		for( RendererSystemComponentList::iterator i = _components.begin( ); i != _components.end( ); ++i )
		{
			( *i ).second->Update( deltaMilliseconds );
		}
	}

	IRendererSystemComponent* RendererSystemScene::GetComponent( const std::string& name ) const
	{
		return ( *( _components.find( name ) ) ).second;
	}
}