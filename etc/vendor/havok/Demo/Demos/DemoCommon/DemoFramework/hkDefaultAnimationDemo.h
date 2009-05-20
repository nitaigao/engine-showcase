/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DEFAULTANIMATIONDEMO_H
#define HK_DEFAULTANIMATIONDEMO_H

#include <Demos/demos.h>


class hkDefaultAnimationDemo : public hkDefaultDemo 
{
	public:

			/// The constructor
		hkDefaultAnimationDemo(hkDemoEnvironment* env);

			/// The destructor
		virtual ~hkDefaultAnimationDemo();

		virtual Result stepDemo();

		

		virtual void setupContexts(hkArray<hkProcessContext*>& contexts);

	protected:

			/// Called by mouseDown, should return true if it picks an object
		virtual hkBool objectPicked( const hkgDisplayObject* displayObject, const hkVector4& worldPosition, int geomIndex  );

			/// Called by mouseUp
		virtual void objectReleased();

			/// Called by mouseDrag
		virtual void objectDragged( const hkVector4& newWorldSpacePoint );

			/// The context.
		//hkAnimationContext* m_animationViewersContext;


};



#endif // HK_DEFAULTANIMATIONDEMO_H

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
