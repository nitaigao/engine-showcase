/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2008 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef BALL_GUN_H
#define BALL_GUN_H


#include <Common/Base/Container/Queue/hkQueue.h>
#include <Demos/DemoCommon/Utilities/Character/Gun/FirstPersonGun.h>


class BallGun : public FirstPersonGun
{
	public:

		BallGun( hkgDisplayContext* ctx, hkgDisplayHandler* dspHandler, int numBullets = 100, const hkgDisplayObject* dspShape = HK_NULL );
		BallGun( int numBullets, const char* blueprintName );
		~BallGun();

		virtual void fireGun( hkDemoEnvironment* env, hkpWorld* world, const hkTransform& viewTransform );

		virtual char* getName() { return "BallGun"; }

		virtual void reset(hkpWorld* world);

	public:

		hkReal m_bulletRadius;
		hkReal m_bulletVelocity;
		hkReal m_bulletMass;
		hkReal m_damageMultiplier;
		hkVector4 m_bulletOffsetFromCenter;

		int m_maxBulletsInWorld;
		hkQueue<class hkpRigidBody*> m_addedBodies;
		hkQueue<hkgDisplayObject*>   m_addedDisplayObjects;

		const char* m_blueprintName;
		const hkgDisplayObject* m_displayShape;
		hkgDisplayContext* m_context;
		hkgDisplayHandler* m_displayHandler;
};



#endif // BALL_GUN_H

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20080925)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2008
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
