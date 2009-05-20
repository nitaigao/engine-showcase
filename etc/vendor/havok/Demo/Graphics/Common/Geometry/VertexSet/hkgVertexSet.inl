/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline hkgVertexSet::hkgVertexSet(hkgDisplayContext* context)
: m_context(context), m_numVerts(0), m_stride( 3*sizeof(float) ), m_interleaved(true), m_dynamic(false), m_vertexFormat(HKG_VERTEX_FORMAT_NONE), m_currentLockType(HKG_LOCK_NONE), m_lockedBuf( HK_NULL )
{
	for (int i=0; i < HKG_VERTEX_COMPONENT_NUM; ++i)
	{
		m_storageOffsets[i] = 0;
		m_storageFormats[i] = HKG_VERTEX_STORAGE_NONE;
	}

	HKG_CHECK_CONTEXT_LOCK_STATUS(context);
}

inline hkgVertexSet::~hkgVertexSet()
{

}

inline int hkgVertexSet::getNumVerts() const
{
	return m_numVerts;
}

inline int hkgVertexSet::getStrideInBytes() const
{
	return m_stride;
}

inline HKG_VERTEX_FORMAT hkgVertexSet::getVertexFormat() const
{
	return m_vertexFormat;
}

inline bool hkgVertexSet::isDynamic() const
{
	return m_dynamic;
}

inline HKG_VERTEX_STORAGE hkgVertexSet::getVertexComponentFormat(HKG_VERTEX_COMPONENT component) const
{
	return m_storageFormats[component];
}

inline hkUlong hkgVertexSet::getVertexComponentOffset(HKG_VERTEX_FORMAT component) const
{
	return m_storageOffsets[component];
}

inline void* hkgVertexSet::getVertexComponentData( HKG_VERTEX_COMPONENT component, int index, hkUint8* data )
{
	if (!m_lockedBuf)
	{
		HK_WARN_ALWAYS(0x101, "You must lock the vertex buffer before being able to query or set data in it." );
		return HK_NULL;
	}

	HK_ASSERT2(0x32435ef, m_currentLockType & HKG_LOCK_READMASK, "You have not locked the buffer in the correct mode. You are reading from a write only buffer." );

#if !defined( HK_PLATFORM_PS2 )

	unsigned char* v;

	if (m_interleaved)
		v = m_lockedBuf + ( m_stride * index ) + m_storageOffsets[component];
	else
		v = m_lockedBuf + ( g_componentSizes[ m_storageFormats[component] ] * index ) + m_storageOffsets[component]; // always make m_storageOffsets relative to lockedBuf

	return v;

#else

	// since the PS2 uses a densely packed vertex format we'll copy the requested data into the supplied buffer
	if( ( component != HKG_VERTEX_COMPONENT_BLENDWEIGHTS ) && ( component != HKG_VERTEX_COMPONENT_BLENDINDICES ) )
	{
		return m_lockedBuf + (m_stride*index) + m_storageOffsets[component];
	}

	HK_ASSERT2( 0x105c7c93, data, "You must supply a pre-allocated 4 byte buffer to receive the blending data." );

	// the blend weights and indices need to be extracted from around the other components
	if( component == HKG_VERTEX_COMPONENT_BLENDWEIGHTS )
	{
		hkUint32 paddingValue = 0;

		// base of the vertex blend data ( p1 )
		hkUint32* vertex = (hkUint32*)( m_lockedBuf + ( m_stride * index ) ); vertex += 2;

		// ( p1 )
		{
			// read current value of the vertex padding component
			paddingValue = *vertex;

			// mask off the bits we ignore
			paddingValue &= 0x0000ff00;
			paddingValue = paddingValue >> 8;

			data[0] = paddingValue;
		}

		// ( p2 )
		{
			vertex++;

			// read current value of the vertex padding component
			paddingValue = *vertex;

			// mask off the bits we ignore
			paddingValue &= 0x0000ff00;
			paddingValue = paddingValue >> 8;

			data[1] = paddingValue;
		}

		// ( p3 )
		{
			vertex += 8;

			// read current value of the vertex padding component
			paddingValue = *vertex;

			// mask off the bits we ignore
			paddingValue &= 0x0000ff00;
			paddingValue = paddingValue >> 8;

			data[2] = paddingValue;
		}

		// ( p4 )
		{
			vertex += 4;

			// read current value of the vertex padding component
			paddingValue = *vertex;

			// mask off the bits we ignore
			paddingValue &= 0x0000ff00;
			paddingValue = paddingValue >> 8;

			data[3] = paddingValue;
		}

	}

	else if( component == HKG_VERTEX_COMPONENT_BLENDINDICES )
	{
		hkUint32 paddingValue = 0;

		// base of the vertex blend data ( p1 )
		hkUint32* vertex = (hkUint32*)( m_lockedBuf + ( m_stride * index ) ); vertex += 2;

		// ( p1 )
		{
			// read current value of the vertex padding component
			paddingValue = *vertex;

			// mask off the bits we ignore and store
			paddingValue &= 0x000000ff;
			data[0] = paddingValue;
		}

		// ( p2 )
		{
			vertex++;

			// read current value of the vertex padding component
			paddingValue = *vertex;

			// mask off the bits we ignore and store
			paddingValue &= 0x000000ff;
			data[1] = paddingValue;
		}

		// ( p3 )
		{
			vertex += 8;

			// read current value of the vertex padding component
			paddingValue = *vertex;

			// mask off the bits we ignore and store
			paddingValue &= 0x000000ff;
			data[2] = paddingValue;
		}

		// ( p4 )
		{
			vertex += 4;

			// read current value of the vertex padding component
			paddingValue = *vertex;

			// mask off the bits we ignore and store
			paddingValue &= 0x000000ff;
			data[3] = paddingValue;
		}

	}

	return data;
#endif
}

