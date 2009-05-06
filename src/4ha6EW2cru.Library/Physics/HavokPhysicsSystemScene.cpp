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

		_components[ name ] = component;

		return component;
	}

	void HavokPhysicsSystemScene::Update( const float& deltaMilliseconds )
	{
		_world->stepDeltaTime( deltaMilliseconds );
		_vdb->step( deltaMilliseconds );

		for( PhysicsSystemComponentList::iterator i = _components.begin( ); i != _components.end( ); ++i )
		{
			( *i ).second->Update( deltaMilliseconds );
		}
	}

	void HavokPhysicsSystemScene::DestroyComponent( ISystemComponent* component )
	{
		_components.erase( component->GetName( ) );
		delete component;
	}
}