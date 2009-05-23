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

	namespace ConfigSections
	{
		static const std::string Graphics					=	"Graphics";
		static const std::string Developer					=	"Developer";
		static const std::string Logging					=	"Logging";
		static const std::string Bindings					=	"Bindings";
	};

	namespace Options
	{
		static const std::string LevelName					=	"levelName";
	}

	namespace Attributes
	{
		static const unsigned int Name						=	( 1 << 1 );
		static const unsigned int LookAt					=	( 1 << 2 );
		static const unsigned int DeltaX					=	( 1 << 3 );
		static const unsigned int DeltaY					=	( 1 << 4 );
		static const unsigned int ScriptPath				=	( 1 << 5 );
		static const unsigned int Position					=	( 1 << 6 );
		static const unsigned int Orientation				=	( 1 << 7 );
		static const unsigned int Scale						=	( 1 << 8 );
		static const unsigned int Model						=	( 1 << 9 );
		static const unsigned int Body						=	( 1 << 10 );
		static const unsigned int Type						=	( 1 << 11 );
		static const unsigned int Parent					=	( 1 << 12 );
		static const unsigned int PlayerPosition			=	( 1 << 13 );
		static const unsigned int PlayerOrientation			=	( 1 << 14 );
		static const unsigned int SoundEventPath			=	( 1 << 15 );
		static const unsigned int Mouse						=	( 1 << 16 );
		static const unsigned int Keyboard					=	( 1 << 17 );
		static const std::string ScriptState				=	"scriptState";
		static const std::string Message					=	"message";
	};

	typedef std::string Message;

	namespace Messages
	{
		static const std::string SetPosition				=	"set_position";
		static const std::string SetOrientation				=	"set_orientation";
		static const std::string SetLookAt					=	"set_lookAt";
		static const std::string TriggerSoundEvent			=	"trigger_soundevent";
		static const std::string KeyOutSoundEvent			=	"keyout_soundevent";
		static const std::string SetPlayerPosition			=	"set_player_position";
		static const std::string Move_Forward				=	"move_forward";
		static const std::string Move_Backward				=	"move_backward";
		static const std::string Strafe_Left				=	"strafe_left";
		static const std::string Strafe_Right				=	"strafe_right";
		static const std::string Turn_Left					=	"turn_left";
		static const std::string Turn_Right					=	"turn_right";
		static const std::string Mouse_Moved				=	"mouse_moved";
		static const std::string Jump						=	"jump";
		static const std::string Fire						=	"attack_primary";
		static const std::string Move_Idle					=	"idle_move";
		static const std::string Attack_Idle				=	"idle_attack";
		static const std::string Pause_Game					=	"pause";
		static const std::string AddedToComponent			=	"added_to_component";
		static const std::string RegisterScriptFunctions	=	"registerScriptFunctions";
		static const std::string RegisterService			=	"registerService";
		static const std::string GetBindingForMessage		=	"getBindingForMessage";
		static const std::string PlayAnimation				=	"playAnimation";
	}
};

#endif