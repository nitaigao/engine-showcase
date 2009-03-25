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

#include "impl/ViewportOverlay.h"

using namespace Ogre;
using namespace Hikari;
using namespace Hikari::Impl;

ViewportOverlay::ViewportOverlay(const Ogre::String& name, Ogre::Viewport* viewport, int width, int height, 
								 const Position& pos, const Ogre::String& matName, Ogre::uchar zOrder, Ogre::uchar tier)
: viewport(viewport), width(width), height(height), position(pos), isVisible(true), zOrder(zOrder), tier(tier)
{
	if(zOrder > 99)
		OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED, 
			"Z-order is out of bounds, must be within [0, 99].", "ViewportOverlay::ViewportOverlay");
	if(tier > 5)
		OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED, 
			"Tier number is out of bounds, must be within [0, 5].", "ViewportOverlay::ViewportOverlay");

	OverlayManager& overlayManager = OverlayManager::getSingleton();

	panel = static_cast<PanelOverlayElement*>(overlayManager.createOverlayElement("Panel", name + "Panel"));
	panel->setMetricsMode(Ogre::GMM_PIXELS);
	panel->setMaterialName(matName);
	panel->setDimensions(width, height);
	
	overlay = overlayManager.create(name + "Overlay");
	overlay->add2D(panel);
	setZOrder(zOrder);
	resetPosition();

	viewport->getTarget()->addListener(this);
}

ViewportOverlay::~ViewportOverlay()
{
	viewport->getTarget()->removeListener(this);

	if(overlay)
	{
		overlay->remove2D(panel);
		OverlayManager::getSingletonPtr()->destroyOverlayElement(panel);
		OverlayManager::getSingletonPtr()->destroy(overlay);
	}
}

void ViewportOverlay::move(int deltaX, int deltaY)
{
	panel->setPosition(panel->getLeft()+deltaX, panel->getTop()+deltaY);
}

void ViewportOverlay::setPosition(const Position& position)
{
	this->position = position;
	resetPosition();
}

void ViewportOverlay::resetPosition()
{
	int viewWidth = viewport->getActualWidth();
	int viewHeight = viewport->getActualHeight();

	if(position.usingRelative)
	{
		int left = 0 + position.data.rel.x;
		int center = (viewWidth/2)-(width/2) + position.data.rel.x;
		int right = viewWidth - width + position.data.rel.x;

		int top = 0 + position.data.rel.y;
		int middle = (viewHeight/2)-(height/2) + position.data.rel.y;
		int bottom = viewHeight-height + position.data.rel.y;

		switch(position.data.rel.position)
		{
		case Left:
			panel->setPosition(left, middle);
			break;
		case TopLeft:
			panel->setPosition(left, top);
			break;
		case TopCenter:
			panel->setPosition(center, top);
			break;
		case TopRight:
			panel->setPosition(right, top);
			break;
		case Right:
			panel->setPosition(right, middle);
			break;
		case BottomRight:
			panel->setPosition(right, bottom);
			break;
		case BottomCenter:
			panel->setPosition(center, bottom);
			break;
		case BottomLeft:
			panel->setPosition(left, bottom);
			break;
		case Center:
			panel->setPosition(center, middle);
			break;
		default:
			panel->setPosition(position.data.rel.x, position.data.rel.y);
			break;
		}
	}
	else
		panel->setPosition(position.data.abs.left, position.data.abs.top);
}

void ViewportOverlay::resize(int width, int height)
{
	this->width = width;
	this->height = height;
	panel->setDimensions(width, height);
}

void ViewportOverlay::hide()
{
	isVisible = false;
}

void ViewportOverlay::show()
{
	isVisible = true;
}

void ViewportOverlay::setTier(Ogre::uchar tier)
{
	this->tier = tier;
	overlay->setZOrder(100 * tier + zOrder);
}

void ViewportOverlay::setZOrder(Ogre::uchar zOrder)
{
	this->zOrder = zOrder;
	overlay->setZOrder(100 * tier + zOrder);
}

Ogre::uchar ViewportOverlay::getTier()
{
	return tier;
}

Ogre::uchar ViewportOverlay::getZOrder()
{
	return zOrder;
}

int ViewportOverlay::getX()
{
	return viewport->getActualLeft() + panel->getLeft();
}

int ViewportOverlay::getY()
{
	return viewport->getActualTop() + panel->getTop();
}

int ViewportOverlay::getRelativeX(int absX)
{
	return absX - viewport->getActualLeft() - panel->getLeft();
}

int ViewportOverlay::getRelativeY(int absY)
{
	return absY - viewport->getActualTop() - panel->getTop();
}

bool ViewportOverlay::isWithinBounds(int absX, int absY)
{
	int localX = getRelativeX(absX);
	int localY = getRelativeY(absY);

	if(localX > 0 && localX < width)
		if(localY > 0 && localY < height)
			return true;

	return false;
}

bool ViewportOverlay::operator>(const ViewportOverlay& rhs) const
{
	return tier * 100 + zOrder > rhs.tier * 100 + rhs.zOrder;
}

bool ViewportOverlay::operator<(const ViewportOverlay& rhs) const
{
	return tier * 100 + zOrder < rhs.tier * 100 + rhs.zOrder;
}

void ViewportOverlay::preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
{
}

void ViewportOverlay::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
{
}

void ViewportOverlay::preViewportUpdate(const Ogre::RenderTargetViewportEvent& evt)
{
	if(evt.source == viewport && isVisible)
		overlay->show();
}

void ViewportOverlay::postViewportUpdate(const Ogre::RenderTargetViewportEvent& evt)
{
	overlay->hide();
}

void ViewportOverlay::viewportAdded(const Ogre::RenderTargetViewportEvent& evt)
{
}

void ViewportOverlay::viewportRemoved(const Ogre::RenderTargetViewportEvent& evt)
{
}