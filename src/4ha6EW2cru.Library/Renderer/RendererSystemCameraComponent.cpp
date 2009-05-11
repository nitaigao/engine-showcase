#include "RendererSystemCameraComponent.h"

using namespace Maths;

#include <Ogre.h>
using namespace Ogre;

#include "../Logging/Logger.h"
using namespace Logging;

namespace Renderer
{
	void RendererSystemCameraComponent::Observe( ISubject* subject, const unsigned int& systemChanges )
	{
		RendererSystemComponent::Observe( subject, systemChanges );

		ISystemComponent* component = static_cast< ISystemComponent* >( subject );

		if ( System::Changes::Geometry::Orientation & systemChanges )
		{
			_sceneNode->setOrientation( component->GetOrientation( ).AsOgreQuaternion( ) );
		}

		if ( System::Changes::Input::Mouse_Moved & systemChanges )
		{
			_xHistory.pop_back( );
			_xHistory.push_front( component->GetProperties( )[ "xDelta" ].GetValue< int >( ) );

			_yHistory.pop_back( );
			_yHistory.push_front( component->GetProperties( )[ "yDelta" ].GetValue< int >( ) );
		}
	}

	void RendererSystemCameraComponent::PushChanges( const unsigned int& systemChanges )
	{
		if ( _observer != 0 )
		{
			_observer->Observe( this, systemChanges );
		}
	}

	void RendererSystemCameraComponent::Update( const float& deltaMilliseconds )
	{
		RendererSystemComponent::Update( deltaMilliseconds );
		
		float pitchDelta = this->AverageInputHistory( _yHistory, _weightModifier );
		float pitch = _cameraNode->getOrientation( ).getPitch( ).valueDegrees( );

		if ( pitch + pitchDelta < 90.0f && pitch + pitchDelta > -90.0f )
		{
			_cameraNode->pitch( Degree( pitchDelta ) );
		}

		float yawDelta = this->AverageInputHistory( _xHistory, _weightModifier );
		_sceneNode->yaw( Degree( -yawDelta ) );

		Matrix3 yawMatrix;
		_sceneNode->getOrientation( ).ToRotationMatrix( yawMatrix ); 

		Matrix3 pitchmatrix;
		_cameraNode->getOrientation( ).ToRotationMatrix( pitchmatrix );

		Ogre::Vector3 forward = MathVector3::Forward( ).AsOgreVector3( );
		forward = forward * pitchmatrix.Inverse( );
		forward = forward * yawMatrix.Inverse( );
		
		_properties[ "lookAt" ] = MathVector3( _sceneNode->getPosition( ) + forward );

		this->PushChanges( System::Changes::Geometry::Orientation | System::Changes::POI::LookAt );
	}

	void RendererSystemCameraComponent::Initialize( AnyValue::AnyValueMap& properties )
	{
		for( int i = 0; i < _historySize; i++ )
		{
			_xHistory.push_front( 0.0f );
			_yHistory.push_front( 0.0f );
		}

		_sceneNode = _scene->GetSceneManager( )->createSceneNode( _name );

		std::stringstream cameraNodeName;
		cameraNodeName << _name << "_camera";

		_cameraNode = _scene->GetSceneManager( )->createSceneNode( cameraNodeName.str( ) );
		_sceneNode->addChild( _cameraNode );

		this->LoadModel( _cameraNode, properties[ "model" ].GetValue< std::string >( ) );

		_scene->GetSceneManager( )->getRootSceneNode( )->addChild( _sceneNode );

		this->InitializeSceneNode( _sceneNode );
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
				Camera* camera = _scene->GetSceneManager( )->getCamera( object->getName( ) );
				_scene->GetSceneManager( )->getCurrentViewport( )->setCamera( camera );
			}
		}
	}

	float RendererSystemCameraComponent::AverageInputHistory( const History& inputHistory, const float& weightModifier )
	{
		int index = 0;
		float sum = 0.0f;

		for ( History::const_iterator i = inputHistory.begin( ); i != inputHistory.end( ); ++i )
		{
			sum += ( *i ) * pow( weightModifier, index++ );
		}

		return sum / _historySize;
	}
}