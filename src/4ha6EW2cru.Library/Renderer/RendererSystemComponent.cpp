#include "RendererSystemComponent.h"

#include "../Management/Management.h"

#include "../Utility/OgreMax/OgreMaxModel.hpp"
using namespace OgreMax;
using namespace Ogre;

#include "../AI/AISystemComponent.h"
using namespace AI;

#include "../Logging/Logger.h"
using namespace Logging;

#include "AnimationBlender.h"
#include "IRendererSystem.hpp"

namespace Renderer
{
	RendererSystemComponent::~RendererSystemComponent( )
	{
		_scene->GetSceneManager( )->getRootSceneNode( )->removeAndDestroyChild( _name );

		for( AnimationBlenderList::iterator i = _animationBlenders.begin( ); i != _animationBlenders.end( ); ++i )
		{
			delete ( *i );
		}
	}

	void RendererSystemComponent::Initialize( AnyValue::AnyValueMap& properties )
	{
		_sceneNode = _scene->GetSceneManager( )->createSceneNode( _name );

		this->LoadModel( _sceneNode, properties[ "model" ].GetValue< std::string >( ) );

		_scene->GetSceneManager( )->getRootSceneNode( )->addChild( _sceneNode );

		this->InitializeSceneNode( _sceneNode );
	}

	void RendererSystemComponent::LoadModel( Ogre::SceneNode* sceneNode, const std::string& modelPath )
	{
		OgreMaxModel* model = new OgreMaxModel( );

		try
		{
			std::stringstream prefix;
			prefix << _name << "_";

			model->Load( modelPath );
			model->CreateInstance( _scene->GetSceneManager( ), prefix.str( ), 0, 
				OgreMaxModel::NO_OPTIONS, 0, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, sceneNode );
		}
		catch( Ogre::FileNotFoundException e )
		{
			Logger::Fatal( e.what( ) );
		}

		delete model;
	}

	void RendererSystemComponent::Destroy( )
	{
		this->DestroySceneNode( _sceneNode );
	}

	void RendererSystemComponent::Observe( ISubject* subject, const unsigned int& systemChanges )
	{
		ISystemComponent* component = static_cast< ISystemComponent* >( subject );
		
		if ( System::Changes::Geometry::Position & systemChanges )
		{
			_sceneNode->setPosition( component->GetPosition( ).AsOgreVector3( ) );
		}
		
		if ( System::Changes::Geometry::Orientation & systemChanges )
		{
			_sceneNode->setOrientation( component->GetOrientation( ).AsOgreQuaternion( ) );
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

	void RendererSystemComponent::Update( const float& deltaMilliseconds )
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
}