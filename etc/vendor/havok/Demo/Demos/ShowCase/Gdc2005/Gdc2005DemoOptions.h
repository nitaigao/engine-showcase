/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GDC_2005_DEMO_OPTIONS_H
#define HK_GDC_2005_DEMO_OPTIONS_H

struct hkGdcDisplayOptions
{
	public:

		HK_DECLARE_REFLECTION();

		hkGdcDisplayOptions() :
			m_originalPose(false), m_finalPose(false),
			m_proxy(false), m_ragdoll(false), m_skinning(true), m_shadows(true), m_lightmaps(true),m_flashlight(false),
			m_cameraMode (TRACKING), m_cameraGain(0.1f), m_showState(false), m_showIkInfo (false)
			{}

		hkBool m_originalPose;
		hkBool m_finalPose;
		hkBool m_proxy;
		hkBool m_ragdoll;
		hkBool m_skinning;
		hkBool m_shadows;
		hkBool m_lightmaps;
		hkBool m_flashlight;

		enum CameraMode
		{
			TRACKING,
			CLOSEST,
			FREE = -1

		};

		hkEnum<CameraMode,hkInt8> m_cameraMode;

		hkReal m_cameraGain;

		hkBool m_showState;

		hkBool m_showIkInfo;
};


struct hkGdcProxyOptions
{
	public:

		HK_DECLARE_REFLECTION();

		hkGdcProxyOptions() :  m_height(1.7f), m_radius(0.4f), m_jumpImpulse(5.5f), m_friction(0.9f), m_strength(1.0f), m_safeFallTime(1.0f),
			m_verticalGain(0.2f), m_horizontalGain(0.8f), m_maxVerticalSeparation(5.0f), m_maxHorizontalSeparation(0.15f),
			m_pushIfFootInAir(false) {}

		float m_height;
		float m_radius;
		float m_jumpImpulse;
		float m_friction;
		float m_strength;
		float m_safeFallTime;
		float m_verticalGain;
		float m_horizontalGain;
		float m_maxVerticalSeparation;
		float m_maxHorizontalSeparation;
		hkBool m_pushIfFootInAir;
};

struct hkGdcIkOptions
{
	public:

		HK_DECLARE_REFLECTION();

		hkGdcIkOptions() :
			m_footIk(true),
			m_footIkOnOffGain(0.2f), m_footIkAscendingGain(0.35f), m_footIkStandAscendingGain (0.6f),
			m_footIkDescendingGain(0.3f), m_footIkMaxFootHeightMS(0.7f), m_footIkMinFootHeightMS(-0.1f),
			m_cosKneeLimit(0.95f), m_pelvisUpDownBias(0.95f), m_pelvisFeedback(0.1f),
			m_lookAtMode(LOOKAT_ALIVE), m_lookAtTarget(LOOKAT_CAMERA),
			m_lookAtLimit(HK_REAL_PI / 4.0f), m_lookAtGain(0.05f), m_lookAtTargetGain(0.2f),
			m_handIkMode(HANDIK_STANDING), m_handIkReachGain(0.3f), m_handIkLeaveGain(0.19f), m_handIkMoveGain(0.085f)
			{}

		hkBool m_footIk;
		float m_footIkOnOffGain;
		float m_footIkAscendingGain;
		float m_footIkStandAscendingGain;
		float m_footIkDescendingGain;
		float m_footIkMaxFootHeightMS;
		float m_footIkMinFootHeightMS;
		float m_cosKneeLimit;
		float m_pelvisUpDownBias;
		float m_pelvisFeedback;

		enum LookAtMode
		{
			LOOKAT_OFF,
			LOOKAT_STANDING,
			LOOKAT_ALIVE,
			LOOKAT_ALWAYS,
		};

		enum LookAtTarget
		{
			LOOKAT_CAMERA,
			LOOKAT_LAST_OBJECT,
		};

		hkEnum<LookAtMode,hkInt8> m_lookAtMode;
		hkEnum<LookAtTarget, hkInt8> m_lookAtTarget;

		float m_lookAtLimit;
		float m_lookAtGain;
		float m_lookAtTargetGain;

		enum HandIkMode
		{
			HANDIK_OFF ,		// No hand Ik
			HANDIK_STANDING,	// Hand IK during standing state
			HANDIK_ALWAYS		// Always do hand Ik
		};

		hkEnum<HandIkMode,hkInt8> m_handIkMode;

		float m_handIkReachGain;	// Speed to reach desired point
		float m_handIkLeaveGain;	// Speed to leave desired point
		float m_handIkMoveGain;		// Speed of moving to the next desired point

};

struct hkGdcMatchOptions
{
	public:
		HK_DECLARE_REFLECTION();

