/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_GDC_2005_CPL_H
#define HK_GDC_2005_CPL_H

#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxyListener.h>

class GdcCharacterListener : public hkReferencedObject, public hkpCharacterProxyListener
{
public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

	GdcCharacterListener(const hkVector4& up, const Gdc2005DemoOptions* options) ;

	virtual void objectInteractionCallback(hkpCharacterProxy* proxy, const hkpCharacterObjectInteractionEvent& input, hkpCharacterObjectInteractionResult& output );

	void processConstraintsCallback( const hkArray<hkpRootCdPoint>& manifold, hkpSimplexSolverInput& input );

public:

	hkVector4 m_up;

	bool m_onMovingSurface;

	const Gdc2005DemoOptions* m_options;

};

#endif // HK_GDC_2005_CPL_H

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
