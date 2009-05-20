/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef GDC_DIVE_STATE_H
#define GDC_DIVE_STATE_H

#include <Demos/ShowCase/Gdc2005/StateMachine/GdcState.h>
#include <Demos/ShowCase/Gdc2005/StateMachine/GdcStateTypes.h>

class GdcDiveState : public GdcState
{
public:

	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		/// Return the state type
	virtual hkUint32 getId() const { return GDC_DIVE_STATE; }

	/// Transition to this state.
	virtual void enterState( hkUint32 prevState, class GdcAnimationMachine* animMachine ) const;

	/// Transition from this state.
	virtual void leaveState( hkUint32 nextState, class GdcAnimationMachine* animMachine ) const;

	/// Update this state
	virtual void update( hkReal timestep, const struct GdcStateInput* input ) const;

};

#endif // GDC_DIVE_STATE_H

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
