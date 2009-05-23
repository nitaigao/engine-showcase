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

namespace Renderer
{
	RendererSystemComponent::~RendererSystemComponent( )
	{
		m_scene->GetSceneManager( )->getRootSceneNode( )->removeAndDestroyChild( m_name );

		for( IAnimationBlender::AnimationBlenderList::iterator i = m_animationBlenders.begin( ); i != m_animationBlenders.end( ); ++i )
		{
			delete ( *i );
		}
	}

	void RendererSystemComponent::Initialize( )
	{
		m_sceneNode = m_scene->GetSceneManager( )->createSceneNode( m_name );

		this->LoadModel( m_sceneNode, m_attributes[ System::Attributes::Model ].As< std::string >( ) );

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
			model->CreateInstance( m_scene->GetSceneManager( ), prefix.str( ), 0, 
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
		this->DestroySceneNode( m_sceneNode );
	}

	AnyType RendererSystemComponent::PushMessage( const System::Message& message, AnyType::AnyTypeKeyMap parameters )
	{
		return m_observer->Message( message, parameters );
	}

	AnyType RendererSystemComponent::Message( const System::Message& message, AnyType::AnyTypeKeyMap parameters )
	{
		if ( message == System::Messages::SetPosition )
		{
			m_sceneNode->setPosition( parameters[ System::Attributes::Position ].As< MathVector3 >( ).AsOgreVector3( ) );
		}

		if ( message == System::Messages::SetOrientation )
		{
			m_sceneNode->setOrientation( parameters[ System::Attributes::Orientation ].As< MathQuaternion >( ).AsOgreQuaternion( ) );
		}

		return AnyType( );
	}

	void RendererSystemComponent::Update( const float& deltaMilliseconds )
	{
		for( IAnimationBlender::AnimationBlenderList::iterator i = m_animationBlenders.begin( ); i != m_animationBlenders.end( ); ++i )
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
				Entity* entity = m_scene->GetSceneManager( )->getEntity( object->getName( ) );

				AnimationStateSet* animationStates = entity->getAllAnimationStates( );

				if ( animationStates != 0 )
				{
					IAnimationBlender* animationBlender = new AnimationBlender( entity );
					m_animationBlenders.push_back( animationBlender );
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

	void RendererSystemComponent::PlayAnimation( const std::string& animationName, const bool& loopAnimation )
	{
		if ( animationName == "hit" )
		{
			std::stringstream gunfireName;
			gunfireName << m_name << "_gunfire";

			Entity* gunfire = static_cast< Entity* >( m_scene->GetSceneManager( )->getMovableObject( gunfireName.str( ), "Entity" ) );
			gunfire->setVisible( true );

			if ( rand( ) % 2 == 1 )
			{
				gunfire->getParentNode()->roll( Degree( 91 ) );
			}
		}
		else
		{
			MovableObject* gunfire = m_scene->GetSceneManager( )->getMovableObject( "player_gunfire", "Entity" );
			gunfire->setVisible( false );
		}

		for( IAnimationBlender::AnimationBlenderList::iterator i = m_animationBlenders.begin( ); i != m_animationBlenders.end( ); ++i )
		{
			( *i )->Blend( animationName, 0.2f, loopAnimation );
		}
	}
}