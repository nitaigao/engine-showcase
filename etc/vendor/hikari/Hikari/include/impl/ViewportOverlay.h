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

#ifndef __ViewportOverlay_H__
#define __ViewportOverlay_H__

#include "Ogre.h"
#include "OgrePanelOverlayElement.h"
#include "Position.h"

namespace Hikari {
namespace Impl {

class ViewportOverlay : public Ogre::RenderTargetListener
{
public:
	Ogre::Viewport* viewport;
	Ogre::Overlay* overlay;
	Ogre::PanelOverlayElement* panel;
	Position position;
	bool isVisible;
	int width, height;
	Ogre::uchar tier, zOrder;

	ViewportOverlay(const Ogre::String& name, Ogre::Viewport* viewport, int width, int height, const Position& pos, const Ogre::String& matName, Ogre::uchar zOrder, Ogre::uchar tier);
	~ViewportOverlay();

	void move(int deltaX, int deltaY);
	void setPosition(const Position& position);
	void resetPosition();
	
	void resize(int width, int height);

	void hide();
	void show();

	void setTier(Ogre::uchar tier);
	void setZOrder(Ogre::uchar zOrder);

	Ogre::uchar getTier();
	Ogre::uchar getZOrder();

	int getX();
	int getY();
	
	int getRelativeX(int absX);
	int getRelativeY(int absY);

	bool isWithinBounds(int absX, int absY);

	bool operator>(const ViewportOverlay& rhs) const;
	bool operator<(const ViewportOverlay& rhs) const;

	void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
	void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
	void preViewportUpdate(const Ogre::RenderTargetViewportEvent& evt);
	void postViewportUpdate(const Ogre::RenderTargetViewportEvent& evt);
	void viewportAdded(const Ogre::RenderTargetViewportEvent& evt);
	void viewportRemoved(const Ogre::RenderTargetViewportEvent& evt);
};

}
}

#endif