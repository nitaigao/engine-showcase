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

#include "impl/KeyboardHook.h"
#include "Hikari.h"

using namespace Hikari;
using namespace Hikari::Impl;

LRESULT CALLBACK GetMessageProc(int nCode, WPARAM wParam, LPARAM lParam);

KeyboardHook* KeyboardHook::instance = 0;

KeyboardHook::KeyboardHook()
{
	instance = this;

	HINSTANCE hInstance = GetModuleHandle(0);

	getMsgHook = SetWindowsHookEx(WH_GETMESSAGE, GetMessageProc, hInstance, GetCurrentThreadId());
}

KeyboardHook::~KeyboardHook()
{
	UnhookWindowsHookEx(getMsgHook);

	instance = 0;
}

void KeyboardHook::handleHook(UINT msg, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
	case WM_DEADCHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_SYSDEADCHAR:
	case WM_SYSCHAR:
	case WM_IME_CHAR:
	case WM_IME_COMPOSITION:
	case WM_IME_COMPOSITIONFULL:
	case WM_IME_CONTROL:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_KEYDOWN:
	case WM_IME_KEYUP:
	case WM_IME_NOTIFY:
	case WM_IME_REQUEST:
	case WM_IME_SELECT:
	case WM_IME_SETCONTEXT:
	case WM_IME_STARTCOMPOSITION:
	case WM_HELP:
	case WM_CANCELMODE: 
		{
			HikariManager* manager = Hikari::HikariManager::GetPointer();
			if(manager)
				manager->injectKeyEvent(msg, wParam, lParam);
			break;
		}
	}
}

LRESULT CALLBACK GetMessageProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode == HC_ACTION)
	{
		MSG *msg = (MSG*)lParam;
		if(wParam & PM_REMOVE)
			KeyboardHook::instance->handleHook(msg->message, msg->hwnd, msg->wParam, msg->lParam);
	}

	return CallNextHookEx(KeyboardHook::instance->getMsgHook, nCode, wParam, lParam);
}