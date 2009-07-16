/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   SystemType.hpp
*  @date   2009/04/25
*/
#pragma once
#ifndef SYSTEMTYPES_H
#define SYSTEMTYPES_H

#include <string>

namespace System
{
	namespace Types
	{
		enum Type
		{
			NONE = 0,
			SCRIPT = 1,
			RENDER = 2,
			GEOMETRY = 3,
			INPUT = 4,
			PHYSICS = 5,
			AI = 6,
			UX = 7,
			SOUND = 8,
			ANIMATION = 9,
			NETWORK = 10,
			TEST = 11,
			SYSTEMTYPES_MAX
		};
	};

	namespace TypeStrings
	{
		static const std::string RENDERER				= "graphics";
		static const std::string SCRIPT					= "script";
		static const std::string GEOMETRY				= "geometry";
		static const std::string PHYSICS				= "physics";
		static const std::string INPUT					= "input";
		static const std::string AI						= "ai";
		static const std::string SOUND					= "sound";
		static const std::string ANIMATION				= "animation";
		static const std::string NETWORK				= "network";
		static const std::string UX						= "ux";
	};

	namespace Queues
	{
		enum Queue
		{
			LOGIC = 0,
			HOUSE = 1,
			RENDER = 2
		};
	};

	namespace Options
	{
		static const std::string LevelName				=	"levelName";
		static const std::string DedicatedServer		=	"dedicated";
	}

	typedef std::string Attribute;

	namespace Attributes
	{
		static const Attribute Name						=	"name";
		static const Attribute LookAt					=	"lookAt";
		static const Attribute POI						=	"poi";
		static const Attribute Position					=	"position";
		static const Attribute Orientation				=	"orientation";
		static const Attribute Scale					=	"scale";
		static const Attribute FilePath					=	"filePath";
		static const Attribute SystemType				=	"system";
		static const Attribute Parent					=	"parent";
		static const Attribute Message					=	"message";
		static const Attribute ComponentType			=	"type";
		
		namespace Animation
		{
			static const Attribute Animations			=	"animations";
			static const Attribute DefaultAnimation		=	"defaultAnimation";
			static const Attribute BindPose				=	"bindPose";
		}

		namespace Network
		{
			static const Attribute IsServer				=	"isServer";
		}
	};

	typedef std::string Parameter;

	namespace Parameters
	{
		static const Parameter DeltaX					=	"deltaX";
		static const Parameter DeltaY					=	"deltaY";
		static const Parameter ScriptPath				=	"scriptPath";
		static const Parameter Model					=	"model";
		static const Parameter Body						=	"body";
		static const Parameter PlayerPosition			=	"playerPosition";
		static const Parameter PlayerOrientation		=	"playerOrientation";
		static const Parameter SoundEventPath			=	"soundEventPath";
		static const Parameter Mouse					=	"mouse";
		static const Parameter Keyboard					=	"keyboard";
		static const Parameter ScriptState				=	"scriptState";
		static const Parameter Binding					=	"binding";
		static const Parameter InvertYAxis				=	"invertYAxis";
		static const Parameter SmoothMouse				=	"smoothMouse";
		static const Parameter NavMeshPath				=	"navMeshPath";
		static const Parameter NavigationMesh			=	"navMesh";
		static const Parameter MouseSensitivity			=	"mouseSensitivity";
		static const Parameter Vertices					=	"vertices";
		static const Parameter Origin					=	"origin";
		static const Parameter Destination				=	"destination";
		static const Parameter GlobalObject				=	"globalObject";
		static const Parameter GlobalName				=	"globalName";
		static const Parameter SortByyDistance			=	"sortByDistance";
		static const Parameter MaxResults				=	"maxResults";
		static const Parameter AnimationName			=	"animationName";
		static const Parameter LoopAnimation			=	"loopAnimation";
		static const Parameter AnimationWeight			=	"animationWeight";
		static const Parameter AnimationController		=	"animationController";

