/*
	This file is part of Hikari, a library that allows developers
	to use Flash in their Ogre3D applications.

	Copyright (C) 2008 Adam J. Simmons

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __Hikari_H__
#define __Hikari_H__

#include <string>
#include <vector>
#include <windows.h>
#include "FlashControl.h"

namespace Hikari {

namespace Impl { class KeyboardHook; }

enum MouseButtonID
{
	LeftMouseButton = 0,
	RightMouseButton, 
	MiddleMouseButton
};

/**
* The HikariManager is used to create/destroy FlashControls and handle global
* events such as mouse-input propagation and the management of keyboard focus.
*/
class _HikariExport HikariManager
{
public:
	/**
	* Instantiates the HikariManager singleton. You can access the singleton later
	* using the HikariManager::Get and GetPointer.
	*
	* @param	assetsDirectory	Specifies the directory (relative to the path of the
	*							executable) that holds your .swf files. This is used
	*							by FlashControl::load.
	*/
	HikariManager(const std::string& assetsDirectory = "");

	/**
	* Explicitly destroys the HikariManager singleton and destroys any active FlashControls.
	*/
	~HikariManager();

	/**
	* Retrieves the HikariManager singleton (must be instantiated first!)
	*/
	static HikariManager& Get();

	/**
	* Retrieves a pointer to the HikariManager singleton.
	*/
	static HikariManager* GetPointer();

	/**
	* Creates a FlashControl as a movable overlay.
	*
	* @param	name	The name of the FlashControl.
	* @param	viewport	The viewport that this overlay will inhabit.
	* @param	width	The width (in pixels).
	* @param	height	The height (in pixels).
	* @param	position	The position of the overlay, relative to the viewport.
	* @param	zOrder	The z-order of the overlay. [0, 99] Specify '0' to automatically
	*					use the next-highest z-order within the specified tier.
	* @param	tier	The tier of this overlay. [0, 5] You can group overlays together into
	*					different tiers to keep background overlays separated from 
	*					foreground overlays during focusing.
	*
	* @return	A pointer to the created FlashControl.
	*/
	FlashControl* createFlashOverlay(const Ogre::String& name, Ogre::Viewport* viewport, int width, int height, const Position& position, Ogre::uchar zOrder = 0, Ogre::uchar tier = 0);

	/**
	* Creates a FlashControl as a pure Ogre Material.
	*
	* @param	name	The name of the FlashControl.
	* @param	width	The width (in pixels).
	* @param	height	The height (in pixels).
	*
	* @return	A pointer to the created FlashControl.
	*
	* @note	Use FlashControl::getMaterialName after creation to retrieve the material using
	*		the Ogre::MaterialManager.
	*/
	FlashControl* createFlashMaterial(const Ogre::String& name, int width, int height);

	/**
	* Flags the specified FlashControl for destruction (to occur at the next update)
	*
	* @param	controlToDestroy	The control to flag for destruction.
	*/
	void destroyFlashControl(FlashControl* controlToDestroy);

	/**
	* Flags the specified FlashControl for destruction (to occur at the next update)
	*
	* @param	controlName	The name of the control to flag for destruction.
	*/
	void destroyFlashControl(const Ogre::String& controlName);

	/**
	* Flags all FlashControls for destruction (to occur at the next update)
	*/
	void destroyAllControls();

	/**
	* Retrieves a previously-created FlashControl by name.
	*
	* @param	controlName	The name of the FlashControl to retrieve.
	*
	* @return	If it is found, returns a pointer to the FlashControl, else returns 0.
	*/
	FlashControl* getFlashControl(const Ogre::String& controlName) const;

	/**
	* Gives each active FlashControl a chance to update and destroys any
	* control that is flagged for destruction.
	*/
	void update();

	/**
	* Retrieves the currently-focused FlashControl.
	*
	* @return	If any is focused, returns a pointer to the focused FlashControl, else returns 0;
	*/
	FlashControl* getFocusedControl() const;

	/**
	* Returns whether or not any FlashControl is focused (and is capturing keyboard input).
	*/
	bool isAnyFocused();

	/**
	* De-focuses any currently-focused FlashControl (removes keyboard focus).
	*/
	void defocusAll();

