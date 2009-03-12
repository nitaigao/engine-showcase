#ifndef __SYSTEMTYPES_H
#define __SYSTEMTYPES_H

enum SystemType
{
	ScriptSystemType,
	RenderSystemType,
	GeometrySystemType,
	InputSystemType,
	PhysicsSystemType,
	TestSystemType,
	Max_SystemTypes
};

namespace System
{
	namespace Changes
	{
		namespace Geometry
		{

			static const unsigned int All			=	( 1 << 0 );
			static const unsigned int Position		=	( 1 << 1 );
			static const unsigned int Orientation	=	( 1 << 2 );
			static const unsigned int Scale			=	( 1 << 3 );

		};

		namespace Physics
		{
			static const unsigned int All			=	( 1 << 4 );
			static const unsigned int Position		=	( 1 << 5 );
			static const unsigned int Orientation	=	( 1 << 6 );
			static const unsigned int Scale			=	( 1 << 7 );
		};
	};
};

#endif