#include "RendererSystemComponent.h"

#include "../Management/Management.h"

#include "AnimationBlender.h"
#include "IRendererSystem.hpp"

#include "../Maths/MathVector3.hpp"
#include "../Maths/MathQuaternion.hpp"
using namespace Maths;

#include "../Utility/OgreMax/OgreMaxModel.hpp"
using namespace OgreMax;
using namespace Ogre;

#include "../AI/AISystemComponent.h"
using namespace AI;

#include "../Logging/Logger.h"
using namespace Logging;

#include "AnimationController.h"

namespace Renderer
{
	RendererSystemComponent::~RendererSystemComponent( )
	{
		m_scene->GetSceneManager( )->getRootSceneNode( )->removeAndDestroyChild( m_name );

		for( IAnimationController::AnimationControllerMap::iterator i = m_animationControllers.begin( ); i != m_animationControllers.end( ); ++i )
		{
			delete ( *i ).second;
		}
	}

	void RendererSystemComponent::Initialize( )
	{
		m_sceneNode = m_scene->GetSceneManager( )->createSceneNode( m_name );

		this->LoadModel( m_sceneNode, m_attributes[ System::Parameters::Model ].As< std::string >( ) );

		m_scene->GetSceneManager( )->getRootSceneNode( )->addChild( m_sceneNode );

		this->InitializeSceneNode( m_sceneNode );
	}

	void RendererSystemComponent::LoadModel( Ogre::SceneNode* sceneNode, const std::string& modelPath )
	{
		OgreMaxModel* model = new OgreMaxModel( );

		try
		{
			std::stringstream prefix;
			prefix << m_name << "_";

			model->Load( modelPath );
			model->CreateInstance( 
				m_scene->GetSceneManager( ), prefix.str( ), 0, OgreMaxModel::NO_ANIMATION_STATES, 
				0, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, sceneNode 
				);
		}
		catch( Ogre::FileNotFoundException e )
		{
			Logger::Fatal( e.what( ) );
		}

		delete model;
	}

	void RendererSystemComponent::Destroy( )
	{
		this->DestroySceneNode( m_sceneNode );
	}

	AnyType RendererSystemComponent::PushMessage( const System::Message& message, AnyType::AnyTypeMap parameters )
	{
		return m_observer->Message( message, parameters );
	}

	AnyType RendererSystemComponent::Message( const System::Message& message, AnyType::AnyTypeMap parameters )
	{
		if ( message == System::Messages::SetPosition )
		{
			m_sceneNode->setPosition( parameters[ System::Attributes::Position ].As< MathVector3 >( ).AsOgreVector3( ) );
		}

		if ( message == System::Messages::SetOrientation )
		{
			m_sceneNode->setOrientation( parameters[ System::Attributes::Orientation ].As< MathQuaternion >( ).AsOgreQuaternion( ) );
		}

		if ( message == System::Messages::GetAnimationState )
		{
			Entity* entity = this->FindSkeletonEntity( m_sceneNode );

			Ogre::Skeleton::BoneIterator boneIterator = entity->getSkeleton( )->getBoneIterator( );

			while( boneIterator.hasMoreElements( ) )
			{
				Ogre::Bone* oBone = boneIterator.getNext( );
				oBone->setManuallyControlled( true );

				Entity *cubeEntity = Root::getSingletonPtr( )->getSceneManager( "default" )->createEntity( oBone->getName( ) + "_cube_entity", "/data/entities/meshes/axes.mesh" );
				TagPoint* tagPoint = entity->attachObjectToBone( oBone->getName( ), cubeEntity );
				tagPoint->setScale( 0.01f, 0.01f, 0.01f );
			}
			
			return entity->getSkeleton( );
		}

		return AnyType( );
	}

	void RendererSystemComponent::InitializeSceneNode( Ogre::SceneNode* sceneNode )
	{
		SceneNode::ObjectIterator objects = sceneNode->getAttachedObjectIterator( );

		while( objects.hasMoreElements( ) )
		{
			MovableObject* object = objects.getNext( );

			if( object->getMovableType( ) == "Entity" )
			{
				Entity* entity = m_scene->GetSceneManager( )->getEntity( object->getName( ) );

				AnimationStateSet* animationStates = entity->getAllAnimationStates( );

				if ( animationStates != 0 )
				{
					AnimationStateIterator animationStateIterator = animationStates->getAnimationStateIterator( );

					while ( animationStateIterator.hasMoreElements( ) )
					{
						AnimationState* animationState = animationStateIterator.getNext( );

						if ( m_animationControllers.find( animationState->getAnimationName( ) ) == m_animationControllers.end( ) )
						{
							m_animationControllers[ animationState->getAnimationName( ) ] = new AnimationController( animationState->getAnimationName( ) );
						}
						
						m_animationControllers[ animationState->getAnimationName( ) ]->AddAnimationState( animationState );
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
				Entity* entity = m_scene->GetSceneManager( )->getEntity( object->getName( ) );
				
				Ogre::Entity::ChildObjectListIterator childObjects = entity->getAttachedObjectIterator( );

				while( childObjects.hasMoreElements( ) )
				{
					m_scene->GetSceneManager( )->destroyMovableObject( childObjects.getNext( ) );
				}
			}

			m_scene->GetSceneManager( )->destroyMovableObject( object );
		}

		sceneNode->removeAndDestroyAllChildren( );
	}

	Ogre::Entity* RendererSystemComponent::FindSkeletonEntity( Ogre::SceneNode* sceneNode )
	{
		SceneNode::ObjectIterator objects = sceneNode->getAttachedObjectIterator( );

		while( objects.hasMoreElements( ) )
		{
			MovableObject* object = objects.getNext( );

			if( object->getMovableType( ) == "Entity" )
			{
				Entity* entity = m_scene->GetSceneManager( )->getEntity( object->getName( ) );

				if ( entity->getSkeleton( ) != 0 )
				{
					return entity;
				}
			}
		}

		Node::ChildNodeIterator children = sceneNode->getChildIterator( );

		while( children.hasMoreElements( ) )
		{
			SceneNode* child = static_cast< SceneNode* >( children.getNext( ) );
			Entity* entity = this->FindSkeletonEntity( child );

			if ( entity )
			{
				return entity;
			}
		}

		return 0;
	}
}