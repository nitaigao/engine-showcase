#ifndef __INPUTSYSTEMCOMPONENT_H
#define __INPUTSYSTEMCOMPONENT_H

#include <string>
#include <ois/OIS.h>

#include "../System/SystemType.hpp"
#include "../System/ISystemComponent.hpp"

class InputSystemComponent : public ISystemComponent
{

public:

	virtual ~InputSystemComponent( ) { };

	InputSystemComponent( const std::string& name )
		: _name( name )
		, _observer( 0 )
	{

	}

	void Initialize( AnyValue::AnyValueMap properties ) { };
	void Update( float deltaMilliseconds ) { };
	void Destroy( ) { };

	inline void AddObserver( IObserver* observer ) { _observer = observer; };
	void Observe( ISubject* subject, unsigned int systemChanges ) { };
	void PushChanges( unsigned int systemChanges );

	inline const std::string& GetName( ) { return _name; };
	inline System::Types::Type GetType( ) { return System::Types::INPUT; };

	inline unsigned int GetRequestedChanges( ) { return System::Changes::None; };

	inline AnyValue::AnyValueMap GetProperties( ) { return _systemProperties; };
	inline void SetProperties( AnyValue::AnyValueMap systemProperties ) { _systemProperties = systemProperties; };

	inline MathVector3 GetPosition( ) { return MathVector3::Zero( ); };
	inline MathVector3 GetScale( ) { return MathVector3::Zero( ); };
	inline MathQuaternion GetOrientation( ) { return MathQuaternion::Identity( ); };

	virtual void KeyDown( const OIS::KeyCode& keyCode, const std::string& keyText );
	virtual void KeyUp( const OIS::KeyCode& keyCode, const std::string& keyText );

private:

	std::string _name;
	IObserver* _observer;

	AnyValue::AnyValueMap _systemProperties;

	InputSystemComponent( ) { };
	InputSystemComponent( const InputSystemComponent & copy ) { };
	InputSystemComponent & operator = ( const InputSystemComponent & copy ) { return *this; };

};

#endif