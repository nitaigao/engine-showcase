/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_TankModel_H
#define HK_TankModel_H

#include <Physics/Vehicle/Camera/hkp1dAngularFollowCam.h>
#include <Physics/Utilities/Collide/ContactModifiers/SurfaceVelocity/hkpSurfaceVelocityUtil.h>


class TankModel
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, TankModel);

		TankModel( hkpWorld* world, const hkVector4& position, hkReal mass );

		~TankModel();

		void init( hkpWorld* world );

		void stepTank( hkDemoEnvironment* environment, hkReal deltaTime );

		void updateCamera( hkDemoEnvironment* environment,  hkReal timestep );

	public:
		hkpRigidBody* m_skiL;
		hkpRigidBody* m_skiR;
		hkpRigidBody* m_body;
		hkpRigidBody* m_tower;
		hkpRigidBody* m_gun;

		hkReal m_rotations;
		hkReal m_lastAngle;

		hkReal m_directionAngle;
		hkReal m_upAngle;

		class hkpLimitedHingeConstraintData* m_towerMotor;
		class hkpLimitedHingeConstraintData* m_gunMotor;

		hkVector4  m_currentVelocity;
		hkpSurfaceVelocityUtil* m_left;
		hkpSurfaceVelocityUtil* m_right;

		hkp1dAngularFollowCam m_camera;


};

#endif // HK_TANKDEMO_H



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
