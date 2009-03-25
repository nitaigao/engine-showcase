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

#include "FlashControl.h"
#include "impl/FlashSite.h"
#include "impl/FlashHandler.h"
#include "impl/RenderBuffer.h"
#include "impl/ViewportOverlay.h"
#include "Hikari.h"
#include "OgreBitwise.h"

using namespace Hikari;
using namespace Ogre;

FlashControl::FlashControl(const Ogre::String& name, Ogre::Viewport* viewport, int width, int height, const Position& position, Ogre::uchar zOrder, Ogre::uchar tier)
:	name(name),
	site(0),
	handler(0),
	flashInterface(0),
	oleObject(0), windowlessObject(0),
	width(width), height(height),
	state(0),
	comCount(0),
	mainContext(0),	mainBitmap(0), mainBuffer(0),
	altContext(0), altBitmap(0), altBuffer(0),
	renderBuffer(0),
	usingAlphaHack(false),
	isClean(true), isTotallyDirty(false),
	overlay(0),
	lastDirtyWidth(0), lastDirtyHeight(0),
	texWidth(width), texHeight(height), texDepth(0), texPitch(0), texUnit(0),
	compensateNPOT(false), isTransparent(false), okayToDelete(false), isDraggable(true),
	isIgnoringTransparent(true), transThreshold(0.04)
{
	renderBuffer = new Impl::RenderBuffer(width, height);
	createControl();
	createMaterial();
	overlay = new Impl::ViewportOverlay(name, viewport, width, height, position, materialName, zOrder, tier);
	if(compensateNPOT)
		overlay->panel->setUV(0, 0, (Real)width/(Real)texWidth, (Real)height/(Real)texHeight);
}

FlashControl::FlashControl(const Ogre::String& name, int width, int height)
:	name(name),
	site(0),
	handler(0),
	flashInterface(0),
	oleObject(0), windowlessObject(0),
	width(width), height(height),
	state(0),
	comCount(0),
	mainContext(0),	mainBitmap(0), mainBuffer(0),
	altContext(0), altBitmap(0), altBuffer(0),
	renderBuffer(0),
	usingAlphaHack(false),
	isClean(true), isTotallyDirty(false),
	overlay(0),
	lastDirtyWidth(0), lastDirtyHeight(0),
	texWidth(width), texHeight(height), texDepth(0), texPitch(0), texUnit(0),
	compensateNPOT(false), isTransparent(false), okayToDelete(false), isDraggable(false),
	isIgnoringTransparent(false), transThreshold(0)
{
	renderBuffer = new Impl::RenderBuffer(width, height);
	createControl();
	createMaterial();
}

FlashControl::~FlashControl()
{
	if(overlay) delete overlay;
	texture.setNull();
	MaterialManager::getSingletonPtr()->remove(name + "Material");
	TextureManager::getSingletonPtr()->remove(name + "Texture");

	if(windowlessObject) windowlessObject->Release();
	if(flashInterface) flashInterface->Release();
	if(handler)
	{
		handler->Shutdown();
		handler->Release();
	}
	
	if(oleObject)
	{
		oleObject->Close(OLECLOSE_NOSAVE);
		oleObject->Release();
	}

	if(site) site->Release();
	
	if(comCount)
		LogManager::getSingleton().logMessage("WARNING: Hikari::FlashControl is leaking COM objects!");
	
	if(mainContext) ::DeleteDC(mainContext);
	if(mainBitmap) ::DeleteObject(mainBitmap);
	if(altContext) ::DeleteDC(altContext);
	if(altBitmap) ::DeleteObject(altBitmap);
	if(renderBuffer) delete renderBuffer;
}

typedef HRESULT (__stdcall *GetClassObject)(REFCLSID rclsid, REFIID riid, LPVOID * ppv); 

