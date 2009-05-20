/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>
#include <Demos/Physics/UseCase/PhysicalAttachments/CharacterAttachments/CharacterAttachments.h>
#include <Demos/Physics/UseCase/PhysicalAttachments/CharacterAttachments/CharacterAttachmentsHelpers.h>

#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>

// Ragdoll  
#include <Animation/Ragdoll/Instance/hkaRagdollInstance.h>
#include <Animation/Ragdoll/Utils/hkaRagdollUtils.h>

// Serialization
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Serialize/Util/hkLoader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>
#include <Physics/Utilities/Serialize/hkpHavokSnapshot.h>

// Graphics Stuff
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Common/Visualize/hkDebugDisplay.h>

// Skeletal Animation
#include <Animation/Animation/hkaAnimationContainer.h>
#include <Animation/Animation/Rig/hkaPose.h>
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Mapper/hkaSkeletonMapper.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>

// Rigid body driving (to animation pose)
#include <Physics/Utilities/Constraint/Keyframe/hkpKeyFrameUtility.h>

// Viewer Helpers and Viewers
#include <Common/Visualize/hkProcess.h>
#include <Common/Visualize/hkProcessFactory.h>
#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpConstraintViewer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpRigidBodyInertiaViewer.h>
#include <Physics/Utilities/Collide/Filter/GroupFilter/hkpGroupFilterUtil.h>

// Debug display.
#include <Common/Visualize/hkDebugDisplay.h>



struct CharacterAttachmentsVariant
{
	const char*	 m_name;
	const char* m_details;
	bool m_useLimits;
	bool m_useDamping;
	bool m_useChain;
};

static const char helpString[] = "A demo of physical (rigid body) attachments on an animation-driven ragdoll. See code comments for details.";




static const CharacterAttachmentsVariant g_variants[] =
{
	{ "Powerline",	helpString, false,  false, false},
	{ "Powerline - Chain",	helpString, false,  false, true},
	{ "Powerline IDEAL - Chain and damping",	helpString, false,  true, false},
	{ "Scabbard - Ball and socket (No limits)",	helpString, false, false, false },
	{ "Scabbard - Ragdoll",	helpString, true, false, false },
	{ "Scabbard IDEAL - Ragdoll + Rigid body damping",	helpString, true , true, false},
	{ "Backpack - Hinge (No limits)",	helpString, false, false, false },
	{ "Backpack IDEAL - Limited Hinge",	helpString, true, false, false }
};