	/**
	* Injects a global mouse-move to be handled by all FlashControl overlays.
	*
	* @param	x	The absolute x-coordinate in screen-space.
	* @param	y	The absolute y-coordinate in screen-space.
	*
	* @return	Returns true if the coordinates are over a FlashControl overlay, else returns false.
	*/
	bool injectMouseMove(short x, short y);

	/**
	* Injects a global mouse-down to be handled by all FlashControl overlays.
	*
	* @param	buttonID	The MouseButtonID of the pressed button.
	*
	* @return	Returns true if the mouse event was handled by a FlashControl overlay, else returns false.
	*/
	bool injectMouseDown(int buttonID);

	/**
	* Injects a global mouse-up to be handled by all FlashControl overlays.
	*
	* @param	buttonID	The MouseButtonID of the released button.
	*
	* @return	Returns true if the mouse event was handled by a FlashControl overlay, else returns false.
	*/
	bool injectMouseUp(int buttonID);

	/**
	* Injects a global mouse-wheel event to be handled by all FlashControl overlays.
	*
	* @param	relScroll	The relative scroll amount of the mouse (in OIS, this is arg.state.Z.rel).
	*
	* @return	Returns true if the mouse event was handled by a FlashControl overlay, else returns false.
	*/
	bool injectMouseWheel(int relScroll);

	/**
	* Injects a keyboard-event to be handled by the focused FlashControl. You generally
	* don't need to call this unless you've disabled the keyboard-hook and wish to
	* inject keyboard/IME messages directly from your WindowProc.
	*
	* @param	msg		Specifies the message to be sent.
	* @param	wParam	Specifies additional message-specific information.
	* @param	lParam	Specifies additional message-specific information.
	*
	* @note You should only forward the following window messages using this function:
	*	<ul>
	*	<li>WM_KEYDOWN
	*	<li>WM_KEYUP
	*	<li>WM_CHAR
	*	<li>WM_DEADCHAR
	*	<li>WM_SYSKEYDOWN
	*	<li>WM_SYSKEYUP
	*	<li>WM_SYSDEADCHAR
	*	<li>WM_SYSCHAR
	*	<li>WM_IME_CHAR
	*	<li>WM_IME_COMPOSITION
	*	<li>WM_IME_COMPOSITIONFULL
	*	<li>WM_IME_CONTROL
	*	<li>WM_IME_ENDCOMPOSITION
	*	<li>WM_IME_KEYDOWN
	*	<li>WM_IME_KEYUP
	*	<li>WM_IME_NOTIFY
	*	<li>WM_IME_REQUEST
	*	<li>WM_IME_SELECT
	*	<li>WM_IME_SETCONTEXT
	*	<li>WM_IME_STARTCOMPOSITION
	*	<li>WM_HELP
	*	<li>WM_CANCELMODE
	*	</ul>
	*/
	void injectKeyEvent(UINT msg, WPARAM wParam, LPARAM lParam);

	/**
	* Sets whether or not the keyboard-hook is enabled and should
	* automatically catch and forward all keyboard messages to the
	* focused FlashControl. (Enabled by default)
	*
	* @param	isEnabled	Whether or not the keyboard-hook is enabled.
	*
	* @note	For some reason, IME doesn't work properly using the
	*	the keyboard hook to catch keyboard messages and so, as
	*	a workaround, you can disable the keyboard-hook and forward
	*	keyboard/IME messages directly to Hikari from your main
	*	window's WindowProc.
	*/
	void setKeyboardHookEnabled(bool isEnabled);

protected:

	bool focusControl(int x, int y, FlashControl* selection = 0);
	FlashControl* getTopControl(int x, int y);

	static HikariManager* instance;
	typedef std::map<Ogre::String, FlashControl*> ControlMap;
	ControlMap controls;
	HMODULE flashLib;
	std::string basePath;
	Impl::KeyboardHook* keyboardHook;
	FlashControl* focusedControl;
	int mouseXPos, mouseYPos;
	bool mouseButtonRDown;
	unsigned short zOrderCounter;

	friend class FlashControl;
	friend class Impl::KeyboardHook;
};

}
#endif