void FlashControl::createControl()
{
	site = new Impl::FlashSite();
	site->AddRef();	
	site->Init(this);
	
	HMODULE flashLib = HikariManager::Get().flashLib;
	
	// Try to load from user-supplied Flash OCX first
	if(flashLib)
	{
		IClassFactory* factory = 0;
		GetClassObject getClassFunc = (GetClassObject)GetProcAddress(flashLib, "DllGetClassObject");
		HRESULT result = getClassFunc(ShockwaveFlashObjects::CLSID_ShockwaveFlash, IID_IClassFactory, (void**)&factory);
		if(SUCCEEDED(result))
		{
			factory->CreateInstance(NULL, IID_IOleObject, (void**)&oleObject);
			factory->Release();	
		}
	}

	// If we still don't have the object, try loading from registry
	if(!oleObject)
	{
		HRESULT result = CoCreateInstance(ShockwaveFlashObjects::CLSID_ShockwaveFlash, 0, CLSCTX_INPROC_SERVER, IID_IOleObject, (void**)&oleObject);
		if(FAILED(result))
			OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED, "Unable to load the Flash ActiveX control.", "FlashControl::createControl");
	}

	IOleClientSite* clientSite = 0;
	site->QueryInterface(__uuidof(IOleClientSite), (void**)&clientSite);
	oleObject->SetClientSite(clientSite);
	clientSite->Release();

	IOleInPlaceObject* inPlaceObject = 0;	
	oleObject->QueryInterface(__uuidof(IOleInPlaceObject), (LPVOID*)&inPlaceObject);			

	if(inPlaceObject)
	{
		invalidateTotally();

		inPlaceObject->SetObjectRects(&dirtyBounds, &dirtyBounds);
		inPlaceObject->Release();
	}

	oleObject->QueryInterface(__uuidof(ShockwaveFlashObjects::IShockwaveFlash), (LPVOID*)&flashInterface);

	flashInterface->PutWMode("opaque");

	oleObject->DoVerb(OLEIVERB_INPLACEACTIVATE, 0, clientSite, 0, 0, 0);
	clientSite->Release();	
		
	oleObject->QueryInterface(__uuidof(IOleInPlaceObjectWindowless), (LPVOID*)&windowlessObject);
			
	handler = new Impl::FlashHandler();
	handler->AddRef();	
	handler->Init(this);

	IViewObject* curView = 0;
	flashInterface->QueryInterface(IID_IViewObject, (void**)&curView);
}

void FlashControl::createMaterial()
{
	texture.setNull();
	MaterialManager::getSingletonPtr()->remove(name + "Material");
	TextureManager::getSingletonPtr()->remove(name + "Texture");

	texWidth = width;
	texHeight = height;
	if(!Bitwise::isPO2(width) || !Bitwise::isPO2(height))
	{
		if(Root::getSingleton().getRenderSystem()->getCapabilities()->hasCapability(RSC_NON_POWER_OF_2_TEXTURES))
		{
			if(Root::getSingleton().getRenderSystem()->getCapabilities()->getNonPOW2TexturesLimited())
				compensateNPOT = true;
		}
		else compensateNPOT = true;
		
		if(compensateNPOT)
		{
			texWidth = Bitwise::firstPO2From(width);
			texHeight = Bitwise::firstPO2From(height);
		}
	}

	// Create the texture
	texture = TextureManager::getSingleton().createManual(
		name + "Texture", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		TEX_TYPE_2D, (uint)texWidth, (uint)texHeight, 0, isTransparent? PF_BYTE_BGRA : PF_BYTE_BGR,
		TU_DYNAMIC_WRITE_ONLY_DISCARDABLE, this);

	HardwarePixelBufferSharedPtr pixelBuffer = texture->getBuffer();
	pixelBuffer->lock(HardwareBuffer::HBL_DISCARD);
	const PixelBox& pixelBox = pixelBuffer->getCurrentLock();
	texDepth = Ogre::PixelUtil::getNumElemBytes(pixelBox.format);
	texPitch = (pixelBox.rowPitch*texDepth);

	uint8* pDest = static_cast<uint8*>(pixelBox.data);

	memset(pDest, 128, texHeight*texPitch);

	pixelBuffer->unlock();

	materialName = name + "Material";

	MaterialPtr material = MaterialManager::getSingleton().create(materialName, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	Pass* matPass = material->getTechnique(0)->getPass(0);
	matPass->setSceneBlending(SBT_TRANSPARENT_ALPHA);
	matPass->setDepthWriteEnabled(false);

	texUnit = matPass->createTextureUnitState(name + "Texture");
	texUnit->setTextureFiltering(FO_NONE, FO_NONE, FO_NONE);

	invalidateTotally();

	if(overlay)
	{
		overlay->panel->setMaterialName(materialName);
		if(compensateNPOT)
			overlay->panel->setUV(0, 0, (Real)width/(Real)texWidth, (Real)height/(Real)texHeight);
	}
}

void FlashControl::load(const Ogre::String& movieFilename)
{
	std::string moviePath = HikariManager::Get().basePath + movieFilename;

	FILE* file = fopen(moviePath.c_str(), "r");
	if(file)
	{
		fclose(file);
		flashInterface->PutMovie((moviePath).c_str());
	}
	else
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_FILE_NOT_FOUND, "Could not load '" + moviePath + "', the file was not found.", "FlashControl::load");
	}
}

