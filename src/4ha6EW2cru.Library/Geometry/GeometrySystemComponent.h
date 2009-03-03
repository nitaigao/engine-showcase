#ifndef __GEOMETRYSYSTEMCOMPONENT_H
#define __GEOMETRYSYSTEMCOMPONENT_H

class GeometrySystemComponent : public ISystemComponent
{

public:

	virtual ~GeometrySystemComponent( ) { };

	GeometrySystemComponent( const std::string& name )
		: _name( name )
	{

	}

	SystemType GetType( ) { return GeometrySystemType; };
	void Initialize( SystemPropertyList properties ) { };

private:

	std::string _name;

};

#endif