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
#ifndef OIS_CONFIG_HEADER
#define OIS_CONFIG_HEADER

//----------------------------------------------------------------------------//
//* This file contains defines for building certain parts of this Lib
//*	This file is not meant for External inclusion. However, you can edit this 
//* file before a build to effect what features are used internally
//----------------------------------------------------------------------------//

/** 
@remarks
	These lines have no bearing on internal build of OIS. This is here for
	external build settings. Meaning, changing this does not require a
	rebuild of OIS. This just affects VC dll import settings, as the DLL
	build already defines this during its build for setting dll exports.
	The undefine here is here just incase you decide to only use
	DLL, and want to set it permently and recompile OIS too.. Avoid redefinition
	from DLL build of OIS.

	So, if wanting to link against DLL version, just uncomment these lines.
*/
//#ifdef OIS_DYNAMIC_LIB
//#  undef OIS_DYNAMIC_LIB
//#endif
//#define OIS_DYNAMIC_LIB

#endif
