#include "GeometrySystemComponent.h"

void GeometrySystemComponent::Initialize( SystemPropertyList properties )
{
	for( SystemPropertyList::iterator i = properties.begin( ); i != properties.end( ); ++i )
	{
		if ( ( *i ).GetName( ) == "position" )
		{
			_position = ( *i ).GetValue< MathVector3 >( );
			this->PushChanges( System::Changes::Geometry::Position );
		}

		if ( ( *i ).GetName( ) == "scale" )
		{
			_scale = ( *i ).GetValue< MathVector3 >( );
			this->PushChanges( System::Changes::Geometry::Scale );
		}

		if ( ( *i ).GetName( ) == "orientation" )
		{
			_orientation = ( *i ).GetValue< MathQuaternion >( );
			this->PushChanges( System::Changes::Geometry::Orientation );
		}
	}
}

void GeometrySystemComponent::PushChanges( unsigned int systemChanges )
{
	if ( _observer != 0 )
	{
		_observer->Observe( this );
	}
}