CharacterAttachments::CharacterAttachments(hkDemoEnvironment* env)
: hkDefaultPhysicsDemo(env)
{

	// Turn off some warnings we know about.
	hkError::getInstance().setEnabled( 0x234f224a, false ); // user allocated motion
	hkError::getInstance().setEnabled( 0x54e4323e, false ); // createmapping report
	hkError::getInstance().setEnabled( 0x9fe65234, false ); // deprecated simulation type 

	//
	// Set up the camera.
	//
	{
		hkVector4 from( -6.0f, -6.0f, 2.0f );
		hkVector4 to  ( 0.0f, 0.0f, 1.0f );
		hkVector4 up  ( 0.0f, 0.0f, 1.0f );
		setupDefaultCameras( env, from, to, up, 0.01f, 100 );
	}

	//
	// Create the World.
	//
	{
		hkpWorldCinfo info;
		info.m_gravity.set(0.0f, 0.0f, -9.81f);
		info.m_enableDeactivation = false;

		m_world = new hkpWorld( info );
		m_world->lock();

		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		// Set static global scale value to be small enough that constraints are easy to see inside ragdoll.
		hkpConstraintViewer::m_scale = 0.05f;

		setupGraphics();	
	}

	//
	// Replace filter.
	//
	hkpGroupFilter* filter  = new hkpGroupFilter();
	m_world->setCollisionFilter( filter, true);
	filter->removeReference();


	//
	// Make animated skeleton
	//
	{
		m_loader = new hkLoader();

		hkaSkeleton* animationSkeleton;

		// Get the rig.
		{
			hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkRig.hkx");
			hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
			HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
			hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

			HK_ASSERT2(0x27343435, ac && (ac->m_numSkeletons > 0), "No skeleton loaded");
			animationSkeleton = ac->m_skeletons[0];
		}

		// Create the animated skeleton for the animation.
		m_animatedSkeleton = new hkaAnimatedSkeleton( animationSkeleton );

		// Get animation and make animated skeleton from it.
		{
			hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkJumpLandLoop.hkx");
			hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
			HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
			hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

			HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0 ), "No animation loaded");
			HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
			hkaAnimationBinding* runBinding = ac->m_bindings[0];

			m_control = new hkaDefaultAnimationControl (runBinding);
			m_control->setMasterWeight(1.0f);
			m_control->setPlaybackSpeed(1.0f);
			m_animatedSkeleton->addAnimationControl( m_control );
			m_control->removeReference();
		}
	}

	//
	// Create the floor
	//
	{
		// Data specific to this shape.
		hkVector4 halfExtents(5.0f, 5.0f, 1.0f);

		hkpBoxShape* shape = new hkpBoxShape( halfExtents, 0 );

		// To illustrate using the shape, create a rigid body by first defining a template.
		hkpRigidBodyCinfo rigidBodyInfo;

		rigidBodyInfo.m_shape = shape;
		rigidBodyInfo.m_position.set(0.0f, 0.0f, -2.0f);
		rigidBodyInfo.m_motionType = hkpMotion::MOTION_FIXED;

		// Create a rigid body (using the template above).
		hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);
		shape->removeReference();
		m_world->addEntity(rigidBody);
		rigidBody->removeReference();
	}



	//
	// Load the ragdoll bodies and constraints
	//
	hkpPhysicsData* physicsData;
	hkpPhysicsSystem* physicsSystem;
	{
		hkString filename = hkAssetManagementUtil::getFilePath( "Resources/Animation/Ragdoll/hires_ragdoll.hkx" );

		hkIstream infile( filename.cString() );
		HK_ASSERT( 0x215d080c, infile.isOk() );

		physicsData = hkpHavokSnapshot::load( infile.getStreamReader(), &m_loadedData );
		physicsSystem = physicsData->getPhysicsSystems()[0];
	}

	//
	// Reorder constraints, set collision filtering information
	//
	CharacterAttachmentsHelpers::createRagdollFromBodiesAndConstraints(physicsSystem, filter, m_ragdollSkeleton, m_ragdollInstance);		

	//
	// Set all bodies in ragdoll to have keyframed motion type initially since it will start off being driven by the animation
	//
	CharacterAttachmentsHelpers::setRagdollMotionType(m_ragdollInstance, hkpMotion::MOTION_KEYFRAMED );

	//
	// Start animation off displaced to the side as we'll want to make it run in a circle.
	//
	m_accumulatedMotion.setIdentity();
	m_accumulatedMotion.m_translation.set(-3.0f, 0.0f, 0.0f);

	//
	// Move ragdoll to this offset start position
	//
	for( int b=0; b < m_ragdollInstance->getNumBones(); b++)
	{
		hkpRigidBody* rb = m_ragdollInstance->getRigidBodyOfBone( b );

		hkTransform t = rb->getTransform();
		t.getTranslation().add4(m_accumulatedMotion.m_translation);
		m_ragdollInstance->getRigidBodyOfBone(b)->setTransform(t);
	}

	//
	// Add ragdoll to the world.
	//
	m_ragdollInstance->addToWorld(m_world, true);

	//
	// Create the high->low mapper
	//
	m_highToLowMapper = CharacterAttachmentsHelpers::setupRagdollMapping(m_animatedSkeleton->getSkeleton(), m_ragdollSkeleton);


	//
	// Add the rigid boody (physical) 'attachments'
	//
	CharacterAttachmentsHelpers::ConstraintStabilityTricks tricks;
	tricks.m_useLimits = g_variants[ getDemoVariant() ].m_useLimits;
	tricks.m_useDamping = g_variants[ getDemoVariant() ].m_useDamping;
	tricks.m_useChain = g_variants[ getDemoVariant() ].m_useChain;

	if( hkString::strStr(g_variants[ getDemoVariant() ].m_name, "Powerline") != HK_NULL )
	{
		CharacterAttachmentsHelpers::addPowerline(m_world, m_ragdollInstance, m_accumulatedMotion, "Ragdoll Spine2", "Ragdoll R Hand", filter, tricks);
	}
	if( hkString::strStr(g_variants[ getDemoVariant() ].m_name, "Scabbard") != HK_NULL )
	{
		CharacterAttachmentsHelpers::addScabbard(m_world, m_ragdollInstance, m_accumulatedMotion,"Ragdoll Pelvis", filter, tricks);
	}
	if( hkString::strStr(g_variants[ getDemoVariant() ].m_name, "Backpack") != HK_NULL )
	{
		CharacterAttachmentsHelpers::addBackpack(m_world, m_ragdollInstance, m_accumulatedMotion,"Ragdoll Spine2", filter, tricks);
	}
	


	//
	// Store all dynamic bodies (ragdoll bodies and attached bodies) for later access
	//
	{
		// Get all bodies currently in the world
		hkpPhysicsSystem* system;
		system = m_world->getWorldAsOneSystem();

		// For each dynamic body
		for(int i = 0; i < system->getRigidBodies().getSize(); i++)
		{
			hkpRigidBody* rb = system->getRigidBodies()[i];
			if(rb->getMotionType() != hkpMotion::MOTION_FIXED)
			{
				m_dynamicBodies.pushBack(rb);
			}
		}
		system->removeReference();
	}

	m_initializeKeyframing = true;

	m_constraintViewerTag = -1;
	m_inertiaViewerTag = -1;

	m_numViewersOn = 0;
	m_playing = true;	
	m_driving = true;

	//
	// Step world for 100 frames to allow constraints to settle since we only placed them roughyl aligned with the ragdoll
	//
	for(int i = 0; i < 100; i++)
	{
		stepDemoInternal( true );
	}

	m_world->unlock();
}

