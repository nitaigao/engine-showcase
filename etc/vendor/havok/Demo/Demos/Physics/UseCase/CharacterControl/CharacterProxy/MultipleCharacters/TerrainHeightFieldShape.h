/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldShape.h>
#include <Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldBaseCinfo.h>

/// This class implements looking the height lookup table
/// Here we will go one step further and make sure the shape
/// is serializable too (write a short xml description, add the getClass method, along with the get and updateFrom cinfo).


class TerrainHeightFieldShape: public hkpSampledHeightFieldShape
{
	public:

		TerrainHeightFieldShape( const hkpSampledHeightFieldBaseCinfo& ci, hkReal* data ): hkpSampledHeightFieldShape(ci) , m_data(data)
		{
		}

			/// Generate a rough terrain
		HK_FORCE_INLINE hkReal getHeightAtImpl( int x, int z ) const
		{
			return m_data[x * m_zRes + z];
		}

		///		this should return true if the two triangles share the edge p00-p11
		///     otherwise it should return false if the triangles sphere the edge p01-p10
		HK_FORCE_INLINE hkBool getTriangleFlipImpl() const
		{	
			return false;
		}

		virtual void collideSpheres( const CollideSpheresInput& input, SphereCollisionOutput* outputArray) const
		{
			hkSampledHeightFieldShape_collideSpheres(*this, input, outputArray);
		}

		hkReal* m_data;
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
