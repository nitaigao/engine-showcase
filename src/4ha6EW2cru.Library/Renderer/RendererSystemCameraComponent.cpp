#include "RendererSystemCameraComponent.h"

using namespace Maths;

#include <Ogre.h>
using namespace Ogre;

#include "../Logging/Logger.h"
using namespace Logging;

namespace Renderer
{
	AnyValue RendererSystemCameraComponent::Message( const unsigned int& messageId, AnyValue::AnyValueKeyMap parameters )
	{
		RendererSystemComponent::Message( messageId, parameters );

		if( messageId & System::Messages::SetOrientation )
		{
			m_sceneNode->setOrientation( parameters[ System::Attributes::Orientation ].GetValue< MathQuaternion >( ).AsOgreQuaternion( ) );
		}

		if ( messageId & System::Messages::Mouse_Moved )
		{
			m_xHistory.pop_back( );
			m_xHistory.push_front( parameters[ System::Attributes::DeltaX ].GetValue< int >( ) );

			m_yHistory.pop_back( );
			m_yHistory.push_front( parameters[ System::Attributes::DeltaY ].GetValue< int >( ) );
		}

		return AnyValue( );
	}

	void RendererSystemCameraComponent::Update( const float& deltaMilliseconds )
	{
		RendererSystemComponent::Update( deltaMilliseconds );
		
		float pitchDelta = this->AverageInputHistory( m_yHistory, m_weightModifier );
		float pitch = m_cameraNode->getOrientation( ).getPitch( ).valueDegrees( );

		if ( pitch + pitchDelta < 90.0f && pitch + pitchDelta > -90.0f )
		{
			m_cameraNode->pitch( Degree( pitchDelta ) );
		}

		float yawDelta = this->AverageInputHistory( m_xHistory, m_weightModifier );
		m_sceneNode->yaw( Degree( -yawDelta ) );

		Matrix3 yawMatrix;
		m_sceneNode->getOrientation( ).ToRotationMatrix( yawMatrix ); 

		Matrix3 pitchmatrix;
		m_cameraNode->getOrientation( ).ToRotationMatrix( pitchmatrix );

		Ogre::Vector3 forward = MathVector3::Forward( ).AsOgreVector3( );
		forward = forward * pitchmatrix.Inverse( );
		forward = forward * yawMatrix.Inverse( );
		
		m_attributes[ System::Attributes::LookAt ] = MathVector3( m_sceneNode->getPosition( ) + forward );
		this->PushMessage( System::Messages::SetLookAt, m_attributes );

		m_attributes[ System::Attributes::Position ] = MathVector3( m_sceneNode->getPosition( ) );
		this->PushMessage( System::Messages::SetPosition, m_attributes );

		m_attributes[ System::Attributes::Orientation ] = MathQuaternion( m_sceneNode->getOrientation( ) );
		this->PushMessage( System::Messages::SetOrientation, m_attributes );

		m_attributes[ System::Attributes::PlayerPosition ] = MathVector3( m_sceneNode->getPosition( ) );
		this->PushMessage( System::Messages::SetPlayerPosition, m_attributes );
	}

	void RendererSystemCameraComponent::Initialize( )
	{
		for( int i = 0; i < m_historySize; i++ )
		{
			m_xHistory.push_front( 0.0f );
			m_yHistory.push_front( 0.0f );
		}

		m_sceneNode = m_scene->GetSceneManager( )->createSceneNode( m_name );

		std::stringstream cameraNodeName;
		cameraNodeName << m_name << "_camera";

		m_cameraNode = m_scene->GetSceneManager( )->createSceneNode( cameraNodeName.str( ) );
		m_sceneNode->addChild( m_cameraNode );

		this->LoadModel( m_cameraNode, m_attributes[ System::Attributes::Model ].GetValue< std::string >( ) );

		m_scene->GetSceneManager( )->getRootSceneNode( )->addChild( m_sceneNode );

		this->InitializeSceneNode( m_sceneNode );
	}

	void RendererSystemCameraComponent::InitializeSceneNode( Ogre::SceneNode* sceneNode )
	{
		RendererSystemComponent::InitializeSceneNode( sceneNode );

		SceneNode::ObjectIterator objects = sceneNode->getAttachedObjectIterator( );

		while( objects.hasMoreElements( ) )
		{
			MovableObject* object = objects.getNext( );

			if( object->getMovableType( ) == "Camera" )
			{
				Camera* camera = m_scene->GetSceneManager( )->getCamera( object->getName( ) );
				m_scene->GetSceneManager( )->getCurrentViewport( )->setCamera( camera );
			}
		}
	}

	float RendererSystemCameraComponent::AverageInputHistory( const InputHistory& inputHistory, const float& weightModifier )
	{
		int index = 0;
		float sum = 0.0f;

		for ( InputHistory::const_iterator i = inputHistory.begin( ); i != inputHistory.end( ); ++i )
		{
			sum += ( *i ) * pow( weightModifier, index++ );
		}

		return sum / m_historySize;
	}
}