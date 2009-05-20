/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GDC_2005_DEMO_H
#define HK_GDC_2005_DEMO_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Graphics/Bridge/SceneData/hkgSceneDataConverter.h>
#include <Demos/ShowCase/Gdc2005/Gdc2005DemoOptions.h>

#include <Graphics/Common/Input/Pad/hkgPad.h>
#include <Graphics/Common/Window/hkgWindow.h>

#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>

class hkaSkeleton;
class hkaPose;
class hkaFootPlacementIkSolver;

// This demo shows an integration of Havok Physics and Havok Animation
class Gdc2005Demo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		// Constructor, loads the assets and initializes the different animation components
		Gdc2005Demo(hkDemoEnvironment* env);

		// Destructor
		~Gdc2005Demo();

		// Called every step automatically by the demo framework, it steps the physics and the animation
		hkDemo::Result stepDemo();

		// Called by stepDemo(); it samples the animation, applies different components.
		// It may also modify the state machine input accordingly.
		void applyAnimationComponents (struct GdcStateInput& stateInput);

	public:

		// Collision Layers used in this demo.
		// By enabling and disabling collisions between these layers, we can tweak the behaviour of the demo
		enum
		{
			LAYER_LANDSCAPE = 1,
			LAYER_PROXY = 2,
			LAYER_DEBRIS = 3,
			LAYER_MOVABLE_ENVIRONMENT = 4,
			LAYER_RAGDOLL_KEYFRAMED = 5,
			LAYER_RAYCAST = 6,
			LAYER_RAGDOLL_DYNAMIC = 7,
			LAYER_PICKING = 8
		};

		/*
		** FOOT IK
		*/

		// Enumerations for readability
		enum WhichLeg {LEFT_LEG, RIGHT_LEG, MAX_LEGS};

		// Create the foot ik components
		void initFootIK ();
		// Perform the foot Ik (modify the pose)
		void doFootIK (hkBool footIkOn, const hkQsTransform& originalLeftFootMS, const hkQsTransform& originalRightFootMS, hkaPose& poseInOut, hkReal& verticalDisplacementInOut);
		// Destroy foot ik components
		void quitFootIK ();

		// Raycast interface : It uses havok physics for raycasting
		class hkaRaycastInterface* m_raycastInterface;
		// The two components (one for each leg) used for foot placement
		hkaFootPlacementIkSolver* m_footPlacementComponent [MAX_LEGS];
		// We keep track of whether the character is supported by each leg
		hkBool m_footOnAir[MAX_LEGS];

		/*
		** REACH
		*/

		// Enumeration, for readability
		enum WhichArm {LEFT_ARM, RIGHT_ARM, MAX_ARMS};

		// Initializes the reach component
		void initReach();
		// Performs reach
		void doReach (hkBool reachOn, WhichArm whichArm, hkaPose& poseInOut);

		// Setup data, initialized in initReach()
		hkInt16 m_shoulderIdx[MAX_ARMS];
		hkInt16 m_elbowIdx[MAX_ARMS];
		hkInt16 m_handIdx[MAX_ARMS];
		hkInt16 m_reachReferenceBoneIdx;
		hkVector4 m_radarLocationRS[MAX_ARMS];
		hkVector4 m_elbowAxis[MAX_ARMS];
		// Run-time data, used for smoothing the reach behaviour
		hkBool m_reachStopped[MAX_ARMS];
		hkReal m_reachWeight[MAX_ARMS];
		hkVector4 m_previousReachPointWS[MAX_ARMS];
		hkVector4 m_previousNormalWS[MAX_ARMS];

		/*
		** LOOK AT
		*/

		// Initialization
		void initLookAt();
		// Performs look at, toward the given target
		void doLookAt(hkBool lookAtOn, const hkVector4& targetWS, hkaPose& poseInOut);

		// Setup data, initialized in initLookAt()
		hkInt16 m_headIdx;
		hkInt16 m_neckIdx;
		// Run-time data, used for smoothing the look at controller
		hkVector4 m_lookAtLastTargetWS;
		hkReal m_lookAtWeight;

		/*
		**
		** RAGDOLL : Death, get hit & get up
		**
		*/

		/* Death & Get Hit (ragdoll controllers) */

		// Initialization, it prepares mappers, instances, etc..
		void initRagdoll();
		// doRagdoll() drives the ragdoll when it's in death mode (using a powered constraint controller)
		void doRagdoll (hkaPose &thePose);
		void stopRagdoll ();
		// doRagdollFeedback() drives the ragdoll when it's in "alive" mode (using a rigid body controller)
		void doRagdollFeedback (hkaPose &poseInOut);
		void stopRagdollFeedback ();
		// called on destruction
		void quitRagdoll ();

		/* Pose matching : selects a driving poses during death */

		// Initializes the hkaPoseMatchingUtility with candidate poses to get up
		void initPoseMatching();
		// Looks for candidates poses to get up and sets the pose accordingly
		void matchPose(hkaPose &thePose, hkReal& error);
		// called on destruction
		void quitPoseMatching();

		/* Get up : blending during getup */

		// Called only during get up, it blends the current ragdoll pose into the animation pose
		void doGetup ( hkaPose &thePose );

		/* Other */

		// Sets the strengths of motors
		void setMotors(class hkaRagdollInstance* ragdoll, hkReal force, hkReal tau, hkReal propRec, hkReal linRec);

		// Shows/hides the rigid bodies of the ragdoll
		void toggleRagdollVisibility();

		/* Data */

		// The ragdoll instance - it's kept at all times (for either get hit or death)
		class hkaRagdollInstance* m_ragdollInstance;

		// Two mappers (anim->ragdoll  and  ragdoll->anim)
		class hkaSkeletonMapper* m_ragdollToHighResMapper;
		class hkaSkeletonMapper* m_highResToRagdollMapper;

		// Ragdoll display
		hkArray<class hkgDisplayObject*> m_ragdollDisplayBodies; // so we can hide them
		hkBool m_ragdollDisplayBodiesVisible;

		// Rigid Body controller, used for get hit effects
		class hkaRagdollRigidBodyController* m_rigidBodyController;
		hkBool m_initFeedback;
		hkBool m_initRd;
		// We keep track of what rigid bodies are in the lower body (they do not react to get hit effects)
		hkArray<hkInt16> m_lowerBodyBones;

		// For get-up blending
		class AnimationMixerSkeletalAnimation* m_matchingPose;
		class AnimationMixerControl* m_matchingPoseControl;
		class hkaDefaultAnimationControl* m_blendControl;
		class hkaPoseMatchingUtility* m_poseMatchUtils;

		// For pose matching
		hkQsTransform m_ragdollFrame;
		hkQsTransform m_animFrame;
		hkBool m_initGetup;
		int m_currentMatch;
		int m_bestMatch;
		hkReal m_timeSinceBetterMatch;


		/*
		** SKINNING
		*/

		// Renders the skin around the given pose
		void doSkinning (const hkaPose& thePose);

		// For skinning
		class hkaMeshBinding** m_skinBindings;
		hkInt32 m_numSkinBindings;
		hkBool	m_bUseHardwareSkinning;
		hkBool  m_bUseWorldHardwareSkinning;


		/*
		** CAMERA
		*/

		// Updates the camera (it uses the pose to locate the head of the character)
		void doCamera (const hkaPose& thePose);

		// The three camera implementations
		void findCamera(const hkVector4& target);
		void fixedCamera(const hkVector4& target) const;
		void otsCamera(const hkVector4& target) const;

		// Cameras
		int m_currentCamera;
		int m_numCameras;
		class hkxCamera** m_cameras;
		hkReal m_haveBetterCamera;
		int m_lastCamera;
		hkBool m_forcedTrackballOn;

		/*
		** CHARACTER CONTROLLER
		*/

		// Create the character proxy and state machine
		void initCharacterController();

		// Displays the current animation state
		void displayCurrentState() const;

		// Updates the display version of the character proxy
		void updateProxyDisplay(hkReal radius, hkReal height);

		// Displacement : decoupling of character controller and animation
		hkReal m_verticalDisplacement;
		hkVector4 m_horizontalDisplacement;

		// Character proxy
		class hkpCharacterProxy* m_characterProxy;
		class GdcCharacterListener* m_proxyListener;
		hkReal m_timeUnsupported;

		// Proxy display
		class hkgDisplayObject* m_proxyDisplay;
		hkBool m_proxyVisible;
		void toggleProxyVisibility();

		/*
		** LANDSCAPE AND OBJECTS
		*/

		// Loads the level
		void initLevel();

		// Called on destruction
		void quitLevel();

		// Wall
		void initBrickWall();
		void rebuildBrickWall();
		bool isBrick(hkpEntity* e);
		void removeBrick(hkpRigidBody* rb);

		// Loads a library of objects (skulls, boxes, arrows) we can chuck around
		void initDynamicObjects();
		void chuckSomething( const hkVector4& startPos, const hkVector4& velocity );
		bool isChuckable(hkpEntity* e);
		void removeChuckable(hkpRigidBody* rb);
		void fireShot( const hkVector4& start, const hkVector4& end, float strength);

		// Loads an hkx scene (with physics). It overrides the layer of all rigid bodies (if layer>=0)
		void loadSceneWithPhysics(const char* filename, hkgAssetConverter::ConvertMask convMask, int layer=-1, int systemGroup=0);

		/* data */

		hkArray<hkTransform> m_wallPoints; // brick wall auto gen points
		hkArray<hkpRigidBody*> m_bricks; // pre done bricks. reused when reset, first one is the clone source.

		// Display choice for landscape graphics look
		hkgDisplayObject* m_lightmapDisplay;
		hkgDisplayObject* m_vertexColorDisplay;

		// For dynamic items we throw into the demo (cloned first, then added to world)
		struct ObjectMap
		{
			class hkpRigidBody* m_rb;
			class hkgDisplayObject* m_dispObj;
		};
		hkArray<ObjectMap> m_chuckables;
		hkArray<hkpRigidBody*> m_chuckedItems;
		hkPseudoRandomGenerator m_randomNumberGen;

		/*
		** ANIMATION & ANIMATION MACHINE
		*/

		// Create the animation state machine and command processor
		void initAnimation();

		// Animation processor and state machine
		class GdcStateManager* m_stateManager;
		class GdcAnimationMachine* m_animationMachine;
		class GdcStateContext* m_animationStateMachine;

		// Animation mixing
		class hkaAnimatedSkeleton* m_animatedSkeleton;

		// The current reference frame (world-from-model)
		hkQsTransform m_currentTransform;

		/*
		** GAME OPTION (Tweaking)
		*/

		// Handles tweaking input (keys, pad) and updates options, called by stepDemo()
		void handleTweakingInput();
		// Updates the demo according to current value in the options
		void optionUpdate();

		// Game options & tweaking
		Gdc2005DemoOptions m_options;
		hkString m_selected;
		bool m_tweaking;

		/*
		** USER INPUT
		*/

		// Handles user input (other than tweaking), called by stepDemo()
		void handleUserInput(struct GdcStateInput& stateInput);

		// Smooths user input
		void updateUserControl();

		// Shows help for the keys in the demo
		void showKeys();

		// Picking support (incl picking of hidden physical objs)
		virtual void mouseDown();

		// Data used to handle directional input
		hkVector4 m_desiredDirection; // desired direction to try and get walking along.
		hkReal  m_forwardSpeed; // as taken from the stick pos wrt to camera frame
		hkReal  m_dampedForwardSpeed; // 0..1 for walk/run ratio
		hkReal  m_dampedTurnAngle; // in radians, per frame

		/*
		** OTHER
		*/


		// Warps the character to a new position
		void warpCharacter(int warpPointIndex);

		// Switches the flashlight on/off
		void updateFlashlight(bool on);

		// Virtual method called by demo framework, it registers some viewers
		virtual void setupContexts(hkArray<hkProcessContext*>& contexts);

	private:

		// Used for loading the different assets
		class hkLoader* m_loader;

		// Contents of the file containing the rig, used by both animation and ragdoll
		class hkRootLevelContainer* m_rigContainer;

		// Warping the character (preset start positions from the level file)
		hkArray<hkTransform> m_warpPoints;
		int m_currentWarpPoint; // used especially when the guy falls off the side of the world

};

#endif // HK_GDC_2005_DEMO_H


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
