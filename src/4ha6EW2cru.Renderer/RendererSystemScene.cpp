#include "RendererSystemScene.h"

#include "RendererSystemComponent.h"
#include "FPSCameraComponent.h"
#include "CameraComponent.h"

namespace Renderer
{
	ISystemComponent* RendererSystemScene::CreateComponent( const std::string& name, const std::string& type )
	{
		IRendererSystemComponent* component = 0;

		if( type == "fpscamera" )
		{
			component = new FPSCameraComponent( name, this );
		}
		else if ( type == "camera" )
		{
			component = new CameraComponent( name, this );
		}
		else
		{
			component = new RendererSystemComponent( name, this );
		}

		m_components[ name ] = component;

		component->SetAttribute( System::Attributes::Name, name );
		component->SetAttribute( System::Attributes::SystemType, System::Types::RENDER );
		component->SetAttribute( System::Attributes::Parent, this );

		return component;
	}

	void RendererSystemScene::DestroyComponent( ISystemComponent* component )
	{
		m_components.erase( component->GetAttributes( )[ System::Attributes::Name ].As< std::string >( ) );
		component->Destroy( );
		delete component;
		component = 0;
	}

	void RendererSystemScene::Update( const float& deltaMilliseconds )
	{
		for( IRendererSystemComponent::RendererSystemComponentList::iterator i = m_components.begin( ); i != m_components.end( ); ++i )
		{
			( *i ).second->Update( deltaMilliseconds );
		}
	}

	IRendererSystemComponent* RendererSystemScene::GetComponent( const std::string& name ) const
	{
		return ( *( m_components.find( name ) ) ).second;
	}
}