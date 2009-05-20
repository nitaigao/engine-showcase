/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

hkgDisplayObject::~hkgDisplayObject()
{
	int ng;
	int i;

	ng = m_solidGeoms.getSize();
	for (i =0; i < ng;++i)
		m_solidGeoms[i]->release();

	ng = m_alphaGeoms.getSize();
	for (i =0; i < ng;++i)
		m_alphaGeoms[i]->release();

	ng = m_mixedGeoms.getSize();
	for (i =0; i < ng;++i)
		m_mixedGeoms[i]->release();

}

inline void hkgDisplayObject::setStatusFlags( HKG_DISPLAY_OBJECT_STATUS f)
{
	m_status = f;
}

inline HKG_DISPLAY_OBJECT_STATUS hkgDisplayObject::getStatusFlags() const
{
	return m_status;
}

inline void hkgDisplayObject::setTransform( const float* t, bool isIdentity)
{
	HK_ASSERT(0x4367ceee,  t != HK_NULL );
	hkgMat4Copy( m_trans, t );


	if (!m_zeroOffset)
	{
		hkgMat4RotateAndAdd( m_trans, m_offset );
		m_identityTrans = false;
	}
	else
		m_identityTrans = isIdentity;
}

inline void hkgDisplayObject::setTransformAligned( hkTransform& t )
{
	hkgMat4CleanBottomRow( (float*)(&t) );

	hkString::memCpy16<64>(m_trans, &t);

	m_identityTrans = false;
}



inline const float*	hkgDisplayObject::getTransform() const
{
	return m_trans;
}

inline bool hkgDisplayObject::getIdentityFlag() const
{
	return m_identityTrans;
}

inline void hkgDisplayObject::setIdentityFlag(bool t)
{
	m_identityTrans = t;
}

inline void hkgDisplayObject::setTransToIdentity()
{
	hkString::memSet( m_trans, 0, 16*sizeof(float));
	m_trans[0] = m_trans[5] = m_trans[10] = m_trans[15] = 1.0f;
	m_identityTrans = true;
}

inline void hkgDisplayObject::addGeometry(hkgGeometry* g)
{
	g->reference();

	HKG_GEOMETRY_TRANSPARENCY t = g->getTransparencyType();
	m_geomTransp |= t;

	switch (t)
	{
		case HKG_GEOMETRY_TRANS_OPAQUE:
			m_solidGeoms.pushBack(g);
			break;
		case HKG_GEOMETRY_TRANS_TRANSPARENT:
			m_alphaGeoms.pushBack(g);
			break;
		default : // mixed or unknown
			m_mixedGeoms.pushBack(g);
			break;
	}
}

inline int hkgDisplayObject::getNumGeometry() const
{
	return ( m_solidGeoms.getSize() + m_alphaGeoms.getSize() + m_mixedGeoms.getSize() );
}

inline hkgGeometry* hkgDisplayObject::getGeometry(int i)
{
	HK_ASSERT(0x1b9d35f7,  i >= 0 && i < getNumGeometry());
	int ss = m_solidGeoms.getSize();
	if (i < ss)
		return m_solidGeoms[i];
	else if ( i < (ss + m_alphaGeoms.getSize()) )
		return m_alphaGeoms[i-ss];
	else if ( i < (ss + m_alphaGeoms.getSize() + m_mixedGeoms.getSize()) )
		return m_mixedGeoms[i-(ss+m_alphaGeoms.getSize())];
	else
		return HK_NULL;
}

inline const hkgGeometry* hkgDisplayObject::getGeometry(int i) const
{
	return const_cast<hkgDisplayObject*>(this)->getGeometry(i);
}

inline hkgArray<hkgGeometry*>* hkgDisplayObject::remapIndex( int& index ) const
{
	HK_ASSERT(0x1290b865,  index < getNumGeometry() );

	int ss = m_solidGeoms.getSize();
	hkgArray<hkgGeometry*>* ga = HK_NULL;

	if (index < ss)
	{
		ga = const_cast<hkgArray<hkgGeometry*>*>( &m_solidGeoms );
	}
	else if ( index < (ss + m_alphaGeoms.getSize()) )
	{
		index -= ss;
		ga = const_cast<hkgArray<hkgGeometry*>*>( &m_alphaGeoms );
	}
	else
	{
		index -= (ss - m_alphaGeoms.getSize());
		ga = const_cast<hkgArray<hkgGeometry*>*>( &m_mixedGeoms );
	}

	return ga;
}

inline hkgGeometry* hkgDisplayObject::removeGeometry(int i, bool preserveOrder)
{
	int index = i;
	hkgArray<hkgGeometry*>* ga = remapIndex( index );

	hkgGeometry* g = (*ga)[index];
	if (preserveOrder)
		ga->removeAtAndCopy(index);
	else
		ga->removeAt(index);

	if (ga->getSize() == 0)
		computeTransparency();

	return g;
}

inline void hkgDisplayObject::setGeometry(int i, hkgGeometry* g)
{
	g->reference();
	
	int index = i;
	hkgArray<hkgGeometry*>* ga = remapIndex( index );

	hkgGeometry* oldGeom = (*ga)[index];
	oldGeom->release();

	(*ga)[index] = g;
}

