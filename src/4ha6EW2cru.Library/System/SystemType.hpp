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

		namespace Input
		{
			static const unsigned int Move_Forward	=	( 1 << 8 );
			static const unsigned int Move_Backward	=	( 1 << 9 );
			static const unsigned int Strafe_Left	=	( 1 << 10 );
			static const unsigned int Strafe_Right	=	( 1 << 11 );
			static const unsigned int Turn_Left		=	( 1 << 12 );
			static const unsigned int Turn_Right	=	( 1 << 13 );
			static const unsigned int Pause_Game	=	( 1 << 14 );
			static const unsigned int Mouse_Moved	=	( 1 << 15 );
			static const unsigned int Jump			=	( 1 << 16 );
		};
	};
};

#endif