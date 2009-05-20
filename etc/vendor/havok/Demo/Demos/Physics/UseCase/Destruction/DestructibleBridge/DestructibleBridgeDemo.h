/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_DESTRUCTIBLE_BRIDGE_DEMO_H
#define HK_DESTRUCTIBLE_BRIDGE_DEMO_H



#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Physics/Utilities/Destruction/BreakOffParts/hkpBreakOffPartsUtil.h>


class DestructibleBridgeUtil;
class DestructibleMoppUtility;


enum DestructibleBridgeDemoType
{
	DESTRUCTIBLE_BRIDGE_DEMO_TYPE_FLYOVER  = 0,
	DESTRUCTIBLE_BRIDGE_DEMO_TYPE_CRASH = 1,
};

class DestructibleBridgeDemo : public hkDefaultPhysicsDemo, public hkpBreakOffPartsListener
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		 DestructibleBridgeDemo(hkDemoEnvironment* env);
		~DestructibleBridgeDemo();

	protected:

			// hkpBreakOffPartsListener implementation
		virtual hkResult breakOffSubPart(   const ContactImpulseLimitBreachedEvent& event, hkArray<hkpShapeKey>& keysBrokenOffOut, hkpPhysicsSystem& systemOut );

			// some helper functions
		void removeSubShapeFromDisplay(hkpRigidBody* moppRigidBody, hkpMoppBvTreeShape* moppShape, int subShapeKey);

		void setupTerrain();

		void setupAirplanes(DestructibleBridgeDemoType type, int numPlanes);

		hkDemo::Result stepDemo();

	protected:
			// a global destruction utility
		hkpBreakOffPartsUtil*	m_breakUtil;

			// a pointer to our terrain mesh
		hkReferencedObject*      m_terrainData;

		enum
		{
			HK_BROKEN_OFF_SHAPEKEY_PROP = 8011985,
		};
};



#endif // HK_DESTRUCTIBLE_BRIDGE_DEMO_H


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
