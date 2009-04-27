#include "HavokPhysicsSystemScene.h"
			
#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>							

#include <Physics/Dynamics/World/hkpSimulationIsland.h>

#include "PhysicsSystemComponent.h"
#include "PhysicsSystemCharacterComponent.h"

namespace Physics
{
	HavokPhysicsSystemScene::HavokPhysicsSystemScene( const hkpWorldCinfo& worldInfo )
	{
		_lastComponentId = 0;
		_stepAccumulator = 0;

		_world = new hkpWorld( worldInfo );
		_world->addIslandPostIntegrateListener( this );

		hkArray<hkProcessContext*> contexts;

		_context = new hkpPhysicsContext( );
		hkpPhysicsContext::registerAllPhysicsProcesses( );
		_context->addWorld( _world );
		contexts.pushBack( _context );

		hkpAgentRegisterUtil::registerAllAgents( _world->getCollisionDispatcher( ) );

		_vdb = new hkVisualDebugger( contexts );
		_vdb->serve( );
	}

	HavokPhysicsSystemScene::~HavokPhysicsSystemScene()
	{
		_world->markForWrite( );
		_world->removeReference( );

		_vdb->removeReference( ); 
		_context->removeReference( );
	}

	ISystemComponent* HavokPhysicsSystemScene::CreateComponent( const std::string& name, const std::string& type )
	{
		PhysicsSystemComponent* component = 0;
		int componentId = _lastComponentId++;

		if ( type == "character" )
		{
			component = new PhysicsSystemCharacterComponent( name, this, componentId );
		}
		else
		{
			component = new PhysicsSystemComponent( name, this, componentId );
		}

		_components[ componentId ] = component;

		return component;
	}

	void HavokPhysicsSystemScene::Update( const float& deltaMilliseconds )
	{
		_world->stepDeltaTime( deltaMilliseconds );
		_vdb->step( deltaMilliseconds );
	}

	void HavokPhysicsSystemScene::DestroyComponent( ISystemComponent* component )
	{
		for( PhysicsSystemComponentList::iterator i = _components.begin( ); i != _components.end( ); ++i )
		{
			if ( component->GetName( ) == ( *i ).second->GetName( ) )
			{
				_components.erase( i );
				break;
			}
		}

		delete component;
	}

	void HavokPhysicsSystemScene::postIntegrateCallback( hkpSimulationIsland *island, const hkStepInfo &stepInfo )
	{
		const hkArray< hkpEntity* >& entities = island->getEntities( );

		for ( int i = 0; i < entities.getSize( ); i++ )
		{
			hkpEntity* entity = entities[ i ];
			
			int componentId = entity->getUserData( );
			PhysicsSystemComponent* component = static_cast< PhysicsSystemComponent* >( _components[ componentId ] );

			component->Update( stepInfo.m_deltaTime );

			component->PushChanges( 
				System::Changes::Geometry::Orientation |
				System::Changes::Geometry::Position |
				System::Changes::Geometry::Scale
				);
		}
	}
}