#ifndef __GEOMETRYSYSTEMCOMPONENT_H
#define __GEOMETRYSYSTEMCOMPONENT_H

#include "../System/IObserver.hpp"
#include "../System/ISystemComponent.hpp"

#include "../System/AnyValue.hpp"
#include "../System/SystemType.hpp"

#include "../Maths/MathVector3.hpp"
#include "../Maths/MathQuaternion.hpp"

class GeometrySystemComponent : public ISystemComponent
{

public:

	virtual ~GeometrySystemComponent( ) { };

	GeometrySystemComponent( const std::string& name )
		: _name( name )
	{

	}
	
	void Initialize( AnyValue::AnyValueMap properties );
	void Update( float deltaMilliseconds ) { };
	void Destroy( ) { };

	void AddObserver( IObserver* observer ) { _observers.push_back( observer ); };
	void Observe( ISubject* subject, unsigned int systemChanges );
	void PushChanges( unsigned int systemChanges );

	inline const std::string& GetName( ) { return _name; };
	inline System::Types::Type GetType( ) { return System::Types::GEOMETRY; };

	inline unsigned int GetRequestedChanges( )
	{
		return System::Changes::Geometry::All;
	};

	inline AnyValue::AnyValueMap GetProperties( ) { return AnyValue::AnyValueMap( ); };
	inline void SetProperties( AnyValue::AnyValueMap systemProperties ) { };

	inline MathVector3 GetPosition( ) { return _position; };
	inline MathVector3 GetScale( ) { return _scale; };
	inline MathQuaternion GetOrientation( ) { return _orientation; };

private:

	std::string _name;

	ObserverList _observers;

	MathVector3 _position;
	MathVector3 _scale;
	MathQuaternion _orientation;

	GeometrySystemComponent( ) { };
	GeometrySystemComponent( const GeometrySystemComponent & copy ) { };
	GeometrySystemComponent & operator = ( const GeometrySystemComponent & copy ) { return *this; };

};

#endif