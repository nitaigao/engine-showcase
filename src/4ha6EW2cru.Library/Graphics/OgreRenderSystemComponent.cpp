#include "OgreRenderSystemComponent.h"

#include "../Utility/OgreMax/OgreMaxModel.hpp"
using namespace OgreMax;

#include <Ogre.h>
using namespace Ogre;

#include "../Geometry/GeometrySystemComponent.h"

OgreRenderSystemComponent::~OgreRenderSystemComponent( )
{
	_scene->GetRoot( )->getSceneManager( "default" )->getRootSceneNode( )->removeAndDestroyChild( _name );
}

void OgreRenderSystemComponent::Initialize( SystemPropertyList properties )
{
	SceneManager* sceneManager = _scene->GetRoot( )->getSceneManager( "default" );
	RenderTarget* renderTarget = _scene->GetRoot( )->getRenderTarget( "Interactive View" );

	for ( SystemPropertyList::iterator i = properties.begin( ); i != properties.end( ); ++i )
	{
		if ( ( *i ).first == "model" || ( *i ).first == "camera" )
		{
			std::string modelPath = ( *i ).second.GetValue< std::string >( );

			OgreMaxModel* model = new OgreMaxModel( );
			model->Load( modelPath );

			SceneNode* modelNode = sceneManager->createSceneNode( _name );
			model->CreateInstance( sceneManager, Ogre::StringUtil::BLANK, 0, 
				OgreMaxModel::NO_OPTIONS, 0, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, modelNode );

			sceneManager->getRootSceneNode( )->addChild( modelNode );

			SceneNode::ObjectIterator objects = modelNode->getAttachedObjectIterator( );

			while( objects.hasMoreElements( ) )
			{
				MovableObject* object = objects.getNext( );

				if( object->getMovableType( ) == "Camera" )
				{
					Camera* camera = sceneManager->getCamera( object->getName( ) );
					renderTarget->getViewport( 0 )->setCamera( camera );

					camera->setPosition( 0, 0, 0 );
					camera->lookAt( 0, 0, 0 );
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
	GeometrySystemComponent* geometryComponent = static_cast< GeometrySystemComponent* >( subject );

	SceneManager* sceneManager = _scene->GetRoot( )->getSceneManager( "default" );
	SceneNode* sceneNode = sceneManager->getSceneNode( _name );

	sceneNode->setPosition( geometryComponent->GetPosition( ).AsOgreVector3( ) );
	sceneNode->setScale( geometryComponent->GetScale( ).AsOgreVector3( ) );
	sceneNode->setOrientation( geometryComponent->GetOrientation( ).AsOgreQuaternion( ) );
}