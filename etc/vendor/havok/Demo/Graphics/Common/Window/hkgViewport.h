/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_VIEWPORT
#define HK_GRAPHICS_VIEWPORT

#include <Common/Base/Types/hkBaseTypes.h>
#include <Graphics/Common/Input/Mouse/hkgMouse.h>
#include <Graphics/Common/Input/Pad/hkgPad.h>
#include <Graphics/Common/Input/Mouse/hkgPickData.h>
#include <Graphics/Common/Input/Keyboard/hkgKeyboard.h>
#include <Graphics/Common/Input/Keyboard/hkgKeyboard.h>
#include <Graphics/Common/Window/hkgViewportDefines.h>

class hkgDisplayContext;
class hkgDisplayWorld;
class hkgCamera;
class hkgSkyBox;
class hkgRay;


/// The viewport is a rectangular region of the framebuffer
/// that has its own camera and desired render state. You can have 
/// multiple viewports on most of your supported platforms but
/// some only allow one). It can be resized dynamically at runtime, 
/// and is able to handle default camera movement requests based 
/// on pad and mouse input.

#include <Graphics/Common/DisplayContext/hkgDisplayContext.h>

class hkgViewport : public hkgReferencedObject
{
		friend class hkgWindow;

	public:

			/// Create a platform specific viewport for the given owner. It will not 
			/// inc the reference count of their owner, as the reference is really the other 
			/// way (window -> viewport).
		static hkgViewport* (HK_CALL *create)(hkgWindow* owner);
		
			/// Set the extents in absolute window coordinates. It will assume that you
			/// want to then want it to ignore window resizes. It is from LowerLeft to UpperRight (OpenGL style).
		inline void setExtentsAbsolute( int ll_x, int ll_y,
										int ur_x, int ur_y );

			/// Set the extents in relative (normalized) coordinates, from 0.0 to 1.0 of 
			/// window width and window height. It will then assume that you want it to 
			/// resize with the window so will set the absoulte coords on resize to match
			/// these fractions. It is from LowerLeft to UpperRight (OpenGL style).
		inline void setExtentsRelative( float ll_x, float ll_y,
										float ur_x, float ur_y );

			/// Get the width, in pixels, of the viewport.
		inline int getWidth() const;

			/// Get the height, in pixels, of the viewport.
		inline int getHeight() const;

			/// Update the extents, and set the viewport up so that the camera is correct 
			/// for the current extents etc. Does not alter the hardware viewport until it
			/// is set as current, and so at the moment no platform specific viewport 
			/// overrides this function yet (this base implementation is enough).
		virtual void updateExtents();

			/// Will set the context vieport to point to this vieport.
			/// and fix the render state so that it matches the viewports
			/// desired state. It will also set the viewport to be the 
			/// current hardware viewport
		virtual void setAsCurrent(hkgDisplayContext* context); 
		
			/// Set the current camera for this viewport. It will be reference counted by this viewport.
			/// Setting this to HK_NULL may have undesirable effects, so it is not advised.
		void setCamera( hkgCamera* cam );

			/// Get the pointer (non const) to the camera. You can alter this camera as you wish, according
			/// to the interface as given in hkgCamera.
		inline hkgCamera* getCamera() const;

			/// Set the current skybox for this viewport. It will be reference counted by this viewport.
		void setSkyBox( hkgSkyBox* skybox );

			/// Get the pointer (non const) to the skybox. 
		inline hkgSkyBox* getSkyBox() const;

			/// Get the virtual nagigation mode (none,trackball,fly)
		inline HKG_CAMERA_NAVIGATION_MODE getNavigationMode() const;

			/// Set the virtual nagigation mode (none,trackball,fly)
		inline void setNavigationMode( HKG_CAMERA_NAVIGATION_MODE m );

		inline void setFlyModeUp( const float* d );
		inline float* getFlyModeUpPtr( );
		inline const float* getFlyModeUpPtr( ) const;

		inline void setInvertLook( bool invert );
		inline bool getInvertLook() const;

			/// Does this viewport accept any mouse events at all? If it does not accept mouse events the
			/// virtual trackball and viewport resize etc. will not work.
		inline bool getAcceptsMouseEvents() const;

