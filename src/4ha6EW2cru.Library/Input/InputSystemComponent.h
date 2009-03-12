#ifndef __INPUTSYSTEMCOMPONENT_H
#define __INPUTSYSTEMCOMPONENT_H

class InputSystemComponent : public ISystemComponent
{

public:

	virtual ~InputSystemComponent( ) { };

	InputSystemComponent( const std::string& name )
		: _name( name )
	{

	}

	void Initialize( SystemPropertyList properties ) { };

	inline void AddObserver( IObserver* observer ) { };

	void Observe( ISubject* subject, unsigned int systemChanges ) { };
	void PushChanges( unsigned int systemChanges ) { };

	inline unsigned int GetRequestedChanges( ) { return 0; };

	inline const std::string& GetName( ) { return _name; };
	inline SystemType GetType( ) { return InputSystemType; };

private:

	std::string _name;
};

#endif