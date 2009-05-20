/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// A demo which shows use of Phantom events to react to a ball when it passes through a Phantom 
// 'box' shape. While the ball is inside the phantom shape it is displayed in RED otherwise
// it is shown in GREEN.
//
// In order to do this we derive our own phantom class, MyPhantomShape, from hkpPhantomCallbackShape and
// provide custom implementations of the phantomEnterEvent(...) and phantomLeaveEvent(...) virtual
// methods. Doing so allows us manipulate the properties, such as the colour, of the ball object upon
// its entry to or exit from the phantom volume.

#ifndef HK_PHANTOMEVENTSDEMO_H
#define HK_PHANTOMEVENTSDEMO_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>


class PhantomEventsDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO );

		PhantomEventsDemo( hkDemoEnvironment* env );
};



#endif // HK_PHANTOMEVENTSDEMO_H


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