		namespace Network
		{
			static const Parameter HostAddress			=	"hostAddress";
			static const Parameter Port					=	"port";
			static const Parameter ComponentId			=	"componentId";
			static const Parameter ComponentMessage		=	"componentMessage";
			static const Parameter EndOfParameters		=	"endOfParameters";
			static const Parameter Sender				=	"sender";
		}

		namespace Game
		{
			static const Parameter LevelName			=	"levelName";
		}
	};

	typedef std::string Message;

	namespace Messages
	{
		static const Message All_Messages				=	"all_messages";
		static const Message SetPosition				=	"set_position";
		static const Message SetOrientation				=	"set_orientation";
		static const Message SetLookAt					=	"set_lookAt";
		static const Message TriggerSoundEvent			=	"trigger_soundevent";
		static const Message KeyOutSoundEvent			=	"keyout_soundevent";
		static const Message SetPlayerPosition			=	"set_player_position";

		static const Message Move_Forward_Pressed		=	"+move_forward";
		static const Message Move_Forward_Released		=	"-move_forward";
		static const Message Move_Backward_Pressed		=	"+move_backward";
		static const Message Move_Backward_Released		=	"-move_backward";
		static const Message Strafe_Left_Pressed		=	"+strafe_left";
		static const Message Strafe_Left_Released		=	"-strafe_left";
		static const Message Strafe_Right_Pressed		=	"+strafe_right";
		static const Message Strafe_Right_Released		=	"-strafe_right";
		static const Message Turn_Left_Pressed			=	"+turn_left";
		static const Message Turn_Left_Released			=	"-turn_left";
		static const Message Turn_Right_Pressed			=	"+turn_right";
		static const Message Turn_Right_Released		=	"-turn_right";

		static const Message Attack_Primary_Pressed		=	"+attack_primary";
		static const Message Attack_Primary_Released	=	"-attack_primary";

		static const Message Mouse_Moved				=	"mouse_moved";
		static const Message Jump						=	"jump";

		static const Message Pause_Game					=	"pause";
		static const Message AddedToComponent			=	"added_to_component";
		static const Message RegisterScriptFunctions	=	"registerScriptFunctions";
		static const Message RegisterService			=	"registerService";
		static const Message GetBindingForMessage		=	"getBindingForMessage";
		static const Message SetBindingForMessage		=	"setBindingForMessage";
		static const Message PlayAnimation				=	"playAnimation";
		static const Message SetInvertYAxis				=	"setInvertYAxis";
		static const Message LoadMesh					=	"loadNavMesh";
		static const Message RenderMesh					=	"renderMesh";
		static const Message DestroyMesh				=	"destroy";
		static const Message FindPath					=	"findPath";
		static const Message GetState					=	"getState";
		static const Message RunScript					=	"runScript";
		static const Message SetScriptGlobal			=	"setScriptGlobal";
		static const Message LoadScript					=	"loadScript";
		static const Message CastRay					=	"castRay";

		static const Message StartAnimation				=	"startAnimation";
		static const Message StopAnimation				=	"stopAnimation";
		static const Message GetAnimationState			=	"getAnimationState";

		static const Message PreInitialize				=	"preInitialize";
		static const Message PostInitialize				=	"postInitialize";

		namespace Network
		{
			static const Message Connect				=	"connect";
			static const Message ComponentUpdate		=	"updateComponent";
			static const Message NetwokSetPosition		=	"networkSetPosition";
			static const Message NetworkSetOrientation	=	"networkSetOrientation";
		}

		namespace Input
		{
			static const Message GetTextForMessage				=	"getTextForMessage";
			static const Message GetMessageBindings				=	"getMessageBindings";
			static const Message SetInputAllowed				=	"setInputAllowed";
			static const Message SetDefaultBindingForMessage	=	"setDefaultBindingForMessage";
		}

		namespace Game
		{
			static const Message ChangeLevel			=	"changeLevel";
		}
	}
};

#endif