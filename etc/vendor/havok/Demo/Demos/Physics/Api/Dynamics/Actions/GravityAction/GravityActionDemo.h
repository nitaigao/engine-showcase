/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// A demonstration the use of an action derived from hkpBinaryAction. A satellite body
// is attracted with a gravity action to a fixed planet body.
// NB: This is NOT the same as the gravity setting that is normally applied to the hkpWorld and
// set using hkpWorldCinfo::m_gravity

#ifndef HK_BINARY_ACTIONAPI_H
#define HK_BINARY_ACTIONAPI_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>


class GravityActionDemo : public hkDefaultPhysicsDemo
{
	public:
		GravityActionDemo(hkDemoEnvironment* env);
};

#endif // HK_BINARY_ACTIONAPI_H

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