hkDemo::Result CharacterAttachments::stepDemo()
{
	return stepDemoInternal( false );
}
 
hkDemo::Result CharacterAttachments::stepDemoInternal( hkBool settling )
{
	m_world->lock();

	//
	// Handle input.
	//
	if (!settling)
	{
		handleInput();
	}

	//
	// Advance the animation and accumulate extracted motion.
	//
	if (!settling && m_playing)
	{
		m_animatedSkeleton->stepDeltaTime( m_timestep );

		// Grab accumulated motion.
		{
			hkQsTransform deltaMotion;
			deltaMotion.setIdentity();
			m_animatedSkeleton->getDeltaReferenceFrame( m_timestep, deltaMotion);
			

			// Rotate this motion a bit to get us going in a circle.
			{
				hkQuaternion q;
				const hkVector4 up(0.0f, 0.0, 1.0f);
				q.setAxisAngle(up, 1.1f * m_timestep);
				hkQsTransform t;
				t.setIdentity();
				t.setRotation( q );
				deltaMotion.setMul( t, deltaMotion );
			}

			m_accumulatedMotion.setMulEq(deltaMotion);
			m_accumulatedMotion.m_rotation.normalize();
		}
	}

	//
	// Create a runtime pose for the animation and sample.
	//
	hkaPose animPose(m_animatedSkeleton->getSkeleton());
	m_animatedSkeleton->sampleAndCombineAnimations( animPose.accessUnsyncedPoseLocalSpace().begin(), animPose.getFloatSlotValues().begin() );

	//
	// Add accumulated motion back into root bone.
	//
	{
		hkQsTransform rootTransform = animPose.accessSyncedPoseLocalSpace()[0];
		rootTransform.setMul(m_accumulatedMotion, rootTransform);
		animPose.accessSyncedPoseLocalSpace()[0] = rootTransform;
	}

	//
	// Draw the original animation.
	//
	if (!settling)
	{
		AnimationUtils::drawPose( animPose, hkQsTransform::getIdentity() );
	}

	//
	// Create a runtime pose for the ragdoll.
	//
	hkaPose ragdollPose(m_ragdollInstance->getSkeleton());
	ragdollPose.setToReferencePose();

	//
	// Map from animation to physics.
	//
	m_highToLowMapper->mapPose( animPose, ragdollPose, hkaSkeletonMapper::CURRENT_POSE );

	//
	// Drive to animation pose if required
	//
	if(m_driving)
	{
		driveRagoll(ragdollPose);
	}

	m_world->unlock();

	//
	// Step demo as usual (unless in initial 'settling mode' in which case we directly step the world
	// to avoid graphics updates)
	//
	if ( !settling )
	{
		hkDefaultPhysicsDemo::stepDemo();
	}
	else
	{
		m_world->stepDeltaTime( m_timestep );
	}
	
	return hkDemo::DEMO_OK;
}

