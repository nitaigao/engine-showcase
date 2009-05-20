/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_WINDOW_PC_H
#define HK_GRAPHICS_WINDOW_PC_H

#include <Graphics/Ogl/Shared/Window/hkgWindowOgl.h>


class hkgDirectJoystickPC;

class hkgWindowPC : public hkgWindowOgl
{
	public:

		static hkgWindow* createWindowPC();

		virtual bool initialize( HKG_WINDOW_CREATE_FLAG flags, HKG_WINDOW_BUFFERS buffers, int w, int h, const char* name, void* windowHandle = HK_NULL, void* parentHandle = HK_NULL ); // returns success
		virtual bool initializeContext();
		virtual void* getProcAddress(const char* procName);

		virtual	bool setAsCurrent(); // only need to call this if you have > 1 window
		virtual	bool setNullAsCurrent(); // only need to call this if you have > 1 window
		virtual bool clearBuffers();
		virtual bool swapBuffers();

		virtual HKG_WINDOW_MSG_STATUS peekMessages(bool inViewportResize = false); // will loop on windows messages if any. Will return if idle.
		virtual void waitMessages(); // block until there is messages

		inline HINSTANCE getInstanceHandle() const;

		virtual bool hasGamePads() const { return m_directJoysticks[0]!=HK_NULL; }

		inline bool getActiveState() const;
		inline void setActiveState(bool a);
	
			// override hkgWindow functionality
		virtual void	setMousePosition(int	x,int	y);
			// override hkgWindow functionality
		virtual void	showMouseCursor(bool	visible);

		virtual int getDisplayAdapter() const { return m_adapter; }


		// Internal

		bool updateJoystick();

	protected:

		hkgWindowPC( HINSTANCE instance );
		virtual ~hkgWindowPC();

		HINSTANCE m_hinstance;
		HGLRC	m_hglrc;	// GL Context
		HDC		m_dc;		// Device Context
		bool    m_active;   // in a minimized state
		
		int		m_adapter;
		bool    m_ownHWND; // did it create the hwnd?

		hkgDirectJoystickPC* m_directJoysticks[2];
};

// WndProc prototype
LRESULT CALLBACK hkgWindowProcOGLPC(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#include <Graphics/Ogl/Pc/Window/hkgWindowPC.inl>

#endif //HK_GRAPHICS_WINDOW_PC_H



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