		hkGdcMatchOptions() : m_force( 40.0f ), m_tau(0.8f), m_proportinalRecovery(2.0f), m_constantRecovery(1.0f), m_matchBlendSpeed(0.5f), m_hysterisis(0.2f), m_showDeactivation(false) {}

		float m_force;
		float m_tau;
		float m_proportinalRecovery;
		float m_constantRecovery;

		float m_matchBlendSpeed;
		float m_hysterisis;

		hkBool m_showDeactivation;
};

struct hkGdcGetupOptions
{
	public:
		HK_DECLARE_REFLECTION();

		hkGdcGetupOptions() : m_autoGetup(0.0f), m_getupBlendSpeed(0.5f), m_dieAnimation(false) {}

		float m_autoGetup;
		float m_getupBlendSpeed;
		hkBool m_dieAnimation;
};

struct hkGdcGetHitOptions
{
	public:
		HK_DECLARE_REFLECTION();

		struct Velocity
		{
			HK_DECLARE_REFLECTION();

			Velocity() :
				m_velocityGain(0.6f) {}

			float m_velocityGain;

		};


		struct Position
		{
			HK_DECLARE_REFLECTION();

			Position() :
				m_positionGain(0.33f),
				m_positionMaxLinearVelocity(1.4f),
				m_positionMaxAngularVelocity(1.8f) {}

			float m_positionGain;
			float m_positionMaxLinearVelocity;
			float m_positionMaxAngularVelocity;

		};

		struct Snap
		{
			HK_DECLARE_REFLECTION();

			Snap() :
				m_snapGain(0.1f),
				m_snapMaxLinearVelocity(0.3f),
				m_snapMaxAngularVelocity(0.3f),
				m_snapMaxLinearDistance(0.03f),
				m_snapMaxAngularDistance(0.1f) {}

			float m_snapGain;
			float m_snapMaxLinearVelocity;
			float m_snapMaxAngularVelocity;
			float m_snapMaxLinearDistance;
			float m_snapMaxAngularDistance;

		};

		hkGdcGetHitOptions() :
			m_fixLegs(true),
			m_showStress(true),
			m_ragdollFeedback(1.0f),
			m_hierarchyGain(0.17f),
			m_dampVelocity(0.0f),
			m_accelerationGain(1.0f) {}

	hkBool m_fixLegs;
	hkBool m_showStress;

	float m_ragdollFeedback;
	float m_hierarchyGain;
	float m_dampVelocity;
	float m_accelerationGain;
	struct Velocity m_velocity;
	struct Position m_position;
	struct Snap m_snap;



};

struct hkGdcPhysicsOptions
{
	public:

		HK_DECLARE_REFLECTION();

		hkGdcPhysicsOptions()
			: m_gravity(10), m_throw(DEBRIS), m_massMultiplier(1.0f), m_maxNumBodies(100),
			  m_launchVelocity(20.0f), m_shotStrength(50.0f),
			  m_brickWallWidth(5), m_brickWallHeight(5) { }

		enum Objects
		{
			LEG = 0,
			BOXES,
			SKULL,
			PULLEY,
			SPEARS,
			CLUB,
			DEBRIS,
			RANDOM
		};

		float m_gravity;
		hkEnum<Objects,hkInt8> m_throw;
		float m_massMultiplier;
		int m_maxNumBodies;
		float m_launchVelocity;
		float m_shotStrength;
		int m_brickWallWidth; // in bricks
		int m_brickWallHeight; // in bricks
		hkBool m_rebuildWall; // rebuild the wall, now (will be reset to false when done next frame)
};

struct hkGdcMiscOptions
{
	public:

		HK_DECLARE_REFLECTION();

		hkGdcMiscOptions() : m_walkToRunSpeed(0.01f), m_settings(NONE) {}

		float m_walkToRunSpeed;

		enum FileCommand
		{
			NONE,
			SAVE,
			LOAD,
			DEFAULT
		};

		hkEnum<FileCommand,hkInt8> m_settings;
};


class Gdc2005DemoOptions
{
	public:

	HK_DECLARE_REFLECTION();

	Gdc2005DemoOptions() : m_interactiveTweaking(true) {}

	struct hkGdcDisplayOptions	m_Display;
	struct hkGdcProxyOptions	m_Proxy;
	struct hkGdcIkOptions		m_IK;
	struct hkGdcMatchOptions	m_Matching;
	struct hkGdcGetupOptions	m_GetUp;
	struct hkGdcGetHitOptions	m_GetHit;
	struct hkGdcPhysicsOptions	m_Physics;
	struct hkGdcMiscOptions		m_Misc;
	hkBool m_interactiveTweaking;
};

#endif // HK_GDC_2005_DEMO_OPTIONS_H

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
