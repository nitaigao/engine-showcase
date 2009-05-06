#include "RendererSystemCameraComponent.h"

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

		if ( System::Changes::Input::Mouse_Moved & systemChanges )
		{
			float mouseXDelta = component->GetProperties( )[ "mouseXDelta" ].GetValue< int >( );
			_xHistory.pop_back( );
			_xHistory.push_front( mouseXDelta );

			float mouseYDelta = component->GetProperties( )[ "mouseYDelta" ].GetValue< int >( );
			_yHistory.pop_back( );
			_yHistory.push_front( mouseYDelta );
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

		if ( _scene->GetSceneManager( )->hasSceneNode( _cameraNode->getName( ) ) )
		{
			float xResult = this->AverageInputHistory( _xHistory, _weightModifier );
			float yResult = this->AverageInputHistory( _yHistory, _weightModifier );

			float pitch = _cameraNode->getOrientation( ).getPitch( ).valueDegrees( );

			if ( pitch + yResult < 90.0f && pitch + yResult > -90.0f )
			{
				_cameraNode->pitch( Degree( yResult ) );
			}

			_sceneNode->yaw( Degree( -xResult ), Node::TS_PARENT );

			_xHistory.pop_back( );
			_xHistory.push_front( 0.0f );

			_yHistory.pop_back( );
			_yHistory.push_front( 0.0f );

			this->PushChanges( System::Changes::Geometry::Orientation );
		}
	}

	void RendererSystemCameraComponent::Initialize( AnyValue::AnyValueMap& properties )
	{
		_sceneNode = _scene->GetSceneManager( )->createSceneNode( _name );

		std::stringstream cameraNodeName;
		cameraNodeName << _name << "_camera";

		_cameraNode = _scene->GetSceneManager( )->createSceneNode( cameraNodeName.str( ) );
		_sceneNode->addChild( _cameraNode );

		this->LoadModel( _cameraNode, properties[ "model" ].GetValue< std::string >( ) );

		_scene->GetSceneManager( )->getRootSceneNode( )->addChild( _sceneNode );

		this->InitializeSceneNode( _sceneNode );

		for( int i = 0; i < _historySize; i++ )
		{
			_xHistory.push_front( 0.0f );
			_yHistory.push_front( 0.0f );
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
}