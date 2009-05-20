/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// This demo illustrates the speed of our raycasting by raytracing a teapot in realtime.

#ifndef HK_RayTraceDemo_H
#define HK_RayTraceDemo_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Common/Base/System/Stopwatch/hkStopwatch.h>

class hkpMeshShape;
class hkpMoppCode;
class hkpMoppBvTreeShape;
class hkgTexture;
class hkgDisplayContext;


class RayTraceDemo: public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		RayTraceDemo(hkDemoEnvironment* env);

		~RayTraceDemo();
		
		virtual Result stepDemo(); 

		virtual void postRenderDisplayWorld(hkgViewport* v);

	protected:

		void drawTexture( hkgTexture* texture );

		// Alter these to change the width and height (and speed!)
		enum { CANVAS_WIDTH = 64 };
		enum { CANVAS_HEIGHT = 64 };
		enum { TEXTURE_SCALE = 2 };

		// The texture for the image
		hkgTexture* m_texture;

		// Used to draw the texture in immediate mode
		hkgDisplayContext* m_context;

		// The teapot as a mopp shape
		const hkpShape*		m_shape;

		// raytraced image as a 32 bit texture
		unsigned char	*m_canvasData;

		// Statistics
		int				m_numPrimaryRays;
		int				m_numShadowRays;

		// Timer per frame
		hkStopwatch		m_stopwatch;

		hkBool			m_textureRealized;

		class LandscapeContainer* m_landscapeContainer;
		int m_rotateCounter;

	private:

		// The stack size is increased for this demo. The old stack gets saved and restored.
		hkThreadMemory::Stack m_oldStack;
		void*  m_stackBuffer;
};

#endif // HK_RayTraceDemo_H


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
