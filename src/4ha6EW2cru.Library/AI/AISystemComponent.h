#ifndef __AISYSTEMCOMPONENT_H
#define __AISYSTEMCOMPONENT_H

#include <string>
#include "../System/SystemType.hpp"
#include "../System/ISystemComponent.hpp"

#include "../Maths/MathVector3.hpp"
#include "../Maths/MathQuaternion.hpp"

class IBehaviour
{

public:

	virtual ~IBehaviour( ) { };

	virtual void SetBehavior( const std::string& behavior ) = 0;
	virtual std::string GetBehavior( ) const = 0;

};

class AISystemComponent : public ISystemComponent, public IBehaviour
{

public:

	virtual ~AISystemComponent( ) { };

	AISystemComponent( const std::string& name, ISystemComponent* scriptComponent )
		: _name( name )
		, _scriptComponent( scriptComponent )
		, _observer( 0 )
	{

	}

	void Initialize( AnyValueMap properties );
	void Destroy( ) { };

	void AddObserver( IObserver* observer );
	inline const std::string& GetName( ) { return _name; };
	inline SystemType GetType( ) { return AISystemType; };

	inline unsigned int GetRequestedChanges( ) 
	{ 
		return 
			System::Changes::Geometry::Position | 
			System::Changes::Geometry::Orientation | 
			System::Changes::Geometry::Scale |
			System::Changes::Geometry::All;
	};

	void Observe( ISubject* subject, unsigned int systemChanges );

	void PushChanges( unsigned int systemChanges );

	void Update( float deltaMilliseconds );

	void SetBehavior( const std::string& behavior );
	std::string GetBehavior( ) const { return _behavior; };

	void WalkForward( );
	void WalkBackward( );
	void FacePlayer( );
	void FireWeapon( );
	float GetPlayerDistance( );

	inline MathVector3 GetPosition( ) { return _position; };
	inline MathVector3 GetScale( ) { return MathVector3::Zero( ); };
	inline MathQuaternion GetOrientation( ) { return _orientation; };

private:

	std::string _name;
	ISystemComponent* _scriptComponent;

	IObserver* _observer;

	std::string _behavior;

	MathVector3 _position;
	MathQuaternion _orientation;

	MathVector3 _playerPosition;
	MathQuaternion _playerOrientation;

};

#endif