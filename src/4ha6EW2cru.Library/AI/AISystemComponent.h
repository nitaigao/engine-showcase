#ifndef __AISYSTEMCOMPONENT_H
#define __AISYSTEMCOMPONENT_H

#include <string>
#include "../System/SystemType.hpp"

#include "../Scripting/ScriptComponent.h"

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

	AISystemComponent( const std::string& name, IScriptComponent* scriptComponent )
		: _name( name )
		, _scriptComponent( scriptComponent )
		, _observer( 0 )
	{

	}

	void Initialize( AnyValueMap properties );
	void Update( float deltaMilliseconds );
	void Destroy( ) { };

	void AddObserver( IObserver* observer );
	void Observe( ISubject* subject, unsigned int systemChanges );
	void PushChanges( unsigned int systemChanges );

	inline const std::string& GetName( ) { return _name; };
	inline SystemType GetType( ) { return AISystemType; };

	inline unsigned int GetRequestedChanges( ) 
	{ 
		return System::Changes::Geometry::All;
	};

	inline AnyValueMap GetProperties( ) { return AnyValueMap( ); };
	inline void SetProperties( AnyValueMap systemProperties ) { };

	inline MathVector3 GetPosition( ) { return _position; };
	inline MathVector3 GetScale( ) { return MathVector3::Zero( ); };
	inline MathQuaternion GetOrientation( ) { return _orientation; };

	void SetBehavior( const std::string& behavior );
	std::string GetBehavior( ) const { return _behavior; };

	void WalkForward( );
	void WalkBackward( );
	void FacePlayer( );
	void FireWeapon( );
	float GetPlayerDistance( );

private:

	std::string _name;
	IScriptComponent* _scriptComponent;

	IObserver* _observer;

	std::string _behavior;

	MathVector3 _position;
	MathQuaternion _orientation;

	MathVector3 _playerPosition;
	MathQuaternion _playerOrientation;

};

#endif