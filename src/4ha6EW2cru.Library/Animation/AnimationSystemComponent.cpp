#include "AnimationSystemComponent.h"

using namespace Maths;

#include "../IO/IResource.hpp"
using namespace Resources;

#include "../Management/Management.h"

#include <Common/Base/System/Io/IStream/hkIStream.h>

#include <Common/Serialize/Packfile/Binary/hkBinaryPackfileReader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Serialize/Version/hkVersionUtil.h>

#include <Physics/Utilities/Serialize/hkpPhysicsData.h>

#include <Animation/Animation/hkaAnimation.h>
#include <Animation/Animation/hkaAnimationContainer.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>
#include <Animation/Animation/Rig/hkaPose.h>
#include <Animation/Animation/Deform/Skinning/Fpu/hkaFPUSkinningDeformer.h>
#include <Animation/Animation/Rig/hkaSkeletonUtils.h>
#include <Animation/Animation/Mapper/hkaSkeletonMapper.h>
#include <Animation/Animation/Mapper/hkaSkeletonMapperData.h>
#include <Animation/Animation/Mapper/hkaSkeletonMapperUtils.h>

#include <Ogre.h>
#include <OgreTagpoint.h>
using namespace Ogre;

namespace Animation
{
	AnimationSystemComponent::~AnimationSystemComponent()
	{
		if ( m_loadedData != 0 )
		{
			m_loadedData->removeReference( );
		}
	}

	void AnimationSystemComponent::Initialize( )
	{
		std::string rigPath = "/data/entities/animations/player_Idle.hkx";

		IResource* resource = Management::GetResourceManager( )->GetResource( rigPath );

		hkIstream istreamFromMemory( resource->GetFileBuffer( )->fileBytes, resource->GetFileBuffer( )->fileLength );
		hkStreamReader* streamReader = istreamFromMemory.getStreamReader( );

		hkBinaryPackfileReader reader;
		reader.loadEntireFile( streamReader );

		hkVersionUtil::updateToCurrentVersion( reader, hkVersionRegistry::getInstance() );

		m_loadedData = reader.getPackfileData( );
		m_loadedData->addReference( );

		hkRootLevelContainer* container = static_cast< hkRootLevelContainer* >( reader.getContents( "hkRootLevelContainer" ) );
		hkaAnimationContainer* animationContainer = reinterpret_cast< hkaAnimationContainer* >( container->findObjectByType( hkaAnimationContainerClass.getName( ) ) );

		m_skeleton = animationContainer->m_skeletons[ 0 ];
		m_skeletonInstance = new hkaAnimatedSkeleton( m_skeleton );

		this->LoadAnimation( "idle", "/data/entities/animations/player_Idle.hkx" );
		this->LoadAnimation( "run_forward", "/data/entities/animations/player_run_forward.hkx" );

		m_animationControls[ "idle" ]->setMasterWeight( 0.5f );
		m_animationControls[ "run_forward" ]->setMasterWeight( 0.5f );
	}

	void AnimationSystemComponent::LoadAnimation( const std::string& animationName, const std::string& animationPath )
	{
		IResource* resource = Management::GetResourceManager( )->GetResource( animationPath );

		hkIstream istreamFromMemory( resource->GetFileBuffer( )->fileBytes, resource->GetFileBuffer( )->fileLength );
		hkStreamReader* streamReader = istreamFromMemory.getStreamReader( );

		hkBinaryPackfileReader reader;
		reader.loadEntireFile( streamReader );

		hkVersionUtil::updateToCurrentVersion( reader, hkVersionRegistry::getInstance() );

		m_loadedData = reader.getPackfileData( );
		m_loadedData->addReference( );

		hkRootLevelContainer* container = static_cast< hkRootLevelContainer* >( reader.getContents( "hkRootLevelContainer" ) );
		hkaAnimationContainer* animationContainer = reinterpret_cast< hkaAnimationContainer* >( container->findObjectByType( hkaAnimationContainerClass.getName( ) ) );

		m_animations.push_back( animationContainer->m_animations[ 0 ] );
		m_animationBindings.push_back( animationContainer->m_bindings[ 0 ] );

		hkaDefaultAnimationControl* control = new hkaDefaultAnimationControl( m_animationBindings[ m_animations.size( ) - 1 ] );
		control->setMasterWeight( 0.0f );
		control->setPlaybackSpeed( 1.0f );

		m_skeletonInstance->addAnimationControl( control );
		m_animationControls.insert( std::make_pair( animationName, control ) );
		control->removeReference( );
	}

