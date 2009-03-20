#ifndef __PHYSICSSYSTEMCOMPONENT_H
#define __PHYSICSSYSTEMCOMPONENT_H

#include "../System/ISystemComponent.hpp"
#include "HavokPhysicsSystemScene.h"

#include "../Maths/MathVector3.hpp"
#include "../Maths/MathQuaternion.hpp"

#include <Common/Base/hkBase.h>
#include <Common/Serialize/Util/hkLoader.h>

#include <Physics/Utilities/CharacterControl/CharacterRigidBody/hkpCharacterRigidBody.h>
#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>

class PhysicsSystemComponent : public ISystemComponent
{

public:

	virtual ~PhysicsSystemComponent( );

	PhysicsSystemComponent( const std::string& name, HavokPhysicsSystemScene* scene )
		: _name ( name )
		, _scene( scene )
		, _body( 0 )
		, _characterBody( 0 )
		, _characterContext( 0 )
		, _loadedData( 0 )
	{

	}

	virtual void Initialize( SystemPropertyList properties );

	inline void AddObserver( IObserver* observer ) { _observer = observer; };

	void Observe( ISubject* subject, unsigned int systemChanges );
	void PushChanges( unsigned int systemChanges );

	inline unsigned int GetRequestedChanges( ) 
	{ 
		return 
			System::Changes::Geometry::Position | 
			System::Changes::Geometry::Orientation | 
			System::Changes::Geometry::Scale |
			System::Changes::Geometry::All |
			System::Changes::Input::Move_Forward |
			System::Changes::Input::Move_Backward |
			System::Changes::Input::Strafe_Left |
			System::Changes::Input::Strafe_Right |
			System::Changes::Input::Turn_Left |
			System::Changes::Input::Turn_Right;
	};

	inline const std::string& GetName( ) { return _name; };
	inline SystemType GetType( ) { return PhysicsSystemType; };

	void Update( float deltaMilliseconds );

	MathVector3 GetPosition( );
	MathQuaternion GetRotation( );

private:

	std::string _name;
	IObserver* _observer;
	HavokPhysicsSystemScene* _scene;
	
	hkPackfileData* _loadedData;
	hkpRigidBody* _body;

	hkpCharacterContext* _characterContext;
	hkpCharacterRigidBody* _characterBody;

	PhysicsSystemComponent( const PhysicsSystemComponent & copy ) { };
	PhysicsSystemComponent & operator = ( const PhysicsSystemComponent & copy ) { return *this; };

};

#endif