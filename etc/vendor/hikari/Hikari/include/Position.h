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

#ifndef __Position_H__
#define __Position_H__

#include "HikariPlatform.h"

namespace Hikari {

/**
* Defines the relative positions for the Position object.
*/
enum RelativePosition
{
	Left,
	TopLeft,
	TopCenter,
	TopRight,
	Right,
	BottomRight,
	BottomCenter,
	BottomLeft,
	Center
};

/**
* Used by HikariManager::createFlashOverlay to define the position of the overlay.
*/
struct _HikariExport Position
{
	bool usingRelative;
	union {
		struct { RelativePosition position; short x; short y; } rel;
		struct { short left; short top; } abs;
	} data;

	/**
	* Creates a default Position object (absolute, top-left corner).
	*/
	Position();

	/**
	* Creates a relatively-positioned Position object.
	*
	* @param	relPosition		The position of the Object in relation to the Viewport
	*
	* @param	offsetLeft	How many pixels from the left to offset the Object from the relative position.
	*
	* @param	offsetTop	How many pixels from the top to offset the Object from the relative position.
	*/
	Position(const RelativePosition &relPosition, short offsetLeft = 0, short offsetTop = 0);

	/**
	* Creates an absolutely-positioned Position object.
	*
	* @param	absoluteLeft	The number of pixels from the left of the Render Window.
	*
	* @param	absoluteTop		The number of pixels from the top of the Render Window.
	*/
	Position(short absoluteLeft, short absoluteTop);
};

}

#endif