inline const void* hkgVertexSet::getVertexComponentData( HKG_VERTEX_COMPONENT component, int index, hkUint8* data ) const
{
	return const_cast<hkgVertexSet*>(this)->getVertexComponentData(component, index, data);
}

inline int hkgVertexSet::getComponentSize( HKG_VERTEX_COMPONENT component ) const
{
	return g_componentSizes[ m_storageFormats[component] ];
}

inline hkgDisplayContext* hkgVertexSet::getContext() const
{
	return m_context;
}

inline bool hkgVertexSet::isInterleaved() const
{
	return m_interleaved;
}

inline HKG_MATERIAL_VERTEX_HINT hkgVertexSet::getMaterialHints() const
{
	HKG_MATERIAL_VERTEX_HINT h = HKG_MATERIAL_VERTEX_HINT_NONE;

	if (m_vertexFormat & HKG_VERTEX_FORMAT_TANGENT)
	{
		h |= HKG_MATERIAL_VERTEX_HINT_TANGENTS;
	}

	if (m_vertexFormat & HKG_VERTEX_FORMAT_BLENDWEIGHTS)
	{
		h |= HKG_MATERIAL_VERTEX_HINT_BLENDING;
	}

	if (m_vertexFormat & HKG_VERTEX_FORMAT_COLOR)
	{
		h |= HKG_MATERIAL_VERTEX_HINT_VCOLOR;
	}

	return h;
}

inline int hkgVertexSet::getNumTextureCoordsUsed() const
{
	HKG_VERTEX_FORMAT tf = HKG_VERTEX_FORMAT_TEX0;
	int numT = 0;
	while ( (m_vertexFormat & tf) && (numT < 4) )
	{
		tf = (HKG_VERTEX_FORMAT)( tf << 1 ); // shift to next tex coord define.
		numT++;
	}

	if (m_vertexFormat & HKG_VERTEX_FORMAT_TANGENT)
	{
		numT++;
	}

	if (m_vertexFormat & HKG_VERTEX_FORMAT_BITANGENT)
	{
		numT++;
	}

	return numT;
}

bool hkgVertexSet::isContiguous() const
{
#if defined( HK_PLATFORM_PS2 )
    // On PS2 in with these formats we are apparently non contiguous
    return  (m_vertexFormat & (HKG_VERTEX_FORMAT_BLENDWEIGHTS | HKG_VERTEX_FORMAT_BLENDINDICES)) == 0;
#else
    return true;
#endif
}

inline void* hkgVertexSet::getVertexComponentStartPointer( HKG_VERTEX_COMPONENT component )
{
	HK_ASSERT2(0x101, m_lockedBuf, "You must lock the vertex buffer before being able to query or set data in it." );
#if defined( HK_PLATFORM_PS2 )
	if( (component == HKG_VERTEX_COMPONENT_BLENDWEIGHTS ) || ( component == HKG_VERTEX_COMPONENT_BLENDINDICES ) )
	{
		return HK_NULL;
	}
#endif
	return ( m_lockedBuf + m_storageOffsets[component] ); // offset may be to different buffer

}

inline const void* hkgVertexSet::getVertexComponentStartPointer( HKG_VERTEX_COMPONENT component ) const
{
	HK_ASSERT2(0x101, m_lockedBuf, "You must lock the vertex buffer before being able to query or set data in it." );
#if defined( HK_PLATFORM_PS2 )
	if( (component == HKG_VERTEX_COMPONENT_BLENDWEIGHTS ) || ( component == HKG_VERTEX_COMPONENT_BLENDINDICES ) )
	{
		return HK_NULL;
	}
#endif
	return ( m_lockedBuf + m_storageOffsets[component] ); // offset may be to different buffer
}

inline int hkgVertexSet::getVertexComponentStride( HKG_VERTEX_COMPONENT component ) const
{
#if defined( HK_PLATFORM_PS2 )
	if( (component == HKG_VERTEX_COMPONENT_BLENDWEIGHTS ) || ( component == HKG_VERTEX_COMPONENT_BLENDINDICES ) )
	{
		return 0;
	}
#endif

	if (m_interleaved)
	{
		return m_stride;
	}
	else
	{
		return g_componentSizes[ m_storageFormats[component] ];
	}
}

inline bool hkgVertexSet::isVertexComponentAligned( HKG_VERTEX_COMPONENT component, int alignment ) const
{
	HK_ASSERT2(0x101, m_lockedBuf, "You must lock the vertex buffer before being able to query or set data in it." );
#if defined( HK_PLATFORM_PS2 )
	if( (component == HKG_VERTEX_COMPONENT_BLENDWEIGHTS ) || ( component == HKG_VERTEX_COMPONENT_BLENDINDICES ) )
	{
		return false;
	}
#endif

	int stride = getVertexComponentStride( component );
	HK_ASSERT(0x0, (stride > 0));
	const hkUlong v0 = (const hkUlong)( m_lockedBuf + m_storageOffsets[component] ); // offset may be to different buffer
	const hkUlong v1 = (const hkUlong)( m_lockedBuf + m_storageOffsets[component] + stride );
	const hkUlong nmv0 = HK_NEXT_MULTIPLE_OF(alignment, v0);
	const hkUlong nmv1 = HK_NEXT_MULTIPLE_OF(alignment, v1);
	return (nmv0==v0) && (nmv1==v1);
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
