/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   SystemType.hpp
*  @date   2009/04/25
*/
#ifndef __SYSTEMTYPES_H
#define __SYSTEMTYPES_H

#include <string>

namespace System
{
	namespace Types
	{
		enum Type
		{
			NONE,
			SCRIPT,
			RENDER,
			GEOMETRY,
			INPUT,
			PHYSICS,
			AI,
			UX,
			SOUND,
			TEST
		};
	};

	namespace Options
	{
		static const std::string LevelName			=	"levelName";
	}

	namespace Changes
	{
		static const unsigned int None				=	( 1 << 1 );

		namespace Geometry
		{

			static const unsigned int Position		=	( 1 << 2 );
			static const unsigned int Orientation	=	( 1 << 3 );
			static const unsigned int Scale			=	( 1 << 4 );
			static const unsigned int All			=	Position | Orientation | Scale;

		};

		namespace Input
		{
			static const unsigned int Move_Forward	=	( 1 << 5 );
			static const unsigned int Move_Backward	=	( 1 << 6 );
			static const unsigned int Strafe_Left	=	( 1 << 7 );
			static const unsigned int Strafe_Right	=	( 1 << 8 );
			static const unsigned int Turn_Left		=	( 1 << 9 );
			static const unsigned int Turn_Right	=	( 1 << 10 );
			static const unsigned int Pause_Game	=	( 1 << 11 );
			static const unsigned int Mouse_Moved	=	( 1 << 12 );
			static const unsigned int Jump			=	( 1 << 13 );
			static const unsigned int Fire			=	( 1 << 14 );
			static const unsigned int All			=	Move_Forward | Move_Backward | Strafe_Left | Strafe_Right | Turn_Left | Turn_Right | Pause_Game | Mouse_Moved | Jump | Fire;
		};

		namespace Render
		{
			static const unsigned int Ray_Cast		=	( 1 << 15 );

		};

		namespace AI
		{
			static const unsigned int Behavior		=	( 1 << 16 );
		}
	};
};

#endif