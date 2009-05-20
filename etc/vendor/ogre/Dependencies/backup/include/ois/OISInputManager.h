/*
The zlib/libpng License

Copyright (c) 2006 Phillip Castaneda (pjcast -- www.wreckedgames.com)

This software is provided 'as-is', without any express or implied warranty. In no event will
the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial 
applications, and to alter it and redistribute it freely, subject to the following
restrictions:

    1. The origin of this software must not be misrepresented; you must not claim that 
		you wrote the original software. If you use this software in a product, 
		an acknowledgment in the product documentation would be appreciated but is 
		not required.

    2. Altered source versions must be plainly marked as such, and must not be 
		misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/
#ifndef OIS_InputManager_H
#define OIS_InputManager_H

#include "OISPrereqs.h"

namespace OIS
{
	/**
		Base Manager class. No longer a Singleton; so feel free to create as many InputManager's as you have
		windows.
	*/
	class _OISExport InputManager
	{
	public:
		/**
		@remarks
			Returns version number (useful in DLL/SO libs)
		@returns
			Bits: 1-8 Patch number, 9-16 Minor version, 17-32 Major version
		*/
		static unsigned int getVersionNumber();

		/**
		@remarks
			Returns version string (useful in DLL/SO libs)
		@returns
			Version name
		*/
		static const char* getVersionName();

		/**
		@remarks
			Creates appropriate input system dependent on platform. 
		@param winHandle
			Contains OS specific window handle (such as HWND or X11 Window)
		@returns
			A pointer to the created manager, or raises Exception
		*/
		static InputManager* createInputSystem( std::size_t winHandle );

		/**
		@remarks
			Creates appropriate input system dependent on platform. 
		@param paramList
			ParamList contains OS specific info (such as HWND and HINSTANCE for window apps),
			and access mode.
		@returns
			A pointer to the created manager, or raises Exception
		*/
		static InputManager* createInputSystem( ParamList &paramList );

		/**
		@remarks
			Gets the name of the current input system.. eg. "Win32", "Linux", "XBox", etc
		*/
		virtual const std::string& inputSystemName() = 0;

		/**
		@remarks
			Destroys the InputManager
		@param manager
			Manager to destroy
		*/
		static void destroyInputSystem(InputManager* manager);

		/**
		@remarks
			Returns the number of joysticks found
		*/
		virtual int numJoySticks() = 0;

		/**
		@remarks
			Returns the number of mice found
		*/
		virtual int numMice() = 0;

		/**
		@remarks
			Returns the number of keyboards found
		*/
		virtual int numKeyboards() = 0;

		/**
		@remarks
			Returns the type of input requested or raises Exception
		*/
		virtual Object* createInputObject( Type iType, bool bufferMode ) = 0;

		/**
		@remarks
			Destroys Input Object
		*/
		virtual void destroyInputObject( Object* obj ) = 0;

	protected:
		//! Internal... Called from createInputSystem
		virtual void _initialize(ParamList &paramList) = 0;

		//These are hidden to prevent app from directly instantiating/deleting an InputManager class
		InputManager() {}
		virtual ~InputManager() {}
	};
}
#endif
