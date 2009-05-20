#include "HavokPhysicsSystemScene.h"
			
#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>							

#include <Physics/Dynamics/World/hkpSimulationIsland.h>

#include "PhysicsSystemComponent.h"
#include "PhysicsSystemCharacterComponent.h"

#include "../Logging/Logger.h"
using namespace Logging;

namespace Physics
{
	HavokPhysicsSystemScene::HavokPhysicsSystemScene( const hkpWorldCinfo& worldInfo )
	{
		m_world = new hkpWorld( worldInfo );
		m_groupFilter = new hkpGroupFilter( );
		m_world->setCollisionFilter( m_groupFilter );
		m_groupFilter->removeReference( );

		hkArray<hkProcessContext*> contexts;

		m_context = new hkpPhysicsContext( );
		hkpPhysicsContext::registerAllPhysicsProcesses( );
		m_context->addWorld( m_world );
		contexts.pushBack( m_context );

		hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher( ) );

#ifdef _DEBUG
		m_vdb = new hkVisualDebugger( contexts );
		m_vdb->serve( );
#endif
		
	}

	HavokPhysicsSystemScene::~HavokPhysicsSystemScene()
	{
		m_world->markForWrite( );
		m_world->removeReference( );

#ifdef _DEBUG
		m_vdb->removeReference( ); 
#endif
		
		m_context->removeReference( );
	}

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
		component->SetAttribute( System::Attributes::Type, System::Types::PHYSICS );
		component->SetAttribute( System::Attributes::Parent, this );

		m_components[ name ] = component;

		return component;
	}

	void HavokPhysicsSystemScene::Update( const float& deltaMilliseconds )
	{
		m_world->stepDeltaTime( deltaMilliseconds );

#ifdef _DEBUG
		m_vdb->step( deltaMilliseconds );
#endif

		for( IPhysicsSystemComponent::PhysicsSystemComponentList::iterator i = m_components.begin( ); i != m_components.end( ); ++i )
		{
			( *i ).second->Update( deltaMilliseconds );
		}
	}

	void HavokPhysicsSystemScene::DestroyComponent( ISystemComponent* component )
	{
		m_components.erase( component->GetAttributes( )[ System::Attributes::Name ].GetValue< std::string >( ) );
		delete component;
	}
}