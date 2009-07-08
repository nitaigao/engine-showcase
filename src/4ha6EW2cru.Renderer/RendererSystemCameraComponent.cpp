#include "RendererSystemCameraComponent.h"

using namespace Maths;

#include <Ogre.h>
using namespace Ogre;

#include "Logging/Logger.h"
using namespace Logging;

namespace Renderer
{
	AnyType RendererSystemCameraComponent::Message( const System::Message& message, AnyType::AnyTypeMap parameters )
	{
		AnyType result = RendererSystemComponent::Message( message, parameters );

		if ( message == System::Messages::Mouse_Moved )
		{
			m_attributes[ System::Parameters::DeltaX ] = parameters[ System::Parameters::DeltaX ].As< float >( );
			m_attributes[ System::Parameters::DeltaY ] = parameters[ System::Parameters::DeltaY ].As< float >( );
		}

		return result;
	}

	void RendererSystemCameraComponent::Update( const float& deltaMilliseconds )
	{
		RendererSystemComponent::Update( deltaMilliseconds );

		float pitchDelta = m_attributes[ System::Parameters::DeltaY ].As< float >( );
		float pitch = m_cameraNode->getOrientation( ).getPitch( ).valueDegrees( );

		if( pitch + -pitchDelta < 90.0f && pitch + -pitchDelta > -90.0f )
		{
			m_cameraNode->pitch( Degree( -pitchDelta ) );
		} 

		float yawDelta = m_attributes[ System::Parameters::DeltaX ].As< float >( );
		m_sceneNode->yaw( Degree( -yawDelta ) );

		Matrix3 yawMatrix;
		m_sceneNode->getOrientation( ).ToRotationMatrix( yawMatrix ); 

		Matrix3 pitchmatrix;
		m_cameraNode->getOrientation( ).ToRotationMatrix( pitchmatrix );

		Ogre::Vector3 forward = MathVector3::Forward( ).AsOgreVector3( );
		forward = forward * pitchmatrix.Inverse( );
		forward = forward * yawMatrix.Inverse( );

		Camera* camera = m_scene->GetSceneManager( )->getCurrentViewport( )->getCamera( );

		Ray ray = camera->getCameraToViewportRay( 
			m_scene->GetSceneManager( )->getCurrentViewport( )->getWidth( ) / 2,
			m_scene->GetSceneManager( )->getCurrentViewport( )->getHeight( ) / 2
			);

		Ogre::Vector3 poi = ray.getPoint( 10.0f ) - ray.getOrigin( ); 
		Vector3 localPoi = m_sceneNode->getOrientation( ).Inverse( ) * poi; 

		SceneNode* axisNode = m_scene->GetSceneManager( )->getSceneNode( "poi_axis" );
		axisNode->setPosition( ray.getPoint( 10.0f ) );

		m_attributes[ System::Attributes::POI ] = MathVector3( localPoi );
		m_attributes[ System::Attributes::LookAt ] = MathVector3( m_sceneNode->getPosition( ) + forward );
		m_attributes[ System::Attributes::Position ] = MathVector3( m_sceneNode->getPosition( ) );
		m_attributes[ System::Attributes::Orientation ] = MathQuaternion( m_sceneNode->getOrientation( ) );
		m_attributes[ System::Parameters::PlayerPosition ] = MathVector3( m_sceneNode->getPosition( ) );

		if ( yawDelta != 0.0f )
		{
			this->PushMessage( System::Messages::SetOrientation, m_attributes );
		}

		if ( yawDelta != 0.0f || pitchDelta != 0.0f )
		{
			this->PushMessage( System::Messages::SetLookAt, m_attributes );
		}

		//this->PushMessage( System::Messages::SetPosition, m_attributes );
		//this->PushMessage( System::Messages::SetPlayerPosition, m_attributes );
	}

	void RendererSystemCameraComponent::Initialize( )
	{
		m_attributes[ System::Parameters::DeltaY ] = 0.0f;
		m_attributes[ System::Parameters::DeltaX ] = 0.0f;

		m_sceneNode = m_scene->GetSceneManager( )->createSceneNode( m_name );
		m_scene->GetSceneManager( )->getRootSceneNode( )->addChild( m_sceneNode );

		std::stringstream cameraNodeName;
		cameraNodeName << m_name << "_cameraNode";

		m_cameraNode = m_sceneNode->createChildSceneNode( cameraNodeName.str( ) );

		std::stringstream cameraName;
		cameraName << m_name << "_camera";

		Camera* camera = m_scene->GetSceneManager( )->createCamera( cameraName.str( ) );
		camera->setNearClipDistance( 0.14f );
		camera->setFOVy( Radian( 1.04463f ) );
		
		m_cameraNode->attachObject( camera );
		Vector3 cameraOffset( 0.0f, 1.67141f, -0.3f );
		m_cameraNode->setPosition( cameraOffset );

		m_scene->GetSceneManager( )->getCurrentViewport( )->setCamera( camera );

		std::stringstream modelNodeName;
		modelNodeName << m_name << "_modelNode";

		SceneNode* modelNode = m_cameraNode->createChildSceneNode( modelNodeName.str( ) );
		this->LoadModel( modelNode, m_attributes[ System::Parameters::Model ].As< std::string >( ) );
		modelNode->setPosition( modelNode->getPosition( ) - cameraOffset );

		this->InitializeSceneNode( m_sceneNode );

		Entity *axis = m_scene->GetSceneManager( )->createEntity( "poi_axis", "/data/entities/meshes/axes.mesh" );
		SceneNode* axisNode = m_scene->GetSceneManager( )->createSceneNode( "poi_axis" );
		axisNode->attachObject( axis );
		axisNode->setScale( 0.005f, 0.005f, 0.005f );
		m_scene->GetSceneManager( )->getRootSceneNode()->addChild( axisNode );
	}
}