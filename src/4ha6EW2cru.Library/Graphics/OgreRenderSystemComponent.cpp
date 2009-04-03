#include "OgreRenderSystemComponent.h"

#include "../Utility/OgreMax/OgreMaxModel.hpp"
using namespace OgreMax;

#include <Ogre.h>
using namespace Ogre;

#include "../Geometry/GeometrySystemComponent.h"
#include "../Input/InputSystemComponent.h"
#include "../Logging/Logger.h"

OgreRenderSystemComponent::~OgreRenderSystemComponent( )
{
	SceneManager* sceneManager = _scene->GetRoot( )->getSceneManager( "default" );
	SceneNode::ObjectIterator objects = _sceneNode->getAttachedObjectIterator( );

	while( objects.hasMoreElements( ) )
	{
		MovableObject* object = objects.getNext( );
		sceneManager->destroyMovableObject( object );
	}

	sceneManager->getRootSceneNode( )->removeAndDestroyChild( _name );
}

void OgreRenderSystemComponent::Initialize( AnyValueMap properties )
{
	SceneManager* sceneManager = _scene->GetRoot( )->getSceneManager( "default" );

	for ( AnyValueMap::iterator i = properties.begin( ); i != properties.end( ); ++i )
	{
		if ( ( *i ).first == "model" || ( *i ).first == "camera" )
		{
			std::string modelPath = ( *i ).second.GetValue< std::string >( );

			OgreMaxModel* model = new OgreMaxModel( );
			model->Load( modelPath );

			_sceneNode = sceneManager->createSceneNode( _name );
			model->CreateInstance( sceneManager, Ogre::StringUtil::BLANK, 0, 
				OgreMaxModel::NO_OPTIONS, 0, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, _sceneNode );

			sceneManager->getRootSceneNode( )->addChild( _sceneNode );

			SceneNode::ObjectIterator objects = _sceneNode->getAttachedObjectIterator( );

			while( objects.hasMoreElements( ) )
			{
				MovableObject* object = objects.getNext( );

				if( object->getMovableType( ) == "Camera" )
				{
					OgreRenderSystem* renderSystem = ( OgreRenderSystem* ) Management::GetInstance( )->GetSystemManager( )->GetSystem( RenderSystemType );

					renderSystem->SetProperty( "activeCamera", object->getName( ) );
					renderSystem->SetProperty( "farClip", 500.0f );
				}
			}

			delete model;
		}
	}
}

unsigned int OgreRenderSystemComponent::GetRequiredSystemChanges()
{
	return System::Changes::Geometry::Orientation
		| System::Changes::Geometry::Position
		| System::Changes::Geometry::Scale;

}

void OgreRenderSystemComponent::Observe( ISubject* subject, unsigned int systemChanges )
{
	float mouseSmooth = 0.15f;

	ISystemComponent* component = static_cast< ISystemComponent* >( subject );

	if ( component->GetType( ) == InputSystemType )
	{
		InputSystemComponent* inputComponent = static_cast< InputSystemComponent* >( component );

		if ( System::Changes::Input::Mouse_Moved & systemChanges )
		{
			float mouseYDelta = inputComponent->GetProperties( )[ "mouseYDelta" ].GetValue< int >( );

			SceneNode::ObjectIterator objects = _sceneNode->getAttachedObjectIterator( );

			while( objects.hasMoreElements( ) )
			{
				MovableObject* object = objects.getNext( );

				if( object->getMovableType( ) == "Camera" )
				{
					Camera* camera = _scene->GetRoot( )->getSceneManager( "default" )->getCamera( object->getName( ) );
					camera->pitch( Degree( mouseYDelta * mouseSmooth ) );
				}
			}
		}
	}

	if ( component->GetType( ) == GeometrySystemType )
	{
		GeometrySystemComponent* geometryComponent = static_cast< GeometrySystemComponent* >( subject );

		SceneManager* sceneManager = _scene->GetRoot( )->getSceneManager( "default" );
		SceneNode* sceneNode = sceneManager->getSceneNode( _name );

		SceneNode::ObjectIterator objects = _sceneNode->getAttachedObjectIterator( );

		while( objects.hasMoreElements( ) )
		{
			MovableObject* object = objects.getNext( );

			if( object->getMovableType( ) == "Camera" )
			{
				MathVector3 cameraPosition = geometryComponent->GetPosition( ) + MathVector3( 0.0f, 2.0f, 0.0f );
				sceneNode->setPosition( cameraPosition.AsOgreVector3( ) );
			}
			else
			{
				sceneNode->setPosition( geometryComponent->GetPosition( ).AsOgreVector3( ) );
			}
		}

		sceneNode->setScale( geometryComponent->GetScale( ).AsOgreVector3( ) );
		sceneNode->setOrientation( geometryComponent->GetOrientation( ).AsOgreQuaternion( ) );
	}
}