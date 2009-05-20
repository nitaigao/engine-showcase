/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKG_GRAPHICS_BRIDGE2_VERTEX_BUFFER_H
#define HKG_GRAPHICS_BRIDGE2_VERTEX_BUFFER_H

#include <Common/SceneData/Mesh/hkxVertexBuffer.h>
#include <Graphics/Common/Geometry/VertexSet/hkgVertexSetDefines.h>

class hkgFaceSet;

/// The HKG vertex buffer to allow locking of the explicit HKG buffers.
class hkgVertexBufferWrapper : public hkgReferencedObject, public hkxVertexBuffer
{
public:
		
	HKG_DECLARE_CLASS_ALLOCATOR();

	hkgVertexBufferWrapper(HKG_LOCK_FLAG lockType);

	~hkgVertexBufferWrapper();

	virtual hkInt8* lock(int startIndex = 0, int length = -1);

	virtual void unlock();

	void setFaceSet(hkgFaceSet* f);

	class hkgVertexSet* getUnderlyingVertexData();

protected:

	HKG_LOCK_FLAG m_lockType;
	hkgFaceSet* m_faceSet;
};

#endif // HKG_GRAPHICS_BRIDGE2_VERTEX_BUFFER_H

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
