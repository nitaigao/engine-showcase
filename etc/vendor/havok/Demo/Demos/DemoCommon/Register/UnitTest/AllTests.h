/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_HAVOK_KEYCODE
#	error please include <Common/Base/KeyCode.h> before including this file
#endif

#include <Common/Base/UnitTest/Tests.h>
#include <Common/Serialize/UnitTest/Tests.h>
#include <Common/SceneData/UnitTest/Tests.h>

#ifdef USING_HAVOK_ANIMATION
#  include <Animation/Animation/UnitTest/Tests.h>
#  include <Animation/Internal/UnitTest/Tests.h>
#endif

#ifdef USING_HAVOK_PHYSICS
#  include <Physics/Collide/UnitTest/Tests.h>
#  include <Physics/Dynamics/UnitTest/Tests.h>
#  include <Physics/Internal/UnitTest/Tests.h>
#  include <Physics/Utilities/UnitTest/Tests.h>
#endif

#ifdef USING_HAVOK_CLOTH
#  include <Cloth/Cloth/UnitTest/Tests.h>
#endif

#ifdef USING_HAVOK_DESTRUCTION
#  include <Destruction/Destruction/UnitTest/Tests.h>
#  include <Destruction/Internal/UnitTest/Tests.h>
#endif



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
