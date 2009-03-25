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

#ifndef __KeyboardHook_H__
#define __KeyboardHook_H__

#include <windows.h>

namespace Hikari {
namespace Impl {

class KeyboardHook
{
public:
	HHOOK getMsgHook;
	static KeyboardHook* instance;

	KeyboardHook();
	~KeyboardHook();

	void handleHook(UINT msg, HWND hwnd, WPARAM wParam, LPARAM lParam);
};

}
}

#endif