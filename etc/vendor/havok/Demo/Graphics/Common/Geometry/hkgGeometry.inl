/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Graphics/Common/hkGraphics.h>
#include <Common/Base/Monitor/hkMonitorStream.h>

hkgGeometry::hkgGeometry()
: m_computedAABB(false),
  m_trans(HKG_GEOMETRY_TRANS_UNKNOWN)

{
	m_aabbMin[0] = m_aabbMin[1] = m_aabbMin[2] =  3e+38f;
	m_aabbMax[0] = m_aabbMax[1] = m_aabbMax[2] = -3e+38f;
}

inline hkgGeometry* hkgGeometry::defaultCreateInternal()
{
	return new hkgGeometry();
}

inline const hkgMaterialFaceSet* hkgGeometry::getMaterialFaceSet(int i) const
{
	const int sfs = m_matSolidFaceSets.getSize();
	HK_ASSERT(0x48393328,  i < (sfs + m_matAlphaFaceSets.getSize()) );
	if ( i < sfs )
		return m_matSolidFaceSets[i];
	else
		return m_matAlphaFaceSets[i-sfs];
}

inline hkgMaterialFaceSet* hkgGeometry::getMaterialFaceSet(int i)
{
	const int sfs = m_matSolidFaceSets.getSize();
	HK_ASSERT(0x12109f05,  i < (sfs + m_matAlphaFaceSets.getSize()) );
	if ( i < sfs )
		return m_matSolidFaceSets[i];
	else
		return m_matAlphaFaceSets[i-sfs];
}

inline void  hkgGeometry::setName( const char* name )
{
	m_name = name;
}

const hkString& hkgGeometry::getName() const
{
	return m_name;
}

inline int hkgGeometry::addMaterialFaceSet( hkgMaterialFaceSet* ms )
{
	int index;
	if (ms->getMaterial() && ms->getMaterial()->isTransparent())
	{
		index = m_matAlphaFaceSets.getSize();
		m_matAlphaFaceSets.pushBack( ms );
		m_trans |= HKG_GEOMETRY_TRANS_TRANSPARENT;
	}
	else
	{
		index = m_matSolidFaceSets.getSize();
		m_matSolidFaceSets.pushBack( ms );
		m_trans |= HKG_GEOMETRY_TRANS_OPAQUE;
	}

	ms->reference();

	return index;
}

inline int hkgGeometry::getNumMaterialFaceSets() const
{
	return ( m_matAlphaFaceSets.getSize() + m_matSolidFaceSets.getSize() );
}

inline void hkgGeometry::render(hkgDisplayContext* context) const
{
	HKG_TIMER_BEGIN("hkgGeometry::render", this);

	hkgArray<hkgMaterialFaceSet*>* afs;
	switch (m_trans)
	{
		case HKG_GEOMETRY_TRANS_OPAQUE: // all opaque
			afs = const_cast<hkgArray<hkgMaterialFaceSet*>*>(&m_matSolidFaceSets);
			break;

		case HKG_GEOMETRY_TRANS_TRANSPARENT: // all transparent
			afs = const_cast<hkgArray<hkgMaterialFaceSet*>*>(&m_matAlphaFaceSets);
			break;

		default:
			mixedModeRender(context);
			HKG_TIMER_END();
			return;
	}


	HK_ASSERT(0x791ceaad, afs);

	int nmfs = afs->getSize();
	for (int i =0; i < nmfs; ++i)
	{
		hkgMaterialFaceSet* fs = (*afs)[i];
		fs->render(context);
	}

	HKG_TIMER_END();
}

inline HKG_GEOMETRY_TRANSPARENCY hkgGeometry::getTransparencyType() const
{
	return m_trans;
}


inline hkgMaterialFaceSet* hkgGeometry::removeMaterialFaceSet(int i)
{
	const int sfs = m_matSolidFaceSets.getSize();
	HK_ASSERT(0x1900bfc6,  i < (sfs + m_matAlphaFaceSets.getSize()) );

	hkgMaterialFaceSet* ret;

	if ( i < sfs )
	{
		ret = m_matSolidFaceSets[i];
		m_matSolidFaceSets.removeAt(i);
		if (m_matSolidFaceSets.getSize() == 0)
			m_trans &= ~HKG_GEOMETRY_TRANS_OPAQUE;
	}
	else
	{
		ret = m_matAlphaFaceSets[i-sfs];
		m_matAlphaFaceSets.removeAt(i-sfs);
		if (m_matAlphaFaceSets.getSize() == 0)
			m_trans &= ~HKG_GEOMETRY_TRANS_TRANSPARENT;
	}

	return ret;
}

void hkgGeometry::getAABB(float minE[3], float maxE[3]) const
{
	hkgVec3Copy( minE, m_aabbMin );
	hkgVec3Copy( maxE, m_aabbMax );
}

const float* hkgGeometry::getAABBMaxPtr() const
{
	return m_aabbMax;
}

const float* hkgGeometry::getAABBMinPtr() const
{
	return m_aabbMin;
}

bool hkgGeometry::hasAABB() const
{
	return m_computedAABB;
}


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
