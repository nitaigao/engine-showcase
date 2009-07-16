#include "RendererSystemComponent.h"

#include "Management/Management.h"

#include "IRendererSystem.hpp"

#include "Maths/MathVector3.hpp"
#include "Maths/MathQuaternion.hpp"
using namespace Maths;

#include "OgreMax/OgreMaxModel.hpp"
using namespace OgreMax;
using namespace Ogre;

#include "Logging/Logger.h"
using namespace Logging;

namespace Renderer
{
	RendererSystemComponent::~RendererSystemComponent( )
	{
		m_scene->GetSceneManager( )->getRootSceneNode( )->removeAndDestroyChild( m_name );
	}

	void RendererSystemComponent::Initialize( )
	{
		m_sceneNode = m_scene->GetSceneManager( )->createSceneNode( m_name );

		this->LoadModel( m_sceneNode, m_attributes[ System::Parameters::Model ].As< std::string >( ) );

		m_scene->GetSceneManager( )->getRootSceneNode( )->addChild( m_sceneNode );
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
			Logger::Get( )->Fatal( e.what( ) );
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
			//m_sceneNode->setOrientation( parameters[ System::Attributes::Orientation ].As< MathQuaternion >( ).AsOgreQuaternion( ) );
		}

		if ( message == System::Messages::GetAnimationState )
		{
			SkeletonList skeletons;

			this->LinkSkeletons( m_sceneNode, &skeletons );
			
			return skeletons;
		}

		return AnyType( );
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

			if( object->getMovableType( ) == EntityFactory::FACTORY_TYPE_NAME )
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

	void RendererSystemComponent::LinkSkeletons( Ogre::SceneNode* sceneNode, RendererSystemComponent::SkeletonList* skeletons )
	{
		SceneNode::ObjectIterator objects = sceneNode->getAttachedObjectIterator( );

		while( objects.hasMoreElements( ) )
		{
			MovableObject* object = objects.getNext( );

			if( object->getMovableType( ) == EntityFactory::FACTORY_TYPE_NAME )
			{
				Entity* entity = m_scene->GetSceneManager( )->getEntity( object->getName( ) );

				if ( entity->hasSkeleton( ) )
				{
					Ogre::Skeleton::BoneIterator boneIterator = entity->getSkeleton( )->getBoneIterator( );

					while( boneIterator.hasMoreElements( ) )
					{
						Ogre::Bone* oBone = boneIterator.getNext( );
						oBone->setManuallyControlled( true );

						/*Entity *axis = m_scene->GetSceneManager( )->createEntity( oBone->getName( ) + "_axis", "/data/entities/meshes/axes.mesh" );
						TagPoint* tagPoint = entity->attachObjectToBone( oBone->getName( ), axis );
						tagPoint->setScale( 0.005f, 0.005f, 0.005f );*/
					}

					skeletons->push_back( entity->getSkeleton( ) );
				}
			}
		}

		Node::ChildNodeIterator children = sceneNode->getChildIterator( );

		while( children.hasMoreElements( ) )
		{
			SceneNode* child = static_cast< SceneNode* >( children.getNext( ) );
			this->LinkSkeletons( child, skeletons );
		}
	}
}