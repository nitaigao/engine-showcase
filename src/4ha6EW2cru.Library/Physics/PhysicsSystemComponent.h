#ifndef __PHYSICSSYSTEMCOMPONENT_H
#define __PHYSICSSYSTEMCOMPONENT_H

#include "../System/ISystemComponent.hpp"
#include "HavokPhysicsSystemScene.h"

#include "../Maths/MathVector3.hpp"

#include <Common/Base/hkBase.h>
#include <Common/Serialize/Util/hkLoader.h>

class PhysicsSystemComponent : public ISystemComponent
{

public:

	virtual ~PhysicsSystemComponent( );

	PhysicsSystemComponent( const std::string& name, HavokPhysicsSystemScene* scene )
		: _name ( name )
		, _scene( scene )
		, _body( 0 )
		, _loadedData( 0 )
	{

	}

	void Initialize( SystemPropertyList properties );

	inline void AddObserver( IObserver* observer ) { _observer = observer; };

	void Observe( ISubject* subject, unsigned int systemChanges );
	void PushChanges( unsigned int systemChanges );

	inline unsigned int GetRequestedChanges( ) 
	{ 
		return 
			System::Changes::Geometry::Position | 
			System::Changes::Geometry::Orientation | 
			System::Changes::Geometry::Scale |
			System::Changes::Geometry::All;
	};

	inline const std::string& GetName( ) { return _name; };
	inline SystemType GetType( ) { return PhysicsSystemType; };

	MathVector3 GetPosition( );

private:

	std::string _name;
	IObserver* _observer;
	HavokPhysicsSystemScene* _scene;
	
	hkPackfileData* _loadedData;
	hkpRigidBody* _body;

	PhysicsSystemComponent( const PhysicsSystemComponent & copy ) { };
	PhysicsSystemComponent & operator = ( const PhysicsSystemComponent & copy ) { return *this; };

};

#endif