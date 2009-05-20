/* 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Common/Base/Types/Geometry/hkGeometry.h>

class MeshSimplificationDemo : public hkDefaultDemo
{

public:
	MeshSimplificationDemo( hkDemoEnvironment* env );
	~MeshSimplificationDemo();
	hkDemo::Result stepDemo();
	void doMeshSimplify();
	void drawGeometry(const hkGeometry&, int triColor) const;
	void handleInput();
	void displayInfoText();

	class hkxMesh* m_originalMesh;
	class hkLoader* m_loader;
	hkArray<class hkxMeshSection*> m_sections;

	int m_displayMeshIndex;	// Which mesh to draw
	int m_numDisplayMeshes; // Normally 2, can be 3 if we're running qslim too
	hkBool m_runQem;		// Run QEM process this frame?
	hkBool m_alwaysRunQem;	// Always run QEM each frame? Off by default, turn on for profiling

	hkGeometry m_geom[3];
	int m_numFaces, m_numVerts, m_originalNumFaces, m_originalNumVerts;

	hkMatrix4 m_worldTransform;

	hkVector4 m_up;
};

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
