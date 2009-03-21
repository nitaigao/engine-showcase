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
		, _loadedData( 0 )
		, _body( 0 )
	{

	}

	void Observe( ISubject* subject, unsigned int systemChanges );
	void PushChanges( unsigned int systemChanges );

	virtual void Initialize( SystemPropertyList properties );
	virtual void Update( float deltaMilliseconds ) { };

	inline const std::string& GetName( ) { return _name; };
	inline SystemType GetType( ) { return PhysicsSystemType; };
	inline void AddObserver( IObserver* observer ) { _observer = observer; };
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

	MathVector3 GetPosition( );
	MathQuaternion GetRotation( );

protected:

	std::string _name;
	IObserver* _observer;
	HavokPhysicsSystemScene* _scene;
	
	hkpRigidBody* _body;
	hkPackfileData* _loadedData;

	PhysicsSystemComponent( const PhysicsSystemComponent & copy ) { };
	PhysicsSystemComponent & operator = ( const PhysicsSystemComponent & copy ) { return *this; };

};

#endif