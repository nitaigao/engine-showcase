#include "GeometrySystemComponent.h"

using namespace Maths;

namespace Geometry
{
	void GeometrySystemComponent::Initialize( AnyValue::AnyValueMap& properties )
	{
		for( AnyValue::AnyValueMap::iterator i = properties.begin( ); i != properties.end( ); ++i )
		{
			if ( ( *i ).first == "position" )
			{
				_position = ( *i ).second.GetValue< MathVector3 >( );
				this->PushChanges( System::Changes::Geometry::Position );
			}

			if ( ( *i ).first == "scale" )
			{
				_scale = ( *i ).second.GetValue< MathVector3 >( );
				this->PushChanges( System::Changes::Geometry::Scale );
			}

			if ( ( *i ).first == "orientation" )
			{
				_orientation = ( *i ).second.GetValue< MathQuaternion >( );
				this->PushChanges( System::Changes::Geometry::Orientation );
			}
		}
	}

	void GeometrySystemComponent::PushChanges( const unsigned int& systemChanges )
	{
		for( ObserverList::iterator i = _observers.begin( ); i != _observers.end( ); ++i )
		{
			( *i )->Observe( this, systemChanges );
		}
	}

	void GeometrySystemComponent::Observe( ISubject* subject, const unsigned int& systemChanges )
	{
		ISystemComponent* component = static_cast< ISystemComponent* >( subject );

		_position = component->GetPosition( );
		_orientation = component->GetOrientation( );

		this->PushChanges( 
			System::Changes::Geometry::Position |
			System::Changes::Geometry::Orientation
			);
	}
};