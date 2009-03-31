#include "HavokPhysicsSystemScene.h"

#include <Common/Base/hkBase.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/Memory/hkThreadMemory.h>
#include <Common/Base/Memory/Memory/Pool/hkPoolMemory.h>
#include <Common/Base/System/Error/hkDefaultError.h>
#include <Common/Base/Monitor/hkMonitorStream.h>

#include <Physics/Collide/hkpCollide.h>										
#include <Physics/Collide/Agent/ConvexAgent/SphereBox/hkpSphereBoxAgent.h>	
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>					
#include <Physics/Collide/Shape/Convex/Sphere/hkpSphereShape.h>				
#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>					

#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>			
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>			

#include <Physics/Dynamics/World/hkpWorld.h>								
#include <Physics/Dynamics/Entity/hkpRigidBody.h>	
#include <Physics/Dynamics/World/hkpSimulationIsland.h>
#include <Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>	

#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Common/Base/Thread/Job/ThreadPool/Spu/hkSpuJobThreadPool.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>

#include <Common/Visualize/hkVisualDebugger.h>
#include <Physics/Utilities/VisualDebugger/hkpPhysicsContext.h>			

#include "PhysicsSystemComponent.h"
#include "PhysicsSystemCharacterComponent.h"

HavokPhysicsSystemScene::HavokPhysicsSystemScene( const hkpWorldCinfo& worldInfo )
{
	_lastComponentId = 0;

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

void HavokPhysicsSystemScene::Update( float deltaMilliseconds )
{
	deltaMilliseconds = 0.0166f;

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