void CharacterAttachments::handleInput()
{
	if (m_env->m_gamePad->isButtonPressed(HKG_PAD_BUTTON_0))	// Navigating timers
	{
		return;
	}


	// Parse input keys
	if (!m_env->m_gamePad->isButtonPressed(HKG_PAD_BUTTON_0))	// Unless navigating timiers
	{
		const int offset = 80;
		m_env->m_textDisplay->outputText("Hit \221 to start/stop animation.", 20, offset );
		m_env->m_textDisplay->outputText("Hit \222 to turn on/off ragdoll driving.", 20, offset + 20);
		m_env->m_textDisplay->outputText("Mouseover and hold down \223 to view colliding bodies.", 20, offset + 40);
		m_env->m_textDisplay->outputText("Hit 'D-Pad LEFT' to turn on/off constraint viewing", 20, offset + 60);
		m_env->m_textDisplay->outputText("Hit 'D-Pad RIGHT' to turn on/off inertia viewing", 20, offset + 80);

		// Pause/unpause animation
		if (m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_1))
		{
			m_playing = !m_playing;
		}

		// Filter display
		if( m_env->m_gamePad->isButtonPressed( HKG_PAD_BUTTON_3 ) )
		{
			// Make sure filter is up-to-date, just for sanity's sake.
			m_world->updateCollisionFilterOnWorld(HK_UPDATE_FILTER_ON_WORLD_FULL_CHECK, HK_UPDATE_COLLECTION_FILTER_PROCESS_SHAPE_COLLECTIONS);

			const int width = m_env->m_window->getWidth() - 180 ;
			const int height = m_env->m_window->getHeight() / 2 ;
			m_env->m_textDisplay->outputText("Selected:  RED", width, height - 20 );
			m_env->m_textDisplay->outputText("Colliding: ORANGE", width, height);
			m_env->m_textDisplay->outputText("Filtered:  GREEN", width, height + 20);

			if( !m_filterViewer.isActive() )
			{
				hkpRigidBody* rb = CollisionFilterViewerUtil::getBodyUnderMousePointer(m_env, m_world);
				m_filterViewer.activate( rb, m_world );
			}
		}
		else
		{
			if( m_filterViewer.isActive() )
			{
				m_filterViewer.deactivate();
			}
		}

		// Turn on/off ragdoll driving. When off the ragdoll has no input from the animation
		// and will preserve its last velocities but fall to the ground (dead).
		if (m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_2))
		{
			if( !m_driving )
			{
				m_driving = true;
				// The animation may keep on running, so its location could be far away from where the ragdoll is.
				// So just warp it close for this demo.
				CharacterAttachmentsHelpers::setRagdollMotionType(m_ragdollInstance,  hkpMotion::MOTION_KEYFRAMED);
				shiftAllBodiesToAnimationPoseCenter();
				m_initializeKeyframing = true;
			}
			else
			{
				m_driving = false;
				CharacterAttachmentsHelpers::setRagdollMotionType(m_ragdollInstance, hkpMotion::MOTION_DYNAMIC);
			}
		}


		// Turn on/off constraint display
		if ( m_env->m_gamePad->wasButtonPressed(HKG_PAD_DPAD_LEFT))
		{
			if(m_constraintViewerTag == -1)
			{
				addViewer( hkpConstraintViewer::getName(), m_constraintViewerTag );
			}
			else
			{
				removeViewer(m_constraintViewerTag);
			}
		}

		// Turn on/off inertia display
		if ( m_env->m_gamePad->wasButtonPressed(HKG_PAD_DPAD_RIGHT))
		{
			if(m_inertiaViewerTag == -1)
			{
				addViewer( hkpRigidBodyInertiaViewer::getName(), m_inertiaViewerTag );
			}
			else
			{
				removeViewer(m_inertiaViewerTag);
			}
		}

	}
}


