#include "AnimationSystemComponent.h"

using namespace Maths;

#include "AnimationBlender.h"

#include "IO/IResource.hpp"
using namespace Resources;

#include "Management/Management.h"

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

#include <Animation/Animation/Ik/Ccd/hkaCcdIkSolver.h>
#include <Animation/Animation/Ik/TwoJoints/hkaTwoJointsIkSolver.h>

#include <Ogre.h>
#include <OgreTagpoint.h>
using namespace Ogre;

namespace Animation
{
	AnimationSystemComponent::~AnimationSystemComponent()
	{
		for( LoadedDataList::iterator i = m_loadedData.begin( ); i != m_loadedData.end( ); ++i )
		{
			( *i )->removeReference( );
		}

		if ( m_animationBlender != 0 )
		{
			delete m_animationBlender;
			m_animationBlender = 0;
		}
	}

	void AnimationSystemComponent::Initialize( )
	{
		m_attributes[ System::Attributes::LookAt ] = MathVector3::Zero( );
		m_attributes[ System::Attributes::Position ] = MathVector3::Zero( );
		m_attributes[ System::Attributes::POI ] = MathVector3::Zero( );

		m_animationBlender = new AnimationBlender( );

		std::string bindPose = m_attributes[ System::Attributes::Animation::BindPose ].As< std::string >( );
		IResource* resource = Management::Get( )->GetResourceManager( )->GetResource( bindPose );

		hkIstream istreamFromMemory( resource->GetFileBuffer( )->fileBytes, resource->GetFileBuffer( )->fileLength );
		hkStreamReader* streamReader = istreamFromMemory.getStreamReader( );

		hkBinaryPackfileReader reader;
		reader.loadEntireFile( streamReader );

		hkVersionUtil::updateToCurrentVersion( reader, hkVersionRegistry::getInstance() );

		hkPackfileData* loadedData = reader.getPackfileData( );
		m_loadedData.push_back( loadedData );
		loadedData->addReference( );

		hkRootLevelContainer* container = static_cast< hkRootLevelContainer* >( reader.getContents( "hkRootLevelContainer" ) );
		hkaAnimationContainer* animationContainer = reinterpret_cast< hkaAnimationContainer* >( container->findObjectByType( hkaAnimationContainerClass.getName( ) ) );

		m_skeleton = animationContainer->m_skeletons[ 0 ];
		m_skeletonInstance = new hkaAnimatedSkeleton( m_skeleton );

		typedef std::map< std::string, std::string > AnimationList;
		AnimationList animations = m_attributes[ System::Attributes::Animation::Animations ].As< AnimationList >( );

		for( AnimationList::iterator i = animations.begin( ); i != animations.end( ); ++i )
		{
			this->LoadAnimation( ( *i ).first, ( *i ).second );
		}

		m_animationBlender->Blend( m_attributes[ System::Attributes::Animation::DefaultAnimation ].As< std::string >( ) );

		AnyType::AnyTypeKeyMap results = this->PushMessage( System::Messages::GetAnimationState, AnyType::AnyTypeMap( ) ).As< AnyType::AnyTypeKeyMap >( );
		m_ogreSkeletons = results[ System::Types::RENDER ].As< SkeletonList >( );
	}

	void AnimationSystemComponent::LoadAnimation( const std::string& animationName, const std::string& animationPath )
	{
		IResource* resource = Management::Get( )->GetResourceManager( )->GetResource( animationPath );

		hkIstream istreamFromMemory( resource->GetFileBuffer( )->fileBytes, resource->GetFileBuffer( )->fileLength );
		hkStreamReader* streamReader = istreamFromMemory.getStreamReader( );

		hkBinaryPackfileReader reader;
		reader.loadEntireFile( streamReader );

		hkVersionUtil::updateToCurrentVersion( reader, hkVersionRegistry::getInstance() );

		hkPackfileData* loadedData = reader.getPackfileData( );  
		m_loadedData.push_back( loadedData );
		loadedData->addReference( );

		hkRootLevelContainer* container = static_cast< hkRootLevelContainer* >( reader.getContents( "hkRootLevelContainer" ) );
		hkaAnimationContainer* animationContainer = reinterpret_cast< hkaAnimationContainer* >( container->findObjectByType( hkaAnimationContainerClass.getName( ) ) );

		m_animations.push_back( animationContainer->m_animations[ 0 ] );
		m_animationBindings.push_back( animationContainer->m_bindings[ 0 ] );

		hkaDefaultAnimationControl* control = new hkaDefaultAnimationControl( m_animationBindings[ m_animations.size( ) - 1 ] );
		control->setMasterWeight( 0.0f );
		control->setPlaybackSpeed( 1.0f );

		m_skeletonInstance->addAnimationControl( control );
		m_animationBlender->RegisterController( animationName, control );
		control->removeReference( );
	}

	AnyType AnimationSystemComponent::Message( const System::Message& message, AnyType::AnyTypeMap parameters )
	{
		AnyType result;

		if ( message == System::Messages::StartAnimation )
		{
			m_animationBlender->Blend( parameters[ System::Parameters::AnimationName ].As< std::string >( ) );
		}

		if ( message == System::Messages::StopAnimation )
		{
			m_animationBlender->UnBlend( parameters[ System::Parameters::AnimationName ].As< std::string >( ) );
		}

		if ( message == System::Messages::SetLookAt )
		{
			m_attributes[ System::Attributes::LookAt ] = parameters[ System::Attributes::LookAt ].As< MathVector3 >( );
			m_attributes[ System::Attributes::POI ] = parameters[ System::Attributes::POI ].As< MathVector3 >( );
		}

		if ( message == System::Messages::SetPosition )
		{
			m_attributes[ System::Attributes::Position ] = parameters[ System::Attributes::Position ].As< MathVector3 >( );
		}

		return result;
	}

	void AnimationSystemComponent::Update( const float& deltaMilliseconds )
	{
		m_animationBlender->Update( deltaMilliseconds );

		m_skeletonInstance->stepDeltaTime( deltaMilliseconds );

		hkaPose* pose = new hkaPose( m_skeleton );
		m_skeletonInstance->sampleAndCombineAnimations( pose->accessUnsyncedPoseLocalSpace( ).begin( ),pose->getFloatSlotValues( ).begin( ) );

		for ( SkeletonList::iterator i = m_ogreSkeletons.begin( ); i != m_ogreSkeletons.end( ); ++i )
		{
			Skeleton::BoneIterator rightBoneIterator = ( *i )->getRootBoneIterator( );

			while( rightBoneIterator.hasMoreElements( ) )
			{
				this->TransformBone( pose, rightBoneIterator.getNext( ) );
			}
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