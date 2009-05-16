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

		m_vdb = new hkVisualDebugger( contexts );
		m_vdb->serve( );
	}

	HavokPhysicsSystemScene::~HavokPhysicsSystemScene()
	{
		m_world->markForWrite( );
		m_world->removeReference( );

		m_vdb->removeReference( ); 
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

		m_components[ name ] = component;

		return component;
	}

	void HavokPhysicsSystemScene::Update( const float& deltaMilliseconds )
	{
		m_world->stepDeltaTime( deltaMilliseconds );
		m_vdb->step( deltaMilliseconds );

		for( PhysicsSystemComponentList::iterator i = m_components.begin( ); i != m_components.end( ); ++i )
		{
			( *i ).second->Update( deltaMilliseconds );
		}
	}

	void HavokPhysicsSystemScene::DestroyComponent( ISystemComponent* component )
	{
		m_components.erase( component->GetName( ) );
		delete component;
	}
}