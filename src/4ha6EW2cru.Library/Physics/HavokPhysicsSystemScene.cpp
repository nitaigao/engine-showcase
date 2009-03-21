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
#include <Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>	

#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Common/Base/Thread/Job/ThreadPool/Spu/hkSpuJobThreadPool.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>

// Visual Debugger includes
#include <Common/Visualize/hkVisualDebugger.h>
#include <Physics/Utilities/VisualDebugger/hkpPhysicsContext.h>			

#include "PhysicsSystemComponent.h"
#include "PhysicsSystemCharacterComponent.h"

HavokPhysicsSystemScene::HavokPhysicsSystemScene( const hkpWorldCinfo& worldInfo )
{
	_world = new hkpWorld( worldInfo );

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

	if ( type == "character" )
	{
		component = new PhysicsSystemCharacterComponent( name, this );
	}
	else
	{
		component = new PhysicsSystemComponent( name, this );
	}

	_components.push_back( component );

	return component;
}

void HavokPhysicsSystemScene::Update( float deltaMilliseconds )
{
	deltaMilliseconds = 0.0166f;

	_world->stepDeltaTime( deltaMilliseconds );
	_vdb->step( deltaMilliseconds );

	for( PhysicsSystemComponentList::iterator i = _components.begin( ); i != _components.end( ); ++i )
	{
		PhysicsSystemComponent* physicsComponent = static_cast< PhysicsSystemComponent* >( ( *i ) );

		physicsComponent->Update( deltaMilliseconds );

		physicsComponent->PushChanges( 
			System::Changes::Geometry::Orientation |
			System::Changes::Geometry::Position |
			System::Changes::Geometry::Scale
			);
	}
}

void HavokPhysicsSystemScene::DestroyComponent( ISystemComponent* component )
{
	for( PhysicsSystemComponentList::iterator i = _components.begin( ); i != _components.end( ); ++i )
	{
		if ( component->GetName( ) == ( *i )->GetName( ) )
		{
			_components.erase( i );
			break;
		}
	}

	delete component;
}