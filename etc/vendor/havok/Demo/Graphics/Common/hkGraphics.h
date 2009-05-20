/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_PRECOMPILE_H
#define HK_GRAPHICS_PRECOMPILE_H

// This is the precompile header for hkgraphics.

#include <Common/Base/hkBase.h>

// Uncomment this line to enable the graphics timers.
//#define HK_GRAPHICS_TIMERS_ENABLED

#if defined(HK_GRAPHICS_TIMERS_ENABLED)
#	include <Common/Base/Monitor/hkMonitorStream.h>
#	define HKG_TIMER_BEGIN			HK_TIMER_BEGIN
#	define HKG_TIMER_END			HK_TIMER_END
#	define HKG_TIMER_NAMED_END		HK_TIMER_NAMED_END
#	define HKG_TIMER_BEGIN_LIST		HK_TIMER_BEGIN_LIST
#	define HKG_TIMER_SPLIT_LIST		HK_TIMER_SPLIT_LIST
#	define HKG_TIMER_END_LIST		HK_TIMER_END_LIST
#	define HKG_TIME_CODE_BLOCK		HK_TIME_CODE_BLOCK
#	define HKG_MONITOR_ADD_VALUE    HK_MONITOR_ADD_VALUE
#else
#	define HKG_TIMER_BEGIN( NAME, OBJECT )		/* nothing */
#	define HKG_TIMER_END()						/* nothing */
#	define HKG_TIMER_NAMED_END(NAME)			/* nothing */
#	define HKG_TIMER_BEGIN_LIST( NAME, OBJECT )	/* nothing */
#	define HKG_TIMER_SPLIT_LIST( NAME )			/* nothing */
#	define HKG_TIMER_END_LIST()					/* nothing */
#	define HKG_TIME_CODE_BLOCK( NAME, OBJECT )	/* nothing */
#	define HKG_MONITOR_ADD_VALUE( NAME, VALUE, MONITOR_TYPE )	/* nothing */
#endif




#include <Common/Base/System/Io/OStream/hkOStream.h>
#include <Graphics/Common/Math/hkgMath.h>
#include <Graphics/Common/hkgObject.h>
#include <Graphics/Common/DisplayContext/hkgDisplayContext.h>

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