void FlashControl::play()
{
	flashInterface->Play();
}

void FlashControl::stop()
{
	flashInterface->Stop();
}

void FlashControl::rewind()
{
	flashInterface->Rewind();
}

void FlashControl::gotoFrame(long frameNum)
{
	flashInterface->raw_GotoFrame(frameNum);
}

void FlashControl::setLoop(bool shouldLoop)
{
	flashInterface->PutLoop(shouldLoop);
}

void FlashControl::setTransparent(bool isTransparent, bool useAlphaHack)
{
	this->isTransparent = isTransparent;
	if(isTransparent)
	{
		flashInterface->PutWMode("transparent");
		usingAlphaHack = useAlphaHack;
	}
	else
	{
		flashInterface->PutWMode("opaque");
		usingAlphaHack = false;
	}
	
	createMaterial();
}

void FlashControl::setQuality(short renderQuality)
{
	switch(renderQuality)
	{
	case RQ_LOW:
		flashInterface->PutQuality2("low");
		break;
	case RQ_MEDIUM:
		flashInterface->PutQuality2("medium");
		break;
	case RQ_HIGH:
		flashInterface->PutQuality2("high");
		break;
	case RQ_BEST:
		flashInterface->PutQuality2("best");
		break;
	case RQ_AUTOLOW:
		flashInterface->PutQuality2("autolow");
		break;
	case RQ_AUTOHIGH:
		flashInterface->PutQuality2("autohigh");
		break;
	}
}

void FlashControl::setScaleMode(short scaleMode)
{
	flashInterface->PutScaleMode(scaleMode);
}

void FlashControl::setDraggable(bool isDraggable)
{
	this->isDraggable = isDraggable;
}

void FlashControl::setIgnoreTransparentPixels(bool shouldIgnore, Ogre::Real threshold)
{
	isIgnoringTransparent = shouldIgnore;
	transThreshold = threshold;
}

const Ogre::String & FlashControl::getName() const
{
	return name;
}

const Ogre::String & FlashControl::getMaterialName() const
{
	return materialName;
}

void FlashControl::bind(const Ogre::DisplayString& funcName, const FlashDelegate& callback)
{
	if(funcName.empty() || callback.empty())
		return;

	delegateMap[funcName] = callback;
}

void FlashControl::unbind(const Ogre::DisplayString& funcName)
{
	DelegateIter i = delegateMap.find(funcName);
	if(i != delegateMap.end())
		delegateMap.erase(i);
}

FlashValue FlashControl::callFunction(Ogre::DisplayString funcName, const Arguments& args)
{
	BSTR returnVal = 0;
	HRESULT result = flashInterface->raw_CallFunction(_bstr_t(Impl::serializeInvocation(funcName, args).c_str()), &returnVal);

#ifdef _DEBUG
	if(FAILED(result))
		OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED, 
			"Failed to call ActionScript function '" + funcName + "' from FlashControl '" + name + "'", 
			"FlashControl::callFunction");
#endif

	return Impl::deserializeValue((wchar_t*)returnVal);
}

void FlashControl::hide()
{
	if(overlay)
		overlay->hide();
}
void FlashControl::show()
{
	if(overlay)
		overlay->show();
}
bool FlashControl::getVisibility() const
{
	if(overlay)
		return overlay->isVisible;
	else
		return false;
}
void FlashControl::setOpacity(Ogre::Real opacity)
{
	if(texUnit)
		texUnit->setAlphaOperation(LBX_MODULATE, LBS_TEXTURE, LBS_MANUAL, 1, opacity);
}

void FlashControl::focus()
{
	HikariManager::Get().focusControl(0, 0, this);
}

void FlashControl::move(int deltaX, int deltaY)
{
	if(overlay)
		overlay->move(deltaX, deltaY);
}

void FlashControl::setPosition(const Position& position)
{
	if(overlay)
		overlay->setPosition(position);
}

void FlashControl::resetPosition()
{
	if(overlay)
		overlay->resetPosition();
}

void FlashControl::getCoordinates(int& x, int& y)
{
	if(overlay)
	{
		x = overlay->getX();
		y = overlay->getY();
	}
}

void FlashControl::getExtents(unsigned short &width, unsigned short &height) const
{
	width = this->width;
	height = this->height;
}

void FlashControl::getUVScale(Ogre::Real &uScale, Ogre::Real &vScale) const
{
	uScale = vScale = 1;

	if(compensateNPOT)
	{
		uScale = (Real)width/(Real)texWidth;
		vScale = (Real)height/(Real)texHeight;
	}
}

void FlashControl::injectMouseMove(int xPos, int yPos)
{
	LRESULT result;
	windowlessObject->OnWindowMessage(WM_MOUSEMOVE, 0, MAKELPARAM(xPos, yPos), &result);
}

void FlashControl::injectMouseDown(int xPos, int yPos)
{
	LRESULT result;
	windowlessObject->OnWindowMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(xPos, yPos), &result);
}

void FlashControl::injectMouseUp(int xPos, int yPos)
{
	LRESULT result;
	windowlessObject->OnWindowMessage(WM_LBUTTONUP, 0, MAKELPARAM(xPos, yPos), &result);
}

#ifndef WM_MOUSEWHEEL
#	define WM_MOUSEWHEEL 0x020A
#endif

void FlashControl::injectMouseWheel(int relScroll, int xPos, int yPos)
{
	LRESULT result;
	windowlessObject->OnWindowMessage(WM_MOUSEWHEEL, MAKEWPARAM(0, relScroll), MAKELPARAM(xPos, yPos), &result);
}

bool FlashControl::isPointOverMe(int screenX, int screenY)
{
	if(!overlay)
		return false;

	if(!overlay->isVisible)
		return false;

	if(overlay->isWithinBounds(screenX, screenY))
	{
		if(isTransparent && isIgnoringTransparent)
			return renderBuffer->buffer[overlay->getRelativeY(screenY)*renderBuffer->rowSpan+overlay->getRelativeX(screenX)*4+3] > transThreshold * 255;
		else
			return true;
	}

	return false;
}

void FlashControl::invalidateTotally()
{
	isClean = false;
	isTotallyDirty = true;
	dirtyBounds.left = 0;
	dirtyBounds.top = 0;
	dirtyBounds.right = width;
	dirtyBounds.bottom = height;
}

void FlashControl::handleKeyEvent(UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT aResult;
	windowlessObject->OnWindowMessage(msg, wParam, lParam, &aResult);
}

void FlashControl::handleFlashCall(const std::wstring& xmlString)
{
	std::wstring funcName;
	Arguments args;

	if(!Impl::deserializeInvocation(xmlString, funcName, args))
		return;

	DelegateIter i = delegateMap.find(funcName);
	if(i != delegateMap.end())
	{
		FlashValue retval = i->second(this, args);
		flashInterface->SetReturnValue(Impl::serializeValue(retval).c_str());
	}
}

void FlashControl::update()
{
	if(isClean)
		return;

	if(overlay)
		if(!overlay->isVisible)
			return;

	int dirtyWidth = dirtyBounds.right - dirtyBounds.left;
	int dirtyHeight = dirtyBounds.bottom - dirtyBounds.top;
	int dirtyBufSize = dirtyWidth * dirtyHeight * 4;

	IViewObject* curView = 0;
	flashInterface->QueryInterface(IID_IViewObject, (void**)&curView);

	if(!oleObject || !curView)
		return;

	if(!mainContext || dirtyWidth != lastDirtyWidth || dirtyHeight != lastDirtyHeight)
	{
		if(mainContext)
		{
			DeleteDC(mainContext);
			mainContext = 0;
		}
		if(mainBitmap)
		{
			DeleteObject(mainBitmap);
			mainBitmap = 0;
		}
		
		lastDirtyWidth = dirtyWidth;
		lastDirtyHeight = dirtyHeight;

		HDC hdc = GetDC(0);
		BITMAPINFOHEADER bih = {0};
		bih.biSize = sizeof(BITMAPINFOHEADER);
		bih.biBitCount = 32;
		bih.biCompression = BI_RGB;
		bih.biPlanes = 1;
		bih.biWidth = dirtyWidth;
		bih.biHeight = -dirtyHeight;
		mainContext = CreateCompatibleDC(hdc);
		mainBitmap = CreateDIBSection(hdc, (BITMAPINFO*)&bih, DIB_RGB_COLORS, (void**)&mainBuffer, 0, 0);
		SelectObject(mainContext, mainBitmap);

		if(usingAlphaHack)
		{
			if(altContext)
			{
				DeleteDC(altContext);
				altContext = 0;
			}
			if(altBitmap)
			{
				DeleteObject(altBitmap);
				altBitmap = 0;
			}

			altContext = CreateCompatibleDC(hdc);
			altBitmap = CreateDIBSection(hdc, (BITMAPINFO*)&bih, DIB_RGB_COLORS, (void **)&altBuffer, 0, 0);
			SelectObject(altContext, altBitmap);
		}

		ReleaseDC(0, hdc);
	}

	RECT local;
	local.left = -dirtyBounds.left;
	local.top = -dirtyBounds.top;
	local.right = local.left + dirtyBounds.right;
	local.bottom = local.top + dirtyBounds.bottom;

	if(!usingAlphaHack)
	{
		memset(mainBuffer, 0, dirtyBufSize);
		
		HRESULT hr = OleDraw(curView, DVASPECT_TRANSPARENT, mainContext, &local);
	}
	else 
	{
		memset(mainBuffer, 0, dirtyBufSize);
		memset(altBuffer, 255, dirtyBufSize);

		OleDraw(curView, DVASPECT_TRANSPARENT, mainContext, &local);
		OleDraw(curView, DVASPECT_TRANSPARENT, altContext, &local);

		// We've rendered the dirty area twice: once on black and once
		// on white. Now we compare the red channels of each to determine
		// the alpha value of each pixel.
		BYTE *blackBuffer, *whiteBuffer;
		blackBuffer = mainBuffer;
		whiteBuffer = altBuffer;
		BYTE blackRed, whiteRed;
		int size = dirtyWidth * dirtyHeight;
		for(int i = 0; i < size; i++)
		{
			blackRed = *blackBuffer;
			whiteRed = *whiteBuffer;
			blackBuffer += 3;
			whiteBuffer += 4;
			*blackBuffer++ = 255 - (whiteRed - blackRed);
		}
	}

	renderBuffer->copyArea(dirtyBounds, mainBuffer, dirtyWidth * 4);
	
	HardwarePixelBufferSharedPtr pixelBuffer = texture->getBuffer();
	pixelBuffer->lock(HardwareBuffer::HBL_DISCARD);
	const PixelBox& pixelBox = pixelBuffer->getCurrentLock();

	uint8* destBuffer = static_cast<uint8*>(pixelBox.data);

	renderBuffer->blitBGR(destBuffer, (int)texPitch, (int)texDepth);

	pixelBuffer->unlock();

	isClean = true;
	isTotallyDirty = false;
}

void FlashControl::loadResource(Ogre::Resource* resource)
{
	Texture *tex = static_cast<Texture*>(resource); 

	tex->setTextureType(TEX_TYPE_2D);
	tex->setWidth(texWidth);
	tex->setHeight(texHeight);
	tex->setNumMipmaps(0);
	tex->setFormat(isTransparent? PF_BYTE_BGRA : PF_BYTE_BGR);
	tex->setUsage(TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);
	tex->createInternalResources();

	invalidateTotally();
}