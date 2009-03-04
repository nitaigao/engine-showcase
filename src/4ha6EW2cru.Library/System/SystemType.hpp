#ifndef __SYSTEMTYPES_H
#define __SYSTEMTYPES_H

enum SystemType
{
	ScriptSystemType,
	RenderSystemType,
	GeometrySystemType,
	InputSystemType,
	TestSystemType,
	Max_SystemTypes
};

namespace System
{
	namespace Changes
	{
		static const unsigned int All				=	( 1 << 0 );

		namespace Geometry
		{

			static const unsigned int Position		=	( 1 << 1 );
			static const unsigned int Orientation	=	( 1 << 2 );
			static const unsigned int Scale			=	( 1 << 3 );

		};
	};
};

#endif