			/// Set the whether the viewport accept any mouse events at all? If it does not 
			/// accept mouse events the virtual trackball and viewport resize etc. will not work.
		inline void setAcceptsMouseEvents(bool a);

			/// Does this viewport accept any pad events at all? If it does not accept pad events the
			/// fly mode will not work.
		inline bool getAcceptsPadEvents() const;

			/// Set the whether the viewport accepts any pad events at all? If it does not 
			/// accept pad events the fly mode will not work.
		inline void setAcceptsPadEvents(bool a);

			/// Get the desired render state of the viewport. A viewport can choose what is the default
			/// state for rendering in the viewport. It will force this state when the viewport is made current (setAsCurrent).
			/// It is a bitfield of enabled options, see the hkgDisplayContext header for more information on render state.
		inline HKG_ENABLED_STATE getDesiredState() const;

			/// Set the desired render state of the viewport. A viewport can choose what is the default
			/// state for rendering in the viewport. It will force this state when the viewport is made current (setAsCurrent).
			/// It is a bitfield of enabled options, see the hkgDisplayContext header for more information on render state.
		inline void setDesiredState( HKG_ENABLED_STATE s);

			/// Get the desired cull face mode (CW or CCW) if culling is enabled in the state
		inline HKG_CULLFACE_MODE getDesiredCullFaceMode() const;

			/// Set the desired cull face mode (CW or CCW) if culling is enabled in the state
		inline void setDesiredCullFaceMode( HKG_CULLFACE_MODE m);

			/// Get the desired alpha blend mode (Add or Modulate).
		inline HKG_BLEND_MODE getDesiredBlendMode() const;

			/// Set the desired alpha blend mode (Add or Modulate) for when alpha blend is enabled.
		inline void setDesiredBlendMode( HKG_BLEND_MODE  m);

			/// Toggle the states as specified in the bitfield s. Any states enabled in 's' will
			/// have the corresponding state in this viewports desired state toggled.
		inline void toggleState(HKG_ENABLED_STATE s);

			/// Get the lower left coordinate, stored in 'x' and 'y'.
		inline void getLowerLeftCoord(int &x, int &y) const;

			/// Get the upper right coordinate, stored in 'x' and 'y'.
		inline void getUpperRightCoord(int &x, int &y) const;

			/// Tell the viewport to accept the mouse focus. This is usually called by the hkgWindow
			/// that owns this viewport when the mouse button is pressed inside the bounds of this
			/// viewport.
		void acceptMouseFocus();

			/// Sets the convention used when associating mouse movements to camera movements.
		inline void setMouseConvention (HKG_MOUSE_CONVENTION convention);

			/// Retrieves the current convention used to associate mouse movements to camera movements.
		inline HKG_MOUSE_CONVENTION getMouseConvention() const;

			/// Process / handle the new mouse x and y coordinates and corresponding delta
			/// movement since the last frame. The delta difference is given so that the viewport
			/// doesn't have to remember mouse position explicitly.
		void processMouseMove(int x, int y, int dx, int dy, bool isVirtualMouse);

			/// Process / handle the mouse wheel delta movement, where the mouse is located at 'x' and 'y'.
		void processMouseWheel(int delta, int x, int y, bool isVirtualMouse);

			/// Process / handle the given mouse button changing to the given state, at location 'x' and 'y'.
			/// State is true or false representing pressed and released respectfully.
		void processMouseButton(HKG_MOUSE_BUTTON button, bool state, int x, int y, bool isVirtualMouse);  
			
			/// Process / handle the given game pad stick movement. The pad number (usually 0 or 1)
			/// and stick (usually 0 or 1) along with the normalized x and y coordinate (-1 to 1) and
			/// delta difference with the last sample are given (so that the viewport does not need to
			/// store pad state explicitly). This function does not emulate a mouse (which the similar 
			/// method in the hkgWindow can if desired.).
		void processPadStickMove( int pad, int stick, float x, float y, float dx, float dy);

			/// Process / handle the given game pad button state change. State is true or false
			/// representing pressed or released respectfully.
		void processPadButton( int pad, HKG_PAD_BUTTON button, bool state);