void CharacterAttachments::driveRagoll(const hkaPose& ragdollPose)
{
	// Set up ragdoll driving params.
	hkaKeyFrameHierarchyUtility::ControlData controlData;
	{
		controlData.m_hierarchyGain = 0.1f;
		controlData.m_velocityDamping = 0.0f;
		controlData.m_accelerationGain = 1.0f;
		controlData.m_velocityGain = 0.2f;

		controlData.m_positionGain = 0.1f;
		controlData.m_positionMaxLinearVelocity = 0.2f;
		controlData.m_positionMaxAngularVelocity = 0.3f;

		controlData.m_snapGain = 0.1f;
		controlData.m_snapMaxLinearVelocity  = 0.3f;
		controlData.m_snapMaxAngularVelocity = 0.3f;
		controlData.m_snapMaxLinearDistance  = 0.03f;
		controlData.m_snapMaxAngularDistance = 0.1f;
	}

	hkaKeyFrameHierarchyUtility::Output output[100];

	hkaKeyFrameHierarchyUtility::KeyFrameData keyFrameData;
	{
		keyFrameData.m_worldFromRoot.setIdentity();
		keyFrameData.m_desiredPoseLocalSpace = ragdollPose.getSyncedPoseLocalSpace().begin();
		keyFrameData.m_internalReferencePose = &m_oldKeyFrameHierarchyData[0];
	}

	hkaKeyFrameHierarchyUtility::BodyData bodyData;
	{
		bodyData.m_numRigidBodies = m_ragdollInstance->m_rigidBodies.getSize();
		bodyData.m_rigidBodies = m_ragdollInstance->m_rigidBodies.begin();
		bodyData.m_parentIndices = m_ragdollInstance->getSkeleton()->m_parentIndices;
		bodyData.m_controlDataIndices = 0;
	}

	if ( m_initializeKeyframing  )
	{
		hkaKeyFrameHierarchyUtility::initialize( bodyData, m_oldKeyFrameHierarchyData );
		m_initializeKeyframing = false;
	}

	hkaKeyFrameHierarchyUtility::applyKeyFrame( m_timestep, keyFrameData, bodyData, &controlData, output );
}

// Helper to add a viewer to the world.
void CharacterAttachments::addViewer(const char* name, int& tag)
{
	hkProcess* p = hkProcessFactory::getInstance().createProcess( name, m_contexts );
	if (p)
	{
		p->m_inStream = HK_NULL; // no streams
		p->m_outStream = HK_NULL;
		p->m_displayHandler = static_cast<hkDebugDisplayHandler*>(m_env->m_displayHandler);
		p->m_processHandler = HK_NULL; // no process handler
		p->init();
		m_debugProcesses.pushBack(p); // so we can delete them
		tag = p->getProcessTag();
		m_numViewersOn++;
	}

	// Color bodies transparent
	if( m_numViewersOn == 1 )
	{
		for(int i = 0; i < m_dynamicBodies.getSize(); i++)
		{
			hkpRigidBody* rb = m_dynamicBodies[i];
			HK_SET_OBJECT_COLOR((hkUlong)rb->getCollidable(), hkColor::rgbFromChars( 255, 255, 255, 128 ));
		}
	}
}

