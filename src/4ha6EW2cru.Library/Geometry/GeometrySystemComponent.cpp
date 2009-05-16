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
				m_position = ( *i ).second.GetValue< MathVector3 >( );
				this->PushChanges( System::Changes::Geometry::Position );
			}

			if ( ( *i ).first == "orientation" )
			{
				m_orientation = ( *i ).second.GetValue< MathQuaternion >( );
				this->PushChanges( System::Changes::Geometry::Orientation );
			}
		}
	}

	void GeometrySystemComponent::PushChanges( const unsigned int& systemChanges )
	{
		for( ObserverList::iterator i = m_observers.begin( ); i != m_observers.end( ); ++i )
		{
			( *i )->Observe( this, systemChanges );
		}
	}

	void GeometrySystemComponent::Observe( ISubject* subject, const unsigned int& systemChanges )
	{
		ISystemComponent* component = static_cast< ISystemComponent* >( subject );

		m_position = component->GetPosition( );
		m_orientation = component->GetOrientation( );

		this->PushChanges( 
			System::Changes::Geometry::Position |
			System::Changes::Geometry::Orientation
			);
	}
};