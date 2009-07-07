#include "AnimationSystemScene.h"

#include "AnimationSystemComponent.h"

namespace Animation
{
	ISystemComponent* AnimationSystemScene::CreateComponent( const std::string& name, const std::string& type )
	{
		IAnimationSystemComponent* component = new AnimationSystemComponent( this );
		component->SetAttribute( System::Attributes::Name, name );
		component->SetAttribute( System::Attributes::SystemType, System::Types::ANIMATION );
		component->SetAttribute( System::Attributes::Parent, this );

		m_components.insert( std::make_pair( name, component ) );

		return component;
	}

	void AnimationSystemScene::DestroyComponent( ISystemComponent* component )
	{
		m_components.erase( component->GetAttributes( )[ System::Attributes::Name ].As< std::string >( ) );
		component->Destroy( ); 
		delete component;
	};

	void AnimationSystemScene::Update( const float& deltaMilliseconds )
	{
		for( ISystemComponent::SystemComponentMap::iterator i = m_components.begin( ); i != m_components.end( ); ++i )
		{
			( *i ).second->Update( deltaMilliseconds );
		}
	}
}