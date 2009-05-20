/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_RaycastAgainstProxy_H
#define HK_RaycastAgainstProxy_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

class RaycastAgainstProxy : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		RaycastAgainstProxy(hkDemoEnvironment* env);
		~RaycastAgainstProxy(void);
		Result stepDemo(); 

		// Helper methods
		void createBodies();

		hkReal m_time;

		class	hkProxyPhantom* m_pf;
		int		m_displayID;
		class hkPackfileData* m_packFileData;

			// helper variable
		hkReal m_angle;

};

#endif	// HK_RaycastAgainstProxy_H

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