			/// Try to pick the closest hkgDisplayObject (using a raycast to the geometries)
			/// at viewport coordinates (mouse x and mouse y minus lower left viewport coord for instance)
			/// If the function succeeds (returns true) then the hkgViewportPickData is filled in
			/// and should be checked for the full details.
		bool pick( int x, int y, const hkgDisplayWorld* world, hkgViewportPickData& data);

			/// Does the viewport contain the given window coordinate?
		inline bool containsWindowCoord( int wx, int wy) const;

			/// Is the viewport border under the window coordinate (wx,wy) given that the 
			/// viewport border should be treated as 'width' pixels wide.
			/// It returns a bitfield of the sides that are under the coord, if any.
			/// It will return 0 (HKG_VIEWPORT_SIDE_NONE) if not under the coord, otherwise a bitwise | of:
			///   HKG_VIEWPORT_SIDE_TOP
			///   HKG_VIEWPORT_SIDE_BOTTOM
			///   HKG_VIEWPORT_SIDE_LEFT
			///   HKG_VIEWPORT_SIDE_RIGHT
		inline HKG_VIEWPORT_SIDE isWindowCoordOnBorder( int wx, int wy, unsigned int width) const;

			/// Get the const pointer to the window that owns this viewport.
		inline const hkgWindow* getOwnerWindow() const;
		
			/// Get the pointer to the window that owns this viewport.
		inline hkgWindow* getOwnerWindow();

		inline void setMouseSpeed(int pan, int wheelZoom, int buttonZoom, bool absoluteSpeed);
		inline void getMouseSpeed(int& pan, int& wheelZoom, int& buttonZoom, bool& absoluteSpeed) const;
		
		inline void setFlySpeed(int strafe, int move);
		inline void getFlySpeed(int& strafe, int& move) const;

		inline void setPadSpeed(int pan, int zoom, bool absoluteSpeed);
		inline void getPadSpeed(int& pan, int& zoom, bool& absoluteSpeed) const;
	
			// Internal reallt. Updates the camera if in fly mode based on current input and the delta time (in secs)
		void updateCamera(float dt);

	protected:

		// Given the current state of the mouse buttons and keys, returns the current interpretation
		// according to the mouse convention stored in m_mouseConvention (default, max, maya);
		void interpretCurrentTrackballAction(bool& doRotate, bool& doPan, bool& doZoom);

		hkgViewport(hkgWindow* owner);
		virtual ~hkgViewport();

		int m_winLL[2]; // from, absolute
		int m_winUR[2]; // to, absolute
		
		float m_winLLRelative[2]; // from, relative to win size
		float m_winURRelative[2]; // to, relative to win size

		bool m_absolute; //do we use the absolute as the reference or the relative

		hkgCamera*  m_camera; // camera to use when rendering
		hkgWindow*  m_owner;
		hkgSkyBox*  m_skyBox; // drawn first

		HKG_CAMERA_NAVIGATION_MODE m_navMode; // trackball or fly
		HKG_MOUSE_CONVENTION m_mouseConvention; // max, maya, etc
		bool	m_wantMouseEvents;
		bool	m_wantPadEvents;
		bool	m_flyInvertLook; // for the other 50% of the poplulation..
		float   m_flyModeUp[3];
		
		HKG_MOUSE_BUTTON   m_viewMouseButton;
		HKG_PAD_BUTTON	   m_viewPadButton;

		HKG_ENABLED_STATE  m_desiredState; 
		HKG_CULLFACE_MODE  m_desiredCullFaceMode;
		HKG_BLEND_MODE     m_desiredBlendMode;

		int m_speedMousePan;
		int m_speedMouseWheelZoom;
		int m_speedMouseButtonZoom;
		int m_speedPadPan;
		int m_speedPadZoom;
		int m_speedFlyStrafe;
		int m_speedFlyMove;
		bool m_mouseAbsoluteSpeed;
		bool m_padAbsoluteSpeed;

	
};

#include <Graphics/Common/Window/hkgViewport.inl>

#endif // HK_GRAPHICS_WINDOW

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
