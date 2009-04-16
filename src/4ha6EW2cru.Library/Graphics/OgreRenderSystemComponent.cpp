#include "OgreRenderSystemComponent.h"

#include "../Utility/OgreMax/OgreMaxModel.hpp"
using namespace OgreMax;

#include <Ogre.h>
using namespace Ogre;

#include "../Geometry/GeometrySystemComponent.h"
#include "../Input/InputSystemComponent.h"
#include "../AI/AISystemComponent.h"
#include "../Logging/Logger.h"

OgreRenderSystemComponent::~OgreRenderSystemComponent( )
{
	_scene->GetSceneManager( )->getRootSceneNode( )->removeAndDestroyChild( _name );
}

void OgreRenderSystemComponent::Initialize( AnyValueMap properties )
{
	for ( AnyValueMap::iterator i = properties.begin( ); i != properties.end( ); ++i )
	{
		if ( ( *i ).first == "model" || ( *i ).first == "camera" )
		{
			std::string modelPath = ( *i ).second.GetValue< std::string >( );

			_sceneNode = _scene->GetSceneManager( )->createSceneNode( _name );

			OgreMaxModel* model = new OgreMaxModel( );

			try
			{
				model->Load( modelPath );
				model->CreateInstance( _scene->GetSceneManager( ), _name, 0, 
					OgreMaxModel::NO_OPTIONS, 0, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, _sceneNode );
			}
			catch( Ogre::FileNotFoundException e )
			{
				Logger::GetInstance( )->Fatal( e.what( ) );
			}
				
			delete model;

			_scene->GetSceneManager( )->getRootSceneNode( )->addChild( _sceneNode );

			this->InitializeSceneNode( _sceneNode );
		}
	}
}

void OgreRenderSystemComponent::Destroy( )
{
	this->DestroySceneNode( _sceneNode );
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
					Camera* camera = _scene->GetSceneManager( )->getCamera( object->getName( ) );
					camera->pitch( Degree( mouseYDelta * mouseSmooth ) );
				}
			}
		}
	}

	if ( component->GetType( ) == GeometrySystemType )
	{
		GeometrySystemComponent* geometryComponent = static_cast< GeometrySystemComponent* >( subject );

		SceneNode* sceneNode = _scene->GetSceneManager( )->getSceneNode( _name );

		sceneNode->setPosition( geometryComponent->GetPosition( ).AsOgreVector3( ) );
		sceneNode->setScale( geometryComponent->GetScale( ).AsOgreVector3( ) );
		sceneNode->setOrientation( geometryComponent->GetOrientation( ).AsOgreQuaternion( ) );
	}

	if ( component->GetType( ) == AISystemType )
	{
		if ( System::Changes::AI::Behavior & systemChanges )
		{
			AISystemComponent* aiComponent = static_cast< AISystemComponent* >( subject );

			std::vector< AnimationState* > statesForEnable;

			for( AnimationStateList::iterator i = _animationStates.begin( ); i != _animationStates.end( ); ++i )
			{
				if ( ( *i )->getAnimationName( ) == aiComponent->GetBehavior( ) )
				{
					( *i )->setEnabled( true );
				}
			}
		}
	}
}

void OgreRenderSystemComponent::Update( float deltaMilliseconds )
{
	for( AnimationStateList::iterator i = _animationStates.begin( ); i != _animationStates.end( ); ++i )
	{
		( *i )->addTime( deltaMilliseconds );
	}
}

void OgreRenderSystemComponent::InitializeSceneNode( Ogre::SceneNode* sceneNode )
{
	SceneNode::ObjectIterator objects = sceneNode->getAttachedObjectIterator( );

	while( objects.hasMoreElements( ) )
	{
		MovableObject* object = objects.getNext( );

		if( object->getMovableType( ) == "Camera" )
		{
			OgreRenderSystem* renderSystem = ( OgreRenderSystem* ) Management::GetInstance( )->GetSystemManager( )->GetSystem( RenderSystemType );

			renderSystem->SetProperty( "activeCamera", object->getName( ) );
			renderSystem->SetProperty( "farClip", 500.0f );
		}

		if( object->getMovableType( ) == "Entity" )
		{
			Entity* entity = _scene->GetSceneManager( )->getEntity( object->getName( ) );

			if ( entity->getAllAnimationStates( ) != 0 )
			{
				AnimationStateIterator animationStates = entity->getAllAnimationStates( )->getAnimationStateIterator( );

				while( animationStates.hasMoreElements( ) )
				{
					_animationStates.push_back(  animationStates.getNext( ) );
				}
			}
		}
	}

	Ogre::Node::ChildNodeIterator children = sceneNode->getChildIterator( );

	while( children.hasMoreElements( ) )
	{
		SceneNode* childSceneNode = static_cast< SceneNode* >( children.getNext( ) );
		this->InitializeSceneNode( childSceneNode );
	}
}

void OgreRenderSystemComponent::DestroySceneNode( Ogre::SceneNode* sceneNode )
{
	Ogre::Node::ChildNodeIterator children = sceneNode->getChildIterator( );

	while( children.hasMoreElements( ) )
	{
		SceneNode* childSceneNode = static_cast< SceneNode* >( children.getNext( ) );
		this->DestroySceneNode( childSceneNode );
	}

	SceneNode::ObjectIterator objects = sceneNode->getAttachedObjectIterator( );

	while( objects.hasMoreElements( ) )
	{
		MovableObject* object = objects.getNext( );

		if( object->getMovableType( ) == "Entity" )
		{
			Entity* entity = _scene->GetSceneManager( )->getEntity( object->getName( ) );
			
			Ogre::Entity::ChildObjectListIterator childObjects = entity->getAttachedObjectIterator( );

			while( childObjects.hasMoreElements( ) )
			{
				_scene->GetSceneManager( )->destroyMovableObject( childObjects.getNext( ) );
			}
		}

		_scene->GetSceneManager( )->destroyMovableObject( object );
	}

	sceneNode->removeAndDestroyAllChildren( );
}