#include "HavokPhysicsSystem.h"

using namespace Maths;

#include "Logging/Logger.h"
using namespace Logging;

#include "Management/Management.h"

#include <Common/Base/hkBase.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/Memory/hkThreadMemory.h>
#include <Common/Base/Memory/Memory/Pool/hkPoolMemory.h>			

#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>	

#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>			
#include <Physics/Collide/Query/Collector/RayCollector/hkpAllRayHitCollector.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>			

#include <Physics/Dynamics/World/hkpWorld.h>								
#include <Physics/Dynamics/Entity/hkpRigidBody.h>							

#define INCLUDE_HAVOK_PHYSICS_CLASSES
#define HK_CLASSES_FILE <Common/Serialize/Classlist/hkClasses.h>
#include <Common/Serialize/Util/hkBuiltinTypeRegistry.cxx>
#define HK_COMPAT_FILE <Common/Compat/hkCompatVersions.h>
#include <Common/Compat/hkCompat_All.cxx> 

namespace Physics
{
	ISystemScene* HavokPhysicsSystem::CreateScene( )
	{
		m_scene = new HavokPhysicsSystemScene( this );
		return m_scene;
	}

	void HavokPhysicsSystem::Initialize( Configuration::IConfiguration* configuration )
	{
		hkPoolMemory* memoryManager = new hkPoolMemory( );
		m_threadMemory = new hkThreadMemory( memoryManager );
		hkBaseSystem::init( memoryManager, m_threadMemory, errorReportFunction );
		memoryManager->removeReference( );

		int stackSize = 0x100000;
		m_stackBuffer = hkAllocate<char>( stackSize, HK_MEMORY_CLASS_BASE );
		hkThreadMemory::getInstance( ).setStackArea( m_stackBuffer, stackSize );

		hkpWorldCinfo info;

		info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_DISCRETE;
		info.m_gravity.set( 0,-9.8f,0);
		info.m_collisionTolerance = 0.1f; 
		info.setBroadPhaseWorldSize( 10000.0f );
		info.setupSolverInfo( hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM );

		m_world = new hkpWorld( info );
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

		Management::Get( )->GetServiceManager( )->RegisterService( this ); 
	}

	void HavokPhysicsSystem::Update( const float& deltaMilliseconds )
	{
		m_world->stepDeltaTime( deltaMilliseconds );

#ifdef _DEBUG
		m_vdb->step( deltaMilliseconds );
#endif

		m_scene->Update( deltaMilliseconds );
	}

	void HavokPhysicsSystem::Release()
	{
		m_world->markForWrite( );
		m_world->removeReference( );

#ifdef _DEBUG
		m_vdb->removeReference( ); 
#endif

		m_context->removeReference( );

		m_threadMemory->setStackArea(0, 0);
		hkDeallocate( m_stackBuffer );
		m_threadMemory->removeReference();

		hkBaseSystem::quit( );
	}

	void HavokPhysicsSystem::errorReportFunction( const char* errorMessage, void* errorOutputObject )
	{
		Logger::Get( )->Warn( errorMessage );
	}

	std::vector< std::string > HavokPhysicsSystem::RayQuery( const Maths::MathVector3& origin, const Maths::MathVector3& destination, const bool& sortByDistance, const unsigned int& maxResults )
	{
		hkpWorldRayCastInput input;
		input.m_from = origin.AshkVector4( );
		input.m_to = destination.AshkVector4( );

		hkpAllRayHitCollector collector;

		m_scene->GetSystem( )->GetWorld( )->castRay( input, collector );

		if( sortByDistance )
		{
			collector.sortHits( );
		}

		std::vector< std::string > results;
		unsigned int resultCount = 0;
		for( int i = 0; i < collector.getHits( ).getSize( ); i++ )
		{
			if ( collector.getHits( )[ i ].hasHit( ) )
			{
				hkpRigidBody* body = hkGetRigidBody( collector.getHits( )[ i ].m_rootCollidable );

				if ( body )
				{
					results.push_back( body->getName( ) );
					
					if ( ++resultCount >= maxResults )
					{
						break;
					}
				}
			}
		}

		return results;
	}

	AnyType::AnyTypeMap HavokPhysicsSystem::Execute( const std::string& actionName, AnyType::AnyTypeMap& parameters )
	{
		AnyType::AnyTypeMap results;

		if ( actionName == System::Messages::CastRay )
		{
			results[ "hits" ] = this->RayQuery( 
				parameters[ System::Parameters::Origin ].As< MathVector3 >( ),
				parameters[ System::Parameters::Destination ].As< MathVector3 >( ),
				parameters[ System::Parameters::SortByyDistance ].As< bool >( ),
				parameters[ System::Parameters::MaxResults ].As< int >( )
				);
		}

		return results;
	}
};