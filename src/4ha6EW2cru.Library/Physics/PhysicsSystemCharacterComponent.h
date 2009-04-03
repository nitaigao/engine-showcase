#ifndef __PHYSICSSYSTEMCHARACTERCOMPONENT_H
#define __PHYSICSSYSTEMCHARACTERCOMPONENT_H

#include "PhysicsSystemComponent.h"

class PhysicsSystemCharacterComponent : public PhysicsSystemComponent
{

public:

	virtual ~PhysicsSystemCharacterComponent( );

	PhysicsSystemCharacterComponent( const std::string& name, HavokPhysicsSystemScene* scene, int componentId )
		: PhysicsSystemComponent( name, scene, componentId )
		, _characterBody( 0 )
		, _characterContext( 0 )
		, _framesInAir( 0 )
		, _forwardBackward( 0 )
		, _leftRight( 0 )
	{
		_previousGround = new hkpSurfaceInfo( );
	}

	virtual void Initialize( AnyValueMap properties );
	virtual void Observe( ISubject* subject, unsigned int systemChanges );

	void Update( float deltaMilliseconds );

private:

	hkpCharacterContext* _characterContext;
	hkpCharacterRigidBody* _characterBody;
	hkpCharacterInput _characterInput;

	float _forwardBackward;
	float _leftRight;

	float _mouseXDelta;

	hkpSurfaceInfo* _previousGround;
	int _framesInAir;

	PhysicsSystemCharacterComponent & operator = ( const PhysicsSystemCharacterComponent & copy ) { return *this; };

};

#endif