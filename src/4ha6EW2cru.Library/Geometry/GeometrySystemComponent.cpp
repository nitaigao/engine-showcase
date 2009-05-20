#include "GeometrySystemComponent.h"

using namespace Maths;

namespace Geometry
{
	AnyValue GeometrySystemComponent::Message( const unsigned int& messageId, AnyValue::AnyValueKeyMap parameters )
	{
		if ( messageId & System::Messages::SetPosition )
		{
			m_attributes[ System::Attributes::Position ] = parameters[ System::Attributes::Position ].GetValue< MathVector3 >( );
		}

		if ( messageId & System::Messages::SetOrientation )
		{
			m_attributes[ System::Attributes::Orientation ] = parameters[ System::Attributes::Orientation ].GetValue< MathQuaternion >( );
		}

		if ( messageId & System::Messages::AddedToComponent )
		{
			this->PushMessage( System::Messages::SetPosition | System::Messages::SetOrientation, m_attributes );
		}

		return MathVector3( );
	}

	AnyValue GeometrySystemComponent::PushMessage( const unsigned int& messageId, AnyValue::AnyValueKeyMap parameters )
	{
		for( ObserverList::iterator i = m_observers.begin( ); i != m_observers.end( ); ++i )
		{
			( *i )->Message( System::Messages::SetPosition, m_attributes );
		}

		return AnyValue( );
	}
};