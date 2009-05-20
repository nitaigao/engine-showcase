/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// This demo shows how to stream in/out MOPP data (compressed bounding volume collision data)
// used by hkShapeCollections (e.g. a hkpMeshShape for arbitrary triangle soups). Once data has been
// calculated it can be streamed out to a file, and this file can be loaded at runtime to avoid
// the overhead of recomputation (which can be costly if the MOPP is optimized for runtime speed
// of collision detection).
//
// The demo uses the MOPP streaming methods found in hkpMoppCodeStreamer.h in
// hkutilities to read/write the hkpMoppCode to the Archive.
// If the data is streamed in correctly, the demo should consist of 10 boxes falling on a triangulated
// landscape.

#ifndef HK_MoppCodeStreamingDemo_H
#define HK_MoppCodeStreamingDemo_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

class hkpMoppCode;
class hkpExtendedMeshShape;


class MoppCodeStreamingDemo : public hkDefaultPhysicsDemo
{
	public:

		MoppCodeStreamingDemo(hkDemoEnvironment* env);

		

	protected:

		// Creates a mesh shape by using the memory defined by m_vertices/m_indices arrays
		hkpExtendedMeshShape* createMeshShape();

		// saves a mopp code
		void saveMoppCode( hkpMoppCode* moppCode, const char* moppFilename  );

		// loads a previously saved mopp code
		hkpMoppCode*	loadMoppCode(const char* moppFilename );


	private:

		hkArray<float> m_vertices;
		hkArray<hkUint16> m_indices;
};

#endif	// HK_MoppCodeStreamingDemo_H

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
