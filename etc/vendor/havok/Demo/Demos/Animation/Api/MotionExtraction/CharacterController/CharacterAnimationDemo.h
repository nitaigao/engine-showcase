/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CHARACTERANIMATIONDEMO_H
#define HK_CHARACTERANIMATIONDEMO_H

//#include <hkdemoframework/hkDefaultDemo.h>
#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>

#include <Demos/Animation/Api/MotionExtraction/CharacterController/StateMachine/MyStateMachine.h>

#include <Common/Serialize/Packfile/hkPackfileReader.h>

class hkpShapePhantom;
class hkpCharacterProxy;
class hkpCharacterContext;

class hkaSkeleton;
class hkaAnimatedSkeleton;
class hkEaseControl;
class hkaPose;

class CharacterAnimationDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		CharacterAnimationDemo(hkDemoEnvironment* env);
		
		~CharacterAnimationDemo();

		hkDemo::Result stepDemo();

	protected:

		// Simulation methods
		virtual void getStateMachineInput(MyStateMachineInput& input) const;
		virtual void stepWorldAndController(const MyStateMachineOutput& output);
		
		// Displays the current state in the screen
		virtual void displayCurrentState() const;

		// Updates the camera 
		virtual void updateCamera(const MyStateMachineOutput& output) const;

		// Gets the pose from the girl, based on controller position
		virtual void getCharacterPose (const MyStateMachineOutput& output, hkaPose& poseOut) const;

		// Renders a pose of the girl
		virtual void renderPose (const hkaPose& pose) const;
		
		// Z is up
		const hkVector4 m_up;

		// Loaded data
		class hkLoader* m_loader; // Animation
		hkPackfileReader::AllocatedData* m_loadedData; // Physics

		class hkaMeshBinding** m_skinBindings;
		hkInt32 m_numSkinBindings;

		/*
		** Character Controller stuff
		*/
		hkpShapePhantom* m_phantom;
		hkpShape* m_standShape;

		// The context for this character in the animation state machine
		MyStateMachineContext m_characterContext;

		// The state machine that govers the animation of the character
		MyStateMachine* m_stateMachine;

		hkBool m_forcedOnTrackball;

};

#endif // HK_CHARACTERANIMATIONDEMO_H

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