inline hkgGeometry* hkgDisplayObject::searchAndRemoveByName( hkgArray<hkgGeometry*>* geomArray, const char* name, bool preserveOrder)
{
	int s = geomArray->getSize();
	int i = 0;

	while(i < s)
	{
		if ( hkString::strCmp(name, (*geomArray)[i]->getName().cString()) == 0 )
		{
			hkgGeometry* g = (*geomArray)[i];

			if (preserveOrder)
				geomArray->removeAtAndCopy(i);
			else
				geomArray->removeAt(i);

			if (geomArray->getSize() == 0)
				computeTransparency();

			return g;
		}
	}
	return HK_NULL;
}

inline hkgGeometry* hkgDisplayObject::removeGeometry(const char* name, bool preserveOrder)
{
	hkgGeometry* g = searchAndRemoveByName( &m_solidGeoms, name, preserveOrder );
	if (g)
		return g;

	g = searchAndRemoveByName( &m_alphaGeoms, name, preserveOrder );
	if (g)
		return g;

	g = searchAndRemoveByName( &m_mixedGeoms, name, preserveOrder );
	return g;
}


inline void hkgDisplayObject::setName( const char* s)
{
	m_name = s; // copy
}

inline const hkString& hkgDisplayObject::getName() const
{
	return m_name;
}

inline void hkgDisplayObject::setGeometryOffset( const float o[3] )
{
	hkgVec3Copy(m_offset, o);
	m_zeroOffset = 	(hkg_fabs(m_offset[0]) < 0.001f) &&
   					(hkg_fabs(m_offset[1]) < 0.001f) &&
					(hkg_fabs(m_offset[2]) < 0.001f);

}

inline const float* hkgDisplayObject::getGeometryOffset() const
{
	return m_offset;
}

inline float* hkgDisplayObject::getGeometryOffset()
{
	return m_offset;
}

inline void hkgDisplayObject::setOffsetToZero()
{
	hkgVec3Zero( m_offset );
	m_zeroOffset = true;
}

inline HKG_GEOMETRY_TRANSPARENCY hkgDisplayObject::getTransparencyType() const
{
	return m_geomTransp;
}

inline bool hkgDisplayObject::hasAABB() const
{
	return m_computedRadius > 0;
}

inline bool hkgDisplayObject::boundsVisibleInFrustum( const hkgCamera* camera )
{
	if (m_computedRadius < 0.001f)
		return true;

	if (!m_identityTrans) // get the world cent
	{
		float wc[3];
		hkgVec3Transform( wc, m_aabbCent, m_trans);
		return camera->sphereVisible( wc, m_computedRadius );
	}
	else
	{
		return camera->sphereVisible( m_aabbCent, m_computedRadius );
	}
}

const float* hkgDisplayObject::getAABBCentPtr() const
{
	return m_aabbCent;
}

const float* hkgDisplayObject::getAABBExtPtr() const
{
	return m_aabbExtent;
}

inline void hkgDisplayObject::setDrawLocalFrame( bool on )
{
	m_drawLocalFrame = on;
}

inline bool hkgDisplayObject::getDrawLocalFrame() const
{
	return m_drawLocalFrame;
}


inline hkgDisplayObject::hkgDisplayObject()
: m_forceNoZRead(false),
  m_forceNoZWrite(false),
  m_forceWireframe(false),
  m_forceDrawBothSides(false),
  m_geomTransp( HKG_GEOMETRY_TRANS_UNKNOWN ), 
  m_computedRadius(-1),
  m_status( HKG_DISPLAY_OBJECT_DYNAMIC ),
  m_userPtrType(USER_POINTER_UNKNOWN),
  m_userPtr(HK_NULL)
{
	setTransToIdentity();
	setOffsetToZero();
	m_drawLocalFrame = false;
	m_drawAABB = false;
}

inline hkgDisplayObject* hkgDisplayObject::defaultCreateInternal()
{
	return new hkgDisplayObject();
}

inline void hkgDisplayObject::setSpecialState( bool forceNoZRead, bool forceNoZWrite,	bool forceWireframe, bool forceBothSides)
{
	m_forceNoZRead = forceNoZRead;   
	m_forceNoZWrite = forceNoZWrite;  
	m_forceWireframe = forceWireframe; 
	m_forceDrawBothSides = forceBothSides;
}

inline void hkgDisplayObject::getSpecialState( bool& forceNoZRead, bool& forceNoZWrite, bool& forceWireframe, bool& forceBothSides) const
{
	forceNoZRead = m_forceNoZRead;   
	forceNoZWrite = m_forceNoZWrite;  
	forceWireframe = m_forceWireframe; 
	forceBothSides = m_forceDrawBothSides;
}

inline void hkgDisplayObject::setUserPointer(int type, void* d)
{
	m_userPtrType = type;
	m_userPtr = d;
}

inline void* hkgDisplayObject::getUserPointer() const
{
	return m_userPtr;
}

inline int hkgDisplayObject::getUserPointerType() const
{
	return m_userPtrType;
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
