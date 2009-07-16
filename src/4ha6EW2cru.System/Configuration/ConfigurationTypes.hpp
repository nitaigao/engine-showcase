/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   ConfigurationTypes.hpp
*  @date   2009/07/15
*/
#pragma once
#ifndef CONFIGURATIONTYPES_HPP
#define CONFIGURATIONTYPES_HPP

namespace Configuration
{
	typedef std::string ConfigSection;

	namespace ConfigSections
	{
		static const ConfigSection Graphics					=	"Graphics";
		static const ConfigSection Developer				=	"Developer";
		static const ConfigSection Logging					=	"Logging";
		static const ConfigSection Bindings					=	"Bindings";
		static const ConfigSection Input					=	"Input";
		static const ConfigSection Sound					=	"Sound";
		static const ConfigSection Network					=	"Network";
	}

	typedef std::string ConfigItem;

	namespace ConfigItems
	{
		namespace Network
		{
			static const ConfigItem ServerPort				=	"server_port";
			static const ConfigItem MaxServerConnections	=	"max_server_connections";
			static const ConfigItem MaxClientConnections	=	"max_client_connections";
			static const ConfigItem ServerSleepTime			=	"server_sleeptime";
			static const ConfigItem ClientSleepTime			=	"client_sleeptime";
			static const ConfigItem MaxServerReleaseTime	=	"max_server_release_time";
			static const ConfigItem MaxClientReleaseTime	=	"max_client_release_time";
		}

		namespace Graphics
		{
			static const ConfigItem FullScreen				=	"fullscreen";
			static const ConfigItem Width					=	"width";
			static const ConfigItem Height					=	"height";
			static const ConfigItem Depth					=	"depth";
			static const ConfigItem WindowTitle				=	"window_title";
			static const ConfigItem VSync					=	"vsync";
		}

		namespace Developer
		{
			static const ConfigItem	Console					=	"console";
		}

		namespace Input
		{
			static const ConfigItem InvertY					=	"inverty";
			static const ConfigItem SmoothMouse				=	"smoothmouse";
			static const ConfigItem MouseSmoothAmount		=	"mousesmooth_amount";
		}

		namespace Sound
		{
			static const ConfigItem SFXVolume				=	"sfx_volume";
			static const ConfigItem MusicVolume				=	"music_volume";
		}

		namespace Logging
		{
			static const ConfigItem LogLevel				=	"level";
		}
	}
};

#endif