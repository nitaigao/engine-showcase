#ifndef __SUITES_H
#define __SUITES_H

#include <string>

class Suites
{

public:

	static std::string EventsSuite( ) { return "Events"; };
	static std::string GraphicsSuite( ) { return "Graphics"; };
	static std::string InputSuite( ) { return "Input"; };
	static std::string IOSuite( ) { return "IO"; };
	static std::string LoggingSuite( ) { return "Logging"; };
	static std::string ScriptingSuite( ) { return "Scripting"; };
	static std::string SystemSuite( ) { return "System"; };
	static std::string ViewSuite( ) { return "View"; };
	static std::string TypesSuite( ) { return "Types"; };
	
};

#endif