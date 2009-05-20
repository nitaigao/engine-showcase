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
#ifndef OIS_Prereqs_H
#define OIS_Prereqs_H
//----------------------------------------------------------------------------//
//* This Header File contains forward declared classes                       *//
//*	Also, common properties for the keyboard and mouse                       *//
//* There is also preprocessor defines to determine type of OS               *//
//----------------------------------------------------------------------------//

//-------------- Common STL Containers ---------------------------------------//
#include <vector>
#include <string>
#include <map>
#include "OISConfig.h"

// Default is blank for most OS's
#define _OISExport

//-------------- Determine Compiler ---------------------------------
#if defined( _MSC_VER )
#	define OIS_MSVC_COMPILER
#elif defined( __GNUC__ )
#	if defined( __WIN32__ ) || defined( _WIN32 )
#		define OIS_MINGW_COMPILER
#	else
#		define OIS_GCC_COMPILER
#	endif
#elif defined( __BORLANDC__ )
#	define OIS_BORLAND_COMPILER
#else
#	error No Recognized Compiler!
#endif

// --------------- Determine Operating System Platform ---------------
#if defined( __WIN32__ ) || defined( _WIN32 ) // Windows 2000, XP, ETC
#	if defined ( _XBOX )
#		define OIS_XBOX_PLATFORM
#	else
#		define OIS_WIN32_PLATFORM
#		if defined( OIS_DYNAMIC_LIB ) && !defined(OIS_MINGW_COMPILER)
#			undef _OISExport
			//Ignorable Dll interface warning...
#			pragma warning (disable : 4251)
#			if defined( OIS_NONCLIENT_BUILD )
#				define _OISExport __declspec( dllexport )
#			else
#				define _OISExport __declspec( dllimport )
#			endif
#		endif
#	endif
#elif defined( __APPLE_CC__ ) // Apple OS X
#	define OIS_APPLE_PLATFORM
#else //Probably Linux
#	define OIS_LINUX_PLATFORM
#endif

//Is Processor 32 or 64 bits...
#if defined(__x86_64__)
#	define OIS_ARCH_64
#else
#	define OIS_ARCH_32
#endif

//-------------- Common Classes, Enums, and Typdef's -------------------------//
#define OIS_VERSION_MAJOR 1
#define OIS_VERSION_MINOR 0
#define OIS_VERSION_PATCH 0
#define OIS_VERSION_NAME "Nitro"

#define OIS_VERSION ((OIS_VERSION_MAJOR << 16) | (OIS_VERSION_MINOR << 8) | OIS_VERSION_PATCH)

namespace OIS
{
	//Device Types
	class InputManager;
	class Object;
	class Keyboard;
	class Mouse;
	class JoyStick;
	class KeyListener;
	class MouseListener;
	class JoyStickListener;
	class Interface;
	class ForceFeedback;
	class Effect;

	//! Way to send OS nuetral parameters.. ie OS Window handles, modes, flags
	typedef _OISExport std::multimap<std::string, std::string> ParamList;

	//! Each Input class has a General Type variable, a form of RTTI
	enum _OISExport Type
	{
		OISUnknown   = 0,
		OISKeyboard  = 1,
		OISMouse     = 2,
		OISJoyStick  = 3,
		OISTablet    = 4
	};

	//--------     Shared common components    ------------------------//

	//! Base type for all device components (button, axis, etc)
	enum _OISExport ComponentType
	{
		OIS_Unknown = 0,
		OIS_Button  = 1, //Key, mouse button, joy button, etc
		OIS_Axis    = 2,
		OIS_Slider  = 3,
		OIS_POV     = 4
	};

	//! Base of all device components (button, axis, etc)
	struct _OISExport Component
	{
		Component() : cType(OIS_Unknown) {};
		Component(ComponentType type) : cType(type) {};
		//! Indicates what type of coponent this is
		ComponentType cType;
	};

	//! Button can be a keyboard key, mouse button, etc
	struct _OISExport Button : Component
	{
		Button() {}
		Button(bool bPushed) : Component(OIS_Button), pushed(bPushed) {};
		//! true if pushed, false otherwise
		bool pushed;
	};

	//! Axis component
	struct _OISExport Axis : Component
	{
		Axis() : Component(OIS_Axis), abs(0), rel(0), absOnly(false) {};

		//! Absoulte and Relative value components
		int abs, rel;

		//! Indicates if this Axis only supports Absoulte (ie JoyStick)
		bool absOnly;

		//! Used internally by OIS
		void clear()
		{
			abs = rel = 0;
		}
	};
}

#endif //end if prereq header defined
