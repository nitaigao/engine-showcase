/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_WINDOW_MANAGER_PC_H
#define HK_GRAPHICS_WINDOW_MANAGER_PC_H

#include <Graphics/Ogl/Pc/Window/hkgWindowPC.h>
#include <Common/Base/Container/PointerMap/hkPointerMap.h> 

//
// It is not Havok mem managed as we
// may need to have the manager present after 
// hkBaseSystem:: quit just to tell the wndproc 
// that the havok managed window system has shut down (hkgWindow list is empty)
// It should not be a big mem resource anyway and is only on PC.
// Add, find, and remove use a critical section for the map access
//

class hkgWindowManagerPC : public hkReferencedObject, public hkSingleton<hkgWindowManagerPC>
{
public:
	
	// There should be no need to create more than one of these,
	// and that one is created globally.

	hkgWindowManagerPC();
	~hkgWindowManagerPC();

	// will not reference actual hkgWindow as it 
	// will assume that windows are only added and 
	// removed under hkgWindow dtor and ctor
	bool addWindow( HWND handle, hkgWindowPC* w );
	bool setHWND( HWND handle, hkgWindowPC* w );

	hkgWindowPC* findWindow( HWND handle ) const;
	
	bool removeWindow( HWND handle );
	
protected:

	hkPointerMap<void*, hkgWindowPC*> m_hwndMap; 
	HANDLE m_hMutex;
};

#endif // HK_GRAPHICS_WINDOW_MANAGER_PC_H

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
