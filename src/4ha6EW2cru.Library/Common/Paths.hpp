#ifndef __PATHS_H
#define __PATHS_H

/*!
	Contains all File System Path's in use by the Game
*/
class Paths
{

public:

	/*! Gets the Path to the default System log file */
	static std::string GetLogFilePath( ) { return "../logs/system.log"; };

	/*! Gets the Path to the default game folder */
	static std::string GetGameFolderPath( ) { return "../game"; };

	/*! Gets the Path to the Screen Information Files */
	static std::string GetScreensPath( ) { return "gui/screens/"; };

	/*! Gets the Path to the HumanView */
	static std::string GetHumanViewPath( ) { return "../game/views/humanview"; };

	/*! Gets the Path to the GUI folder */
	static std::string GetGUIFolder( ) { return "../game/views/humanview/gui"; };

	/*! GEts the Path to the Configuration directory */
	static std::string GetConfigPath( ) { return "../game/config"; };

	/*! Get the path to the views.bad file */
	static std::string GetViewPackagePath( ) { return "../game/data/views.bad"; };

	/*! Get the path to the gui.bad file */
	static std::string GetGUIPackagePath( ) { return "../game/data/gui.bad"; };

	/*! Get the path to the gui controls directory */
	static std::string GetControlsPath( ) { return "gui/controls/"; };
};

#endif