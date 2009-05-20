/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_FACE_SET_DX9_H
#define HK_GRAPHICS_FACE_SET_DX9_H

#include <Graphics/Common/Geometry/FaceSet/hkgFaceSet.h>
#include <Graphics/Dx9/Shared/DisplayContext/hkgDisplayContextDX9.h>

class hkgFaceSetDX9 : public hkgFaceSet
{
public:

	static hkgFaceSet* createFaceSetDX9(hkgDisplayContext* context)
	{
		return new hkgFaceSetDX9(context);
	}	

	void render(hkgDisplayContext* context);

	// psuedo Instanced cpu style rendering
	void renderSetState(hkgDisplayContext* state);
	void renderPrims(hkgDisplayContext* state);
	void renderCleanState(hkgDisplayContext* state);

	inline LPDIRECT3DDEVICE9 getDevice();
	inline const LPDIRECT3DDEVICE9 getDevice() const;


    	virtual void initialize(hkArray<hkUint32>* reorderedVertices = HK_NULL);

protected:
		
	inline hkgFaceSetDX9(hkgDisplayContext* context);
	virtual ~hkgFaceSetDX9();

	void createIndexBuffer();

	LPDIRECT3DDEVICE9		m_device;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
	bool					m_hardwareIB;
	unsigned int			m_numIndices;

	struct IndexBufferInfo
	{
		unsigned int minIndex;
		unsigned int startIndex;
		unsigned int rangeSize;
	};

	hkArray<IndexBufferInfo> m_connectionInfo;
};

#include <Graphics/Dx9/Shared/Geometry/FaceSet/hkgFaceSetDX9.inl>

#endif // HK_GRAPHICS_FACE_SET_DX9_H

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
