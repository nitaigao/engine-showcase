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

		m_components[ name ] = component;

		return component;
	}

	void RendererSystemScene::DestroyComponent( ISystemComponent* component )
	{
		m_components.erase( component->GetName( ) );
		component->Destroy( );
		delete component;
		component = 0;
	}

	void RendererSystemScene::Update( const float& deltaMilliseconds )
	{
		for( RendererSystemComponentList::iterator i = m_components.begin( ); i != m_components.end( ); ++i )
		{
			( *i ).second->Update( deltaMilliseconds );
		}
	}

	IRendererSystemComponent* RendererSystemScene::GetComponent( const std::string& name ) const
	{
		return ( *( m_components.find( name ) ) ).second;
	}
}