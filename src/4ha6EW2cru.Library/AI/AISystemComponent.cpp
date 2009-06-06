#include "AISystemComponent.h"

using namespace Maths;

namespace AI
{
	AnyType AISystemComponent::PushMessage( const System::Message& message, AnyType::AnyTypeMap parameters )
	{
		return m_observer->Message( message, parameters );
	}

	AnyType AISystemComponent::Message( const System::Message& message, AnyType::AnyTypeMap parameters )
	{
 		if ( message == System::Messages::SetPlayerPosition )
		{
			MathVector3 playerPosition = parameters[ System::Attributes::Position ].As< MathVector3 >( );
			
			m_attributes[ System::Parameters::PlayerPosition ] = playerPosition;
			m_attributes[ System::Parameters::PlayerOrientation ] = parameters[ System::Attributes::Orientation ].As< MathQuaternion >( );

			m_playerDistance = ( ( playerPosition - m_attributes[ System::Attributes::Position ].As< MathVector3 >( ) ) ).Length( );
		}

		if ( message == System::Messages::SetPosition )
		{
			m_attributes[ System::Attributes::Position ] = parameters[ System::Attributes::Position ].As< MathVector3 >( );
		}

		if ( message == System::Messages::SetOrientation )
		{
			m_attributes[ System::Attributes::Orientation ] = parameters[ System::Attributes::Orientation ].As< MathQuaternion >( );
		}

		return AnyType( );
	}
}