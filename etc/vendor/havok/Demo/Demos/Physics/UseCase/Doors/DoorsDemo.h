/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DOORSDEMO_H
#define HK_DOORSDEMO_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Physics/Dynamics/Action/hkpUnaryAction.h>

class DoorSpring : public hkpUnaryAction
{

private:

	/// The hinge that this action acts on
	const hkpConstraintInstance* m_hinge;

	/// The strengh of the spring that forces the door back to a closed position
	hkReal m_strength;

	/// This damps the angular velocity of the door to stop oscillation
	hkReal m_damping;

	/// The axis about which the door is hung
	hkVector4 m_axis;
public:

	hkReal m_angVel;  // These variables are not needed, but it might be useful
	hkReal m_torque;  // to access them in your own code, if you want to trigger
	hkBool m_opening; // events based on the door swing.

	DoorSpring(const hkpConstraintInstance* hinge, const hkReal strength, const hkReal damping);
	void applyAction(const hkStepInfo& stepInfo);
	hkpAction* clone( const hkArray<hkpEntity*>& newEntities, const hkArray<hkpPhantom*>& newPhantoms ) const
	{
		HK_ASSERT2(0x0, 0, "Not written yet");
		return HK_NULL;
	}
};

class PlayerWalkAction : public hkpUnaryAction
{
  private:
	/// The speed that the character walks at
	hkReal		m_walkSpeed;

	/// The axis about which the player walks
	hkVector4	m_axis;

  public:

	PlayerWalkAction (hkpEntity* entity, const hkReal walkSpeed);

	void applyAction(const hkStepInfo& stepInfo);
	hkpAction* clone( const hkArray<hkpEntity*>& newEntities, const hkArray<hkpPhantom*>& newPhantoms ) const
	{
		HK_ASSERT2(0x0, 0, "Not written yet");
		return HK_NULL;
	}

};

class DoorsDemo : public hkDefaultPhysicsDemo
{
  public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

  private:
	/// The data that we load from the HKX file
	class hkPackfileData* m_data;

	/// The trigger volume for the powered door.
	class hkpAabbPhantom* m_doorTrigger;

	/// The powered constraints that we will use to open the
	/// door, one the player walks into the trigger volume
	hkArray<class hkpLimitedHingeConstraintData*> m_poweredHingeDatas;
	hkArray<class hkpConstraintInstance*> m_poweredHingeInstances;

	/// The current target angle for the powered doors.
	hkReal m_targetAngle;

	/// The time since a player has left the trigger volume (in seconds).
	hkReal m_timeSinceExit;

	/// The speed at which the door opens (in meters per second).
	hkReal m_doorSpeed;

	/// The amount of time the door stays open for,
	/// once the player has left the trigger volume (in seconds).
	hkReal m_doorDelayBeforeClosing;

  public:
		DoorsDemo(hkDemoEnvironment* env);
		~DoorsDemo(void);
		Result stepDemo( void );	
};


#endif // HK_DOORSDEMO_H

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
