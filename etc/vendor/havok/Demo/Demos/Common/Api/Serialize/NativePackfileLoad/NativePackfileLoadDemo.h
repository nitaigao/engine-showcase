/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_WORLDSNAPSHOTAPI_H
#define HK_WORLDSNAPSHOTAPI_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Common/Serialize/Util/hkNativePackfileUtils.h>

/// This demo shows how to quickly load native binary packfiles.
class NativePackfileLoadDemo : public hkDefaultPhysicsDemo
{
	public:

		NativePackfileLoadDemo( hkDemoEnvironment* env);
		~NativePackfileLoadDemo();

		Result stepDemo();

	protected:

			// A buffer (re-sizable array of chars) to store data only from native binary packfile
		hkArray<char> m_dataBuffer;
};

#endif // HK_WORLDSNAPSHOTAPI_H

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