// Helper to remove a viewer from the world.
void CharacterAttachments::removeViewer(int& tag)
{
	for(int i = 0; i < m_debugProcesses.getSize(); i++)
	{
		if( m_debugProcesses[i]->getProcessTag() == tag )
		{
			hkProcess* p =  m_debugProcesses[i];
			m_debugProcesses.removeAt(i);
			delete p;
			tag = -1;
			m_numViewersOn--;
			break;
		}
	}

	// Color bodies back to non-transparent if no viewers on
	if( m_numViewersOn == 0 )
	{
		for(int i = 0; i < m_dynamicBodies.getSize(); i++)
		{
			hkpRigidBody* rb = m_dynamicBodies[i];
			HK_SET_OBJECT_COLOR((hkUlong)rb->getCollidable(), hkColor::rgbFromChars( 240, 240, 240, 255 ));
		}
	}
}

// Helper function to move all bodies of the current ragdoll and the attachment bodies with a constant transform
// to align to the root pose of the current animation frame.
// This simply ensures that if we turn on animation driving at any point, the ragdoll rigid bodies
// will at least be warped to a place close to where they should be as determined by the animation pose
// and so the driving code will not have much work to do and will not overshoot.
// This is only a useful function for this demo as in a real game the user will likely warp the bodies
// to an exact 'start' pose where the attachement bodies have a stable 'rest'position known to the game - here we do not know
// a stable rest position, hence it is safeest to just move the entire current configuation at once.
void CharacterAttachments::shiftAllBodiesToAnimationPoseCenter()
{
	// Move ragdoll root body back and shift all bodies similarly

	// Create a runtime pose for the animation.
	hkaPose animPose(m_animatedSkeleton->getSkeleton());
	{
		// Sample animation.
		m_animatedSkeleton->sampleAndCombineAnimations( animPose.accessUnsyncedPoseLocalSpace().begin(), animPose.getFloatSlotValues().begin() );

		// Add accumulated motion back into root bone.
		hkQsTransform rootTransform = animPose.accessSyncedPoseLocalSpace()[0];
		rootTransform.setMul(m_accumulatedMotion, rootTransform);
		animPose.accessSyncedPoseLocalSpace()[0] = rootTransform;
	}


	hkTransform currentRootTransformT;
	hkTransform desiredRootTransformT;

	// Map from anim to ragdoll
	{		
		hkpRigidBody* rb = m_ragdollInstance->getRigidBodyOfBone( 0 );
		currentRootTransformT = rb->getTransform();
		{
			hkaPose ragdollPose(m_ragdollInstance->getSkeleton());
			ragdollPose.setToReferencePose();

			// Map from animation to physics.
			m_highToLowMapper->mapPose( animPose, ragdollPose, hkaSkeletonMapper::CURRENT_POSE );
			hkQsTransform t2 = ragdollPose.accessSyncedPoseLocalSpace()[0];

			desiredRootTransformT = hkTransform(t2.m_rotation, t2.m_translation);
		}
	}

	hkTransform diffT;
	diffT.setMulMulInverse( desiredRootTransformT, currentRootTransformT);

	// Loop over and transform all bodies
	for(int i = 0; i < m_dynamicBodies.getSize(); i++)
	{
		hkpRigidBody* rb = m_dynamicBodies[i];
		hkTransform newT;
		newT.setMul(diffT, rb->getTransform());
		rb->setTransform(newT);
	}

	// Stabilize constraints since we just warped ragdoll
	for(int i = 0; i < 500; i++)
	{
		stepDemoInternal( true );
	}	
}


CharacterAttachments::~CharacterAttachments()
{
	m_world->lock();

	m_highToLowMapper->removeReference();
	m_animatedSkeleton->removeReference();
	m_ragdollInstance->removeReference();

	hkaRagdollUtils::destroySkeleton(m_ragdollSkeleton);

	m_world->unlock();
	m_world->markForWrite();
	m_world->removeReference();
	m_world = HK_NULL;

	m_loadedData->removeReference();

	delete m_loader;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString2[] = "Press 1 to pause animation.\n" ;

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( CharacterAttachments, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_SERIALIZE, CharacterAttachmentsVariant, g_variants, helpString2 );

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20090216)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2009
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available at www.havok.com/tryhavok.
* 
*/