	AnyType AnimationSystemComponent::Message( const System::Message& message, AnyType::AnyTypeMap parameters )
	{
		AnyType result;

		if ( message == System::Messages::AddedToComponent )
		{
			AnyType::AnyTypeKeyMap results = this->PushMessage( System::Messages::GetAnimationState, AnyType::AnyTypeMap( ) ).As< AnyType::AnyTypeKeyMap >( );
			m_ogreSkeleton = results[ System::Types::RENDER ].As< Ogre::SkeletonInstance* >( );
		}

		/*if ( message == System::Messages::StartAnimation )
		{
			AnyType::AnyTypeKeyMap results = this->PushMessage( System::Messages::GetAnimationState, parameters ).As< AnyType::AnyTypeKeyMap >( );
			
			Renderer::IAnimationController* controller = results[ System::Types::RENDER ].As< Renderer::IAnimationController* >( );
			
			controller->SetTime( 0 );
			controller->SetLoop( parameters[ System::Parameters::LoopAnimation ].As< bool >( ) );
			controller->SetEnabled( true );

			IAnimationNode* node = new AnimationNode( controller );
			m_animationTree->AddNode( node );
		}

		if ( message == System::Messages::StopAnimation )
		{
			std::string animationName = parameters[ System::Parameters::AnimationName ].As< std::string >( );
			IAnimationNode* node = m_animationTree->RemoveNode( animationName );
			Renderer::IAnimationController* controller = node->GetAnimationController( );
			controller->SetEnabled( false );
			delete node;
		}*/

		return result;
	}

	void AnimationSystemComponent::Update( const float& deltaMilliseconds )
	{
		m_skeletonInstance->stepDeltaTime( deltaMilliseconds );

		hkaPose* pose = new hkaPose( m_skeleton );
		m_skeletonInstance->sampleAndCombineAnimations( pose->accessUnsyncedPoseLocalSpace( ).begin( ),pose->getFloatSlotValues( ).begin( ) );

		Ogre::Skeleton::BoneIterator boneIterator = m_ogreSkeleton->getRootBoneIterator( );

		while( boneIterator.hasMoreElements( ) )
		{
			Ogre::Bone* oBone = boneIterator.getNext( );
			this->TransformBone( pose, oBone );
		}
	}

	void AnimationSystemComponent::TransformBone( hkaPose* pose, Ogre::Node* bone )
	{
		const hkQsTransform* poseBones = ( !bone->getParent( ) ) 
			? poseBones = pose->getSyncedPoseModelSpace( ).begin( ) 
			: poseBones = pose->getSyncedPoseLocalSpace( ).begin( );
	
		int boneIndex = hkaSkeletonUtils::findBoneWithName( *pose->getSkeleton( ), bone->getName( ).c_str( ) );

		if ( boneIndex > -1 )
		{
			const hkQsTransform hBone = poseBones[ boneIndex ];

			bone->setPosition( MathVector3( hBone.getTranslation( ) ).AsOgreVector3( ) );
			bone->setOrientation( MathQuaternion( hBone.getRotation( ) ).AsOgreQuaternion( ) );

			Node::ChildNodeIterator children = bone->getChildIterator( );

			while( children.hasMoreElements( ) )
			{
				this->TransformBone( pose, children.getNext( ) );
			}
		}
	}

	
}