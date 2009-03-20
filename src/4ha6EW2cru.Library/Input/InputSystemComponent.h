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

	void Initialize( SystemPropertyList properties ) { };

	inline void AddObserver( IObserver* observer ) { _observer = observer; };

	void Observe( ISubject* subject, unsigned int systemChanges ) { };
	void PushChanges( unsigned int systemChanges );

	inline unsigned int GetRequestedChanges( ) { return 0; };

	inline const std::string& GetName( ) { return _name; };
	inline SystemType GetType( ) { return InputSystemType; };

	virtual void KeyDown( const OIS::KeyCode& keyCode, const std::string& keyText );
	virtual void KeyUp( const OIS::KeyCode& keyCode, const std::string& keyText );

private:

	std::string _name;
	IObserver* _observer;

	InputSystemComponent( ) { };
	InputSystemComponent( const InputSystemComponent & copy ) { };
	InputSystemComponent & operator = ( const InputSystemComponent & copy ) { return *this; };

};

#endif