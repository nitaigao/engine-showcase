/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


inline hkgInstancedDisplayObject* hkgInstancedDisplayObjectDX9S::createInstancedDisplayObjectDX9S( hkgDisplayContext* ctx )
{
	return new hkgInstancedDisplayObjectDX9S( (hkgDisplayContextDX9S*)ctx );
}

hkgInstancedDisplayObjectDX9S::hkgInstancedDisplayObjectDX9S( hkgDisplayContextDX9S* ctx )
: hkgInstancedDisplayObject(), m_device( ctx->getDevice() ), m_createdOwnBuffers(false)
{
	// disable DX9S hardware instancing
	m_ownerWindow = (hkgWindowDX9S*)( ctx->getOwner() );
#ifdef HK_PLATFORM_XBOX360
	m_enableHardwareInstancing = false;
#else
	m_enableHardwareInstancing = m_ownerWindow->vertexShaderMajorVersion() >= 3;
#endif

	// no vertex buffer created
	m_instanceTransformRow[0] = HK_NULL;
	m_instanceTransformRow[1] = HK_NULL;
	m_instanceTransformRow[2] = HK_NULL;

	m_instanceTransformRowTexture[0] = HK_NULL;
	m_instanceTransformRowTexture[1] = HK_NULL;
	m_instanceTransformRowTexture[2] = HK_NULL;

	m_instanceTransformRowTextureSystem[0] = HK_NULL;
	m_instanceTransformRowTextureSystem[1] = HK_NULL;
	m_instanceTransformRowTextureSystem[2] = HK_NULL;


	setStatusFlags( getStatusFlags() | HKG_DISPLAY_OBJECT_DYNAMIC | HKG_DISPLAY_OBJECT_NOT_CULLED | HKG_DISPLAY_OBJECT_USER_AABB);

	m_ownerWindow->addD3DEventHandler(this);
	m_ownerWindow->addReference();
}

hkgInstancedDisplayObjectDX9S::~hkgInstancedDisplayObjectDX9S()
{
	if (m_instanceTransformRow[0])
	{
		m_instanceTransformRow[0]->Release();
		m_instanceTransformRow[1]->Release();
		m_instanceTransformRow[2]->Release();
	}

	if (m_instanceTransformRowTexture[0])
	{
		m_instanceTransformRowTexture[0]->Release();
		m_instanceTransformRowTexture[1]->Release();
		m_instanceTransformRowTexture[2]->Release();
	}

	if (m_instanceTransformRowTextureSystem[0])
	{
		m_instanceTransformRowTextureSystem[0]->Release();
		m_instanceTransformRowTextureSystem[1]->Release();
		m_instanceTransformRowTextureSystem[2]->Release();
	}

	m_ownerWindow->removeD3DEventHandler(this);
	m_ownerWindow->removeReference();
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
