#ifndef __HAVOKPHYSICS_SYSTEMSCENE
#define __HAVOKPHYSICS_SYSTEMSCENE

#include "../System/ISystemScene.hpp"
#include "../System/ISystemComponent.hpp"

#include <Common/Base/hkBase.h>
#include <Physics/Dynamics/World/hkpWorld.h>

#include <Common/Visualize/hkVisualDebugger.h>
#include <Physics/Utilities/VisualDebugger/hkpPhysicsContext.h>			

class HavokPhysicsSystemScene : public ISystemScene
{

	typedef std::vector< ISystemComponent* > PhysicsSystemComponentList;

public:

	virtual ~HavokPhysicsSystemScene( );

	HavokPhysicsSystemScene( const hkpWorldCinfo& worldInfo );

	ISystemComponent* CreateComponent( const std::string& name );
	void DestroyComponent( ISystemComponent* component );
	SystemType GetType( ) { return PhysicsSystemType; };

	void Update( float deltaMilliseconds );

	hkpWorld* GetWorld( ) { return _world; };

private:

	hkpWorld* _world;
	hkVisualDebugger* _vdb;
	hkpPhysicsContext* _context;
	PhysicsSystemComponentList _components;

	HavokPhysicsSystemScene( ) { };
	HavokPhysicsSystemScene( const HavokPhysicsSystemScene & copy ) { };
	HavokPhysicsSystemScene & operator = ( const HavokPhysicsSystemScene & copy ) { return *this; };

};

#endif