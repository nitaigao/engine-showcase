#include "HavokPhysicsSystemScene.h"						

#include <Physics/Dynamics/World/hkpSimulationIsland.h>

#include "PhysicsSystemComponent.h"
#include "PhysicsSystemCharacterComponent.h"

#include "Logging/Logger.h"
using namespace Logging;

namespace Physics
{

	ISystemComponent* HavokPhysicsSystemScene::CreateComponent( const std::string& name, const std::string& type )
	{
		PhysicsSystemComponent* component = 0;

		if ( type == "character" )
		{
			component = new PhysicsSystemCharacterComponent( name, this );
		}
		else
		{
			component = new PhysicsSystemComponent( name, this );
		}

		component->SetAttribute( System::Attributes::Name, name );
		component->SetAttribute( System::Attributes::SystemType, System::Types::PHYSICS );
		component->SetAttribute( System::Attributes::Parent, this );

		m_components[ name ] = component;

		return component;
	}

	void HavokPhysicsSystemScene::Update( const float& deltaMilliseconds )
	{
		for( IPhysicsSystemComponent::PhysicsSystemComponentList::iterator i = m_components.begin( ); i != m_components.end( ); ++i )
		{
			( *i ).second->Update( deltaMilliseconds );
		}
	}

	void HavokPhysicsSystemScene::DestroyComponent( ISystemComponent* component )
	{
		m_components.erase( component->GetAttributes( )[ System::Attributes::Name ].As< std::string >( ) );
		delete component;
	}
}