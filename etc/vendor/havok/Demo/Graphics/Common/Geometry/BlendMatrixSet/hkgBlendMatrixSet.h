/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_BLEND_MATRIX_SET
#define HK_GRAPHICS_BLEND_MATRIX_SET

class hkgDisplayContext;
/// A class to represent a group of matrices used for blending
class hkgBlendMatrixSet : public hkgReferencedObject
{
public:
	
	hkgBlendMatrixSet() {}

	static hkgBlendMatrixSet* (HK_CALL *create)(hkgDisplayContext* context);

	virtual int getNumMatrices() const = 0; 

		/// As the format of the hardware matrix is probably not exactly the same as HKG the
		/// set and get for the matrices is virtual and the storage is left up to the impl.
	virtual void addMatrix(const float* m, int referenceID) = 0;

		/// As the format of the hardware matrix is probably not exactly the same as HKG the
		/// set and get for the matrices is virtual and the storage is left up to the impl.
	virtual void setMatrix(int i, const float* m) = 0;

		/// Wipe the current set (resize to 0)
	virtual void clear() = 0;

		/// Bind the data for this matrix set, ie set these matrices as the current blending set
	virtual bool bind() = 0;

		/// To aid in setup and update from where ever these matrices come from
		/// we store a reference int for each matrix. Has to be the same size as getNumMatrices().
	hkArray<hkInt16> m_referenceIDs;
};

#endif // HK_GRAPHICS_BLEND_MATRIX_SET

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
