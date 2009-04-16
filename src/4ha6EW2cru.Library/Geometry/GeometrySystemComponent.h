#ifndef __GEOMETRYSYSTEMCOMPONENT_H
#define __GEOMETRYSYSTEMCOMPONENT_H

#include "../System/IObserver.hpp"
#include "../System/ISystemComponent.hpp"

#include "../System/SystemProperty.hpp"
#include "../System/SystemType.hpp"

#include "../Maths/MathVector3.hpp"
#include "../Maths/MathQuaternion.hpp"

class GeometrySystemComponent : public ISystemComponent
{

public:

	virtual ~GeometrySystemComponent( ) { };

	GeometrySystemComponent( const std::string& name )
		: _name( name )
		, _observer( 0 )
	{

	}
	
	void Initialize( AnyValueMap properties );
	void Destroy( ) { };

	void AddObserver( IObserver* observer ) { _observer = observer; }
	void PushChanges( unsigned int systemChanges );

	inline unsigned int GetRequestedChanges( )
	{
		return 
			System::Changes::Geometry::All |
			System::Changes::Geometry::Orientation |
			System::Changes::Geometry::Position |
			System::Changes::Geometry::Scale;
	};

	void Observe( ISubject* subject, unsigned int systemChanges );

	inline const std::string& GetName( ) { return _name; };
	inline SystemType GetType( ) { return GeometrySystemType; };

	inline MathVector3 GetPosition( ) { return _position; };
	inline MathVector3 GetScale( ) { return _scale; };
	inline MathQuaternion GetOrientation( ) { return _orientation; };

private:

	std::string _name;
	IObserver* _observer;

	MathVector3 _position;
	MathVector3 _scale;
	MathQuaternion _orientation;

	GeometrySystemComponent( ) { };
	GeometrySystemComponent( const GeometrySystemComponent & copy ) { };
	GeometrySystemComponent & operator = ( const GeometrySystemComponent & copy ) { return *this; };

};

#endif