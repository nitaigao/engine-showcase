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

#include "Hikari.h"
#include "impl/ViewportOverlay.h"
#include "impl/KeyboardHook.h"
#include <objbase.h>
#include <direct.h>
#include <stdlib.h>

std::string getCurrentWorkingDirectory()
{
	std::string workingDirectory = "";
	char currentPath[_MAX_PATH];
	getcwd(currentPath, _MAX_PATH);
	workingDirectory = currentPath;

	return workingDirectory + "\\";
}

using namespace Hikari;

HikariManager* HikariManager::instance = 0;

HikariManager::HikariManager(const std::string& assetsDirectory) : flashLib(0), focusedControl(0), mouseXPos(0), mouseYPos(0), 
	mouseButtonRDown(false), zOrderCounter(1), keyboardHook(0)
{
	if(instance)
		OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED, 
			"Can't instantiate the HikariManager singleton, it already exists.", 
			"HikariManager::HikariManager");

	instance = this;

	std::string workingDirectory = getCurrentWorkingDirectory();

	this->basePath = workingDirectory + assetsDirectory;

	if(this->basePath.at(this->basePath.length()-1) != '\\')
		this->basePath.push_back('\\');

	CoInitialize(0);

	// Attempt to load 'Flash.ocx', it's alrite if this fails, we have a fallback strategy
	flashLib = LoadLibrary((workingDirectory + "Flash.ocx").c_str());

	keyboardHook = new Impl::KeyboardHook();
}

HikariManager::~HikariManager()
{
	for(ControlMap::iterator i = controls.begin(); i != controls.end();)
	{
		FlashControl* toDelete = i->second;
		i = controls.erase(i);
		delete toDelete;
	}

	if(keyboardHook) delete keyboardHook;

	if(flashLib) FreeLibrary(flashLib);

	CoUninitialize();

	instance = 0;
}

HikariManager& HikariManager::Get()
{
	if(!instance)
		OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED, 
			"Can't retrieve the HikariManager singleton, it isn't instantiated.", 
			"HikariManager::Get");

	return *instance;
}

HikariManager* HikariManager::GetPointer()
{
	return instance;
}

FlashControl* HikariManager::createFlashOverlay(const Ogre::String& name, Ogre::Viewport* viewport, int width, int height, const Position& position, Ogre::uchar zOrder, Ogre::uchar tier)
{
	if(controls.find(name) != controls.end())
		OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED, 
			"An attempt was made to create a FlashControl named '" + name + "' when a FlashControl by the same name already exists!", 
			"HikariManager::createFlashOverlay");

	if(zOrder == 0)
	{
		int highestZOrder = -1;

		std::map<Ogre::String, FlashControl*>::iterator iter;
		for(iter = controls.begin(); iter != controls.end(); iter++)
			if(iter->second->overlay)
				if(iter->second->overlay->getTier() == tier)
					if(iter->second->overlay->getZOrder() > highestZOrder)
						highestZOrder = iter->second->overlay->getZOrder();

		if(highestZOrder == -1)
			zOrder = 0;
		else
			zOrder = highestZOrder + 1;
	}

	return controls[name] = new FlashControl(name, viewport, width, height, position, zOrder, tier);
}

FlashControl* HikariManager::createFlashMaterial(const Ogre::String& name, int width, int height)
{
	if(controls.find(name) != controls.end())
		OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED, 
			"An attempt was made to create a FlashControl named '" + name + "' when a FlashControl by the same name already exists!", 
			"HikariManager::createFlashMaterial");

	return controls[name] = new FlashControl(name, width, height);
}

void HikariManager::destroyFlashControl(FlashControl* controlToDestroy)
{
	if(controlToDestroy)
		controlToDestroy->okayToDelete = true;
}

void HikariManager::destroyFlashControl(const Ogre::String& controlName)
{
	FlashControl* control = getFlashControl(controlName);
	if(control)
		control->okayToDelete = true;
}

void HikariManager::destroyAllControls()
{
	for(ControlMap::iterator iter = controls.begin(); iter != controls.end(); iter++)
		iter->second->okayToDelete = true;

	focusedControl = 0;
}

FlashControl* HikariManager::getFlashControl(const Ogre::String& controlName) const
{
	ControlMap::const_iterator i = controls.find(controlName);
	if(i != controls.end())
		return i->second;

	return 0;
}

void HikariManager::update()
{
	for(ControlMap::iterator iter = controls.begin(); iter != controls.end();)
	{
		if(iter->second->okayToDelete)
		{
			FlashControl* controlToDelete = iter->second;
			iter = controls.erase(iter);
			if(focusedControl == controlToDelete) focusedControl = 0;
			delete controlToDelete;
		}
		else
		{
			iter->second->update();
			iter++;
		}
	}
}

FlashControl* HikariManager::getFocusedControl() const
{
	return focusedControl;
}

bool HikariManager::isAnyFocused()
{
	return !!focusedControl;
}

void HikariManager::defocusAll()
{
	focusedControl = 0;
}

bool HikariManager::injectMouseMove(short x, short y)
{
	bool eventHandled = false;

	if(mouseButtonRDown && focusedControl)
	{
		if(focusedControl->isDraggable)
		{
			focusedControl->move(x-mouseXPos, y-mouseYPos);
			eventHandled = true;
		}
	}
	else
	{
		for(ControlMap::iterator iter = controls.begin(); iter != controls.end(); iter++)
		{
			if(iter->second->overlay)
				iter->second->injectMouseMove(iter->second->overlay->getRelativeX(x), iter->second->overlay->getRelativeY(y));

			if(!eventHandled)
				if(iter->second->isPointOverMe(x, y))
					eventHandled = true;
		}
	}

	mouseXPos = x;
	mouseYPos = y;

	return eventHandled;
}

bool HikariManager::injectMouseDown(int buttonID)
{
	if(buttonID == LeftMouseButton)
	{
		if(focusControl(mouseXPos, mouseYPos))
		{
			int relX = focusedControl->overlay->getRelativeX(mouseXPos);
			int relY = focusedControl->overlay->getRelativeY(mouseYPos);

			focusedControl->injectMouseDown(relX, relY);
		}
	}
	else if(buttonID == RightMouseButton)
	{
		mouseButtonRDown = true;
		focusControl(mouseXPos, mouseYPos);
	}

	if(focusedControl)
		return true;

	return false;
}

bool HikariManager::injectMouseUp(int buttonID)
{
	if(buttonID == LeftMouseButton && focusedControl)
	{
		if(focusedControl->overlay)
			focusedControl->injectMouseUp(focusedControl->overlay->getRelativeX(mouseXPos), focusedControl->overlay->getRelativeY(mouseYPos));
	}
	else if(buttonID == RightMouseButton)
		mouseButtonRDown = false;

	if(focusedControl)
		return true;

	return false;
}

bool HikariManager::injectMouseWheel(int relScroll)
{
	if(focusedControl)
	{
		if(focusedControl->isPointOverMe(mouseXPos, mouseYPos))
		{
			focusedControl->injectMouseWheel(relScroll, focusedControl->overlay->getRelativeX(mouseXPos), focusedControl->overlay->getRelativeY(mouseYPos));
			return true;
		}
	}

	return false;
}

void HikariManager::injectKeyEvent(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(focusedControl)
		focusedControl->handleKeyEvent(msg, wParam, lParam);
}

void HikariManager::setKeyboardHookEnabled(bool isEnabled)
{
	if(isEnabled)
	{
		if(!keyboardHook)
			keyboardHook = new Impl::KeyboardHook();
	}
	else
	{
		if(keyboardHook)
		{
			delete keyboardHook;
			keyboardHook = 0;
		}
	}
}

bool HikariManager::focusControl(int x, int y, FlashControl* selection)
{
	defocusAll();

	if(selection)
	{
		if(!selection->overlay)
		{
			focusedControl = selection;
			return true;
		}
	}

	FlashControl* controlToFocus = selection? selection : getTopControl(x, y);

	if(!controlToFocus)
		return false;

	std::vector<FlashControl*> sortedControls;

	std::map<Ogre::String, FlashControl*>::iterator iter;
	for(iter = controls.begin(); iter != controls.end(); iter++)
		if(iter->second->overlay)
			if(iter->second->overlay->getTier() == controlToFocus->overlay->getTier())
				sortedControls.push_back(iter->second);

	struct compare { bool operator()(FlashControl* a, FlashControl* b){ return(a->overlay->getZOrder() > b->overlay->getZOrder()); }};

	std::sort(sortedControls.begin(), sortedControls.end(), compare());

	if(sortedControls.size())
	{
		if(sortedControls.at(0) != controlToFocus)
		{
			unsigned int popIdx = 0;
			for(; popIdx < sortedControls.size(); popIdx++)
				if(sortedControls.at(popIdx) == controlToFocus)
					break;

			unsigned char highestZ = sortedControls.at(0)->overlay->getZOrder();
			for(unsigned int i = 0; i < popIdx; i++)
				sortedControls.at(i)->overlay->setZOrder(sortedControls.at(i+1)->overlay->getZOrder());
			
			sortedControls.at(popIdx)->overlay->setZOrder(highestZ);
		}
	}

	focusedControl = controlToFocus;

	return true;
}

FlashControl* HikariManager::getTopControl(int x, int y)
{
	FlashControl* top = 0;

	std::map<Ogre::String, FlashControl*>::iterator iter;
	for(iter = controls.begin(); iter != controls.end(); iter++)
	{
		if(!iter->second->isPointOverMe(x, y))
			continue;

		if(!top)
			top = iter->second;
		else
			top = *(top->overlay) > *(iter->second->overlay) ? top : iter->second;
	}

	return top;
}