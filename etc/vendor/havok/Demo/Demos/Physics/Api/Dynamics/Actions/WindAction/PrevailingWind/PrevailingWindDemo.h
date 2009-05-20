/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// This simple example shows how to set up an action that applies to a single object.
// It applies a simple reverse gravity force on the object and is an example of a class
// derived from UnaryAction.

#ifndef PREVAILING_WIND_DEMO_H
#define PREVAILING_WIND_DEMO_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Physics/Utilities/Actions/Wind/hkpWindAction.h>
#include <Physics/Utilities/Actions/Wind/hkpPrevailingWind.h>

class PrevailingWindDemo : public hkDefaultPhysicsDemo
{
	public:
		PrevailingWindDemo( hkDemoEnvironment* env );
		~PrevailingWindDemo();
		virtual Result stepDemo();
	private:
		void createPalmTree ( hkpWorld* world, const hkpWind* wind, const hkVector4& pos );
		void createWindmill ( hkpWorld* world, const hkpWind* wind, const hkVector4& pos );
		hkpPrevailingWind* m_wind;
};

#endif // PREVAILING_WIND_DEMO_H

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
