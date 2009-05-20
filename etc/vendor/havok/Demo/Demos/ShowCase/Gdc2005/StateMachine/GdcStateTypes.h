/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef GDC_STATE_TYPES_H
#define GDC_STATE_TYPES_H

enum GdcStateTypes
{
	GDC_WALK_STATE = 0,
	GDC_STAND_STATE,
	GDC_JUMP_STATE,
	GDC_IN_AIR_STATE,
	GDC_LAND_STATE,
	GDC_DYING_STATE,
	GDC_DEAD_STATE,
	GDC_DIVE_STATE,
	GDC_GETTING_UP_STATE,
	GDC_MAX_STATE
};

enum GdcAnimControls
{
	GDC_MOVE_CONTROL,
	GDC_IDLE_CONTROL,
	GDC_JUMP_CONTROL,
	GDC_IN_AIR_CONTROL,
	GDC_LAND_CONTROL,
	GDC_DYING_CONTROL,
	GDC_GET_UP_CONTROL,
	GDC_WALK_CONTROL,
	GDC_RUN_CONTROL,
	GDC_DIVE_CONTROL,
	GDC_DEATH_CONTROL,
	GDC_MAX_CONTROL
};

#endif // GDC_STATE_TYPES_H

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
