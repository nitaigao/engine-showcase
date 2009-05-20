/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   SystemType.hpp
*  @date   2009/04/25
*/
#ifndef SYSTEMTYPES_H
#define SYSTEMTYPES_H

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

	namespace Attributes
	{
		static const unsigned int Name				=	( 1 << 1 );
		static const unsigned int LookAt			=	( 1 << 2 );
		static const unsigned int DeltaX			=	( 1 << 3 );
		static const unsigned int DeltaY			=	( 1 << 4 );
		static const unsigned int ScriptPath		=	( 1 << 5 );
		static const unsigned int Position			=	( 1 << 6 );
		static const unsigned int Orientation		=	( 1 << 7 );
		static const unsigned int Scale				=	( 1 << 8 );
		static const unsigned int Model				=	( 1 << 9 );
		static const unsigned int Body				=	( 1 << 10 );
		static const unsigned int Type				=	( 1 << 11 );
		static const unsigned int Parent			=	( 1 << 12 );
		static const unsigned int PlayerPosition	=	( 1 << 13 );
		static const unsigned int PlayerOrientation =	( 1 << 14 );
		static const unsigned int SoundEventPath	=	( 1 << 15 );
	};

	namespace Messages
	{
		static const unsigned int SetPosition		=	( 1 << 1 );
		static const unsigned int SetOrientation	=	( 1 << 2 );
		static const unsigned int SetLookAt			=	( 1 << 3 );
		static const unsigned int TriggerSoundEvent	=	( 1 << 4 );
		static const unsigned int KeyOutSoundEvent	=	( 1 << 5 );
		static const unsigned int SetPlayerPosition	=	( 1 << 6 );
		static const unsigned int Move_Forward		=	( 1 << 7 );
		static const unsigned int Move_Backward		=	( 1 << 8 );
		static const unsigned int Strafe_Left		=	( 1 << 9 );
		static const unsigned int Strafe_Right		=	( 1 << 10 );
		static const unsigned int Turn_Left			=	( 1 << 11 );
		static const unsigned int Turn_Right		=	( 1 << 12 );
		static const unsigned int Mouse_Moved		=	( 1 << 13 );
		static const unsigned int Jump				=	( 1 << 14 );
		static const unsigned int Fire				=	( 1 << 15 );
		static const unsigned int Move_Idle			=	( 1 << 16 );
		static const unsigned int Attack_Idle		=	( 1 << 17 );
		static const unsigned int Pause_Game		=	( 1 << 18 );
		static const unsigned int AddedToComponent	=	( 1 << 19 );
	}
};

#endif