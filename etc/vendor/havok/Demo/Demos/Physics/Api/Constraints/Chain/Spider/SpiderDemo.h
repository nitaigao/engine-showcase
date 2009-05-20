/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// This example shows how to create a hkGenericConstraint using the constraint kit
// that removes all linear and angular degrees of freedom (a 'fixed' constraint).

#ifndef HK_SPIDER_DEMO_H
#define HK_SPIDER_DEMO_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

class SpiderDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		SpiderDemo(hkDemoEnvironment* env);
		~SpiderDemo();

		Result stepDemo();

		struct Leg
		{
			hkReal m_lenA;
			hkReal m_lenB;
			class hkpConstraintChainInstance* m_constraint;
		};

		struct SpiderLayout
		{
			hkVector4 m_bodyHalfExtents;
			hkVector4 m_legHalfExtents;
			int m_numLegs;
			int m_filterInfo;
		};

		SpiderLayout m_spiderLayout;
		hkArray<Leg> m_legs; 

		hkReal m_time;
		hkReal m_velocity;
		hkReal m_leftFactor;
		hkReal m_rightFactor;

	protected:
		struct calcLegMatrizesIn
		{
			hkVector4 m_from;
			hkVector4 m_to;
			hkVector4 m_up;
			hkReal    m_lenA;
			hkReal    m_lenB;
		};

		void createSpider( hkpWorld* world, SpiderLayout& sl );
		void moveLeg( SpiderLayout& sl, SpiderDemo::Leg& leg, hkReal z, hkReal xf, hkReal time, hkReal vel );
		void moveSpider( SpiderLayout& sl, hkReal time, hkReal leftVel, hkReal rightVel);


		static void HK_CALL doLegIk( const calcLegMatrizesIn& in, hkVector4& pivotOut, hkRotation& mAOut, hkRotation& mBOut );

		static void HK_CALL buildLeg( hkpWorld* world, hkpRigidBody* rootBody, const calcLegMatrizesIn& in, int filterInfo, Leg& legOut );


};

#endif	// HK_CONSTRAINT_CHAIN_DEMO_H

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
