#include "RendererSystemCameraComponent.h"

using namespace Maths;

#include <Ogre.h>
using namespace Ogre;

#include "../Logging/Logger.h"
using namespace Logging;

namespace Renderer
{
	AnyType RendererSystemCameraComponent::Message( const System::Message& message, AnyType::AnyTypeMap parameters )
	{
		RendererSystemComponent::Message( message, parameters );

		if( message == System::Messages::SetOrientation )
		{
			m_sceneNode->setOrientation( parameters[ System::Attributes::Orientation ].As< MathQuaternion >( ).AsOgreQuaternion( ) );
		}

		if ( message == System::Messages::Mouse_Moved )
		{
			m_attributes[ System::Attributes::DeltaX ] = parameters[ System::Attributes::DeltaX ].As< float >( );
			m_attributes[ System::Attributes::DeltaY ] = parameters[ System::Attributes::DeltaY ].As< float >( );
		}

		return AnyType( );
	}

	void RendererSystemCameraComponent::Update( const float& deltaMilliseconds )
	{
		RendererSystemComponent::Update( deltaMilliseconds );

		float pitchDelta = m_attributes[ System::Attributes::DeltaY ].As< float >( );
		float pitch = m_cameraNode->getOrientation( ).getPitch( ).valueDegrees( );

		if ( pitch + pitchDelta < 90.0f && pitch + pitchDelta > -90.0f )
		{
			m_cameraNode->pitch( Degree( -pitchDelta ) );
		}

		float yawDelta = m_attributes[ System::Attributes::DeltaX ].As< float >( );
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
		m_sceneNode = m_scene->GetSceneManager( )->createSceneNode( m_name );

		m_attributes[ System::Attributes::DeltaY ] = 0.0f;
		m_attributes[ System::Attributes::DeltaX ] = 0.0f;

		std::stringstream cameraNodeName;
		cameraNodeName << m_name << "_camera";

		m_cameraNode = m_scene->GetSceneManager( )->createSceneNode( cameraNodeName.str( ) );
		m_sceneNode->addChild( m_cameraNode );

		this->LoadModel( m_cameraNode, m_attributes[ System::Attributes::Model ].As< std::string >( ) );

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
}