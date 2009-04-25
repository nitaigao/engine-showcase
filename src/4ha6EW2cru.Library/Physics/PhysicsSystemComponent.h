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

	PhysicsSystemComponent( const std::string& name, HavokPhysicsSystemScene* scene, int componentId )
		: _name ( name )
		, _scene( scene )
		, _componentId( componentId )
		, _loadedData( 0 )
		, _body( 0 )
	{

	}

	virtual void Initialize( AnyValueMap properties );
	virtual void Update( float deltaMilliseconds ) { };
	void Destroy( ) { };

	inline void AddObserver( IObserver* observer ) { _observer = observer; };
	virtual void Observe( ISubject* subject, unsigned int systemChanges );
	void PushChanges( unsigned int systemChanges );

	inline const std::string& GetName( ) { return _name; };
	inline SystemType GetType( ) { return PhysicsSystemType; };

	inline unsigned int GetRequestedChanges( ) 
	{ 
		return 
			System::Changes::Geometry::All |
			System::Changes::Input::All;
	};

	inline AnyValueMap GetProperties( ) { return AnyValueMap( ); };
	inline void SetProperties( AnyValueMap systemProperties ) { };

	MathVector3 GetPosition( );
	inline MathVector3 GetScale( ) { return MathVector3::Zero( ); };
	MathQuaternion GetOrientation( );

protected:

	std::string _name;
	IObserver* _observer;
	HavokPhysicsSystemScene* _scene;
	int _componentId;
	
	hkpRigidBody* _body;
	hkPackfileData* _loadedData;

	PhysicsSystemComponent( const PhysicsSystemComponent & copy ) { };
	PhysicsSystemComponent & operator = ( const PhysicsSystemComponent & copy ) { return *this; };

};

#endif