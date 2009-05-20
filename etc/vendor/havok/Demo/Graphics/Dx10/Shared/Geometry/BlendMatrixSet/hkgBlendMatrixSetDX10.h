/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_BLEND_MATRIX_SET_DX10
#define HK_GRAPHICS_BLEND_MATRIX_SET_DX10

#include <Graphics/Common/Geometry/BlendMatrixSet/hkgBlendMatrixSet.h>

/// A class to represent a group of matrices used for blending
class hkgBlendMatrixSetDX10 : public hkgBlendMatrixSet
{
public:
	
	static hkgBlendMatrixSet* createBlendMatrixSetDX10(hkgDisplayContext* context)
	{
		return new hkgBlendMatrixSetDX10(context);
	}	

	virtual int getNumMatrices() const { return m_matrices.getSize(); }

	virtual void addMatrix(const float* m, int referenceID);

	virtual void setMatrix(int i, const float* m);

	virtual bool bind();	

	virtual void clear() { m_matrices.setSize(0); m_referenceIDs.setSize(0); }

protected:

	struct BlendMatrix
	{
		float m[4][4];
	};

	hkgBlendMatrixSetDX10(hkgDisplayContext* context);

	hkgDisplayContext* m_context;
	hkArray<BlendMatrix> m_matrices;

};

#endif // HK_GRAPHICS_BLEND_MATRIX_SET_DX10

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
