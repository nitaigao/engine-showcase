#ifndef __PHYSICSSYSTEMCHARACTERCOMPONENT_H
#define __PHYSICSSYSTEMCHARACTERCOMPONENT_H

#include "PhysicsSystemComponent.h"

class PhysicsSystemCharacterComponent : public PhysicsSystemComponent
{

public:

	virtual ~PhysicsSystemCharacterComponent( );

	PhysicsSystemCharacterComponent( const std::string& name, HavokPhysicsSystemScene* scene )
		: PhysicsSystemComponent( name, scene )
		, _characterBody( 0 )
		, _characterContext( 0 )
		, _framesInAir( 0 )
	{
		_previousGround = new hkpSurfaceInfo( );
	}

	virtual void Initialize( SystemPropertyList properties );

	void Update( float deltaMilliseconds );

private:

	hkpCharacterContext* _characterContext;
	hkpCharacterRigidBody* _characterBody;

	hkpSurfaceInfo* _previousGround;
	int _framesInAir;

	PhysicsSystemCharacterComponent & operator = ( const PhysicsSystemCharacterComponent & copy ) { return *this; };

};

#endif