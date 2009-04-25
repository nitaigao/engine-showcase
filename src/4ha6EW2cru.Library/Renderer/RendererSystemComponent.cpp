#include "RendererSystemComponent.h"

#include "../Utility/OgreMax/OgreMaxModel.hpp"
using namespace OgreMax;

#include <Ogre.h>
using namespace Ogre;

#include "../Geometry/GeometrySystemComponent.h"
#include "../Input/InputSystemComponent.h"
#include "../AI/AISystemComponent.h"
#include "../Logging/Logger.h"

#include "AnimationBlender.h"

#include "../Scripting/ScriptEvent.hpp"

RendererSystemComponent::~RendererSystemComponent( )
{
	_scene->GetSceneManager( )->getRootSceneNode( )->removeAndDestroyChild( _name );

	for( AnimationBlenderList::iterator i = _animationBlenders.begin( ); i != _animationBlenders.end( ); ++i )
	{
		delete ( *i );
	}
}

void RendererSystemComponent::Initialize( AnyValueMap properties )
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
				std::stringstream prefix;
				prefix << _name << "_";

				model->Load( modelPath );
				model->CreateInstance( _scene->GetSceneManager( ), prefix.str( ), 0, 
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

void RendererSystemComponent::Destroy( )
{
	this->DestroySceneNode( _sceneNode );
}

void RendererSystemComponent::Observe( ISubject* subject, unsigned int systemChanges )
{
	ISystemComponent* component = static_cast< ISystemComponent* >( subject );

	SceneNode* sceneNode = _scene->GetSceneManager( )->getSceneNode( _name );
	
	if ( System::Changes::Geometry::Position & systemChanges || System::Changes::Geometry::Orientation & systemChanges )
	{
		sceneNode->setPosition( component->GetPosition( ).AsOgreVector3( ) );
		sceneNode->setScale( component->GetScale( ).AsOgreVector3( ) );
		sceneNode->setOrientation( component->GetOrientation( ).AsOgreQuaternion( ) );
	}

	if ( System::Changes::AI::Behavior & systemChanges )
	{
		AISystemComponent* aiComponent = static_cast< AISystemComponent* >( subject );

		for( AnimationBlenderList::iterator i = _animationBlenders.begin( ); i != _animationBlenders.end( ); ++i )
		{
			this->PlayAnimation( aiComponent->GetBehavior( ), true );
		}
	}
}

void RendererSystemComponent::Update( float deltaMilliseconds )
{
	for( AnimationBlenderList::iterator i = _animationBlenders.begin( ); i != _animationBlenders.end( ); ++i )
	{
		( *i )->Update( deltaMilliseconds );
	}
}

void RendererSystemComponent::InitializeSceneNode( Ogre::SceneNode* sceneNode )
{
	SceneNode::ObjectIterator objects = sceneNode->getAttachedObjectIterator( );

	while( objects.hasMoreElements( ) )
	{
		MovableObject* object = objects.getNext( );

		if( object->getMovableType( ) == "Camera" )
		{
			RendererSystem* renderSystem = ( RendererSystem* ) Management::GetInstance( )->GetSystemManager( )->GetSystem( RenderSystemType );
			renderSystem->SetProperty( "activeCamera", object->getName( ) );
		}

		if( object->getMovableType( ) == "Entity" )
		{
			Entity* entity = _scene->GetSceneManager( )->getEntity( object->getName( ) );

			AnimationStateSet* animationStates = entity->getAllAnimationStates( );

			if ( animationStates != 0 )
			{
				IAnimationBlender* animationBlender = new AnimationBlender( entity );
				_animationBlenders.push_back( animationBlender );
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

void RendererSystemComponent::DestroySceneNode( Ogre::SceneNode* sceneNode )
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

void RendererSystemComponent::PlayAnimation( const std::string& animationName, const bool& loopAnimation )
{
	for( AnimationBlenderList::iterator i = _animationBlenders.begin( ); i != _animationBlenders.end( ); ++i )
	{
		( *i )->Blend( animationName, 0.2f, loopAnimation );
	}
}