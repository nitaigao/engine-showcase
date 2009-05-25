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
		static const std::string Input						=	"Input";
		static const std::string Sound						=	"Sound";
	};

	namespace Options
	{
		static const std::string LevelName					=	"levelName";
	}

	typedef std::string Attribute;

	namespace Attributes
	{
		static const std::string Name						=	"name";
		static const std::string LookAt						=	"lookAt";
		static const std::string DeltaX						=	"deltaX";
		static const std::string DeltaY						=	"deltaY";
		static const std::string ScriptPath					=	"scriptPath";
		static const std::string Position					=	"position";
		static const std::string Orientation				=	"orientation";
		static const std::string Scale						=	"scale";
		static const std::string Model						=	"model";
		static const std::string Body						=	"body";
		static const std::string Type						=	"type";
		static const std::string Parent						=	"parent";
		static const std::string PlayerPosition				=	"playerPosition";
		static const std::string PlayerOrientation			=	"playerOrientation";
		static const std::string SoundEventPath				=	"soundEventPath";
		static const std::string Mouse						=	"mouse";
		static const std::string Keyboard					=	"keyboard";
		static const std::string ScriptState				=	"scriptState";
		static const std::string Message					=	"message";
		static const std::string Binding					=	"binding";
		static const std::string InvertYAxis				=	"invertYAxis";
		static const std::string SmoothMouse				=	"smoothMouse";
		static const std::string MouseSensitivity			=	"mouseSensitivity";
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
		static const std::string SetBindingForMessage		=	"setBindingForMessage";
		static const std::string PlayAnimation				=	"playAnimation";
		static const std::string SetInvertYAxis				=	"setInvertYAxis";
	}
};

#endif