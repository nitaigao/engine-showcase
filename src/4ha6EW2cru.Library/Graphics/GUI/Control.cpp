#include "Control.h"

#include "View.h"

#include "../../Logging/Logger.h"
#include "../../Common/Paths.hpp"

#include "../../IO/FileManager.h"

#include "../../Exceptions/NullReferenceException.hpp"
#include "../../Exceptions/FileNotFoundException.hpp"

#include <luabind/luabind.hpp>
using namespace luabind;

void Control::Initialize( )
{
	std::stringstream scriptPath;
	scriptPath << Paths::GetControlsPath( ) << _name << "/" << _name << ".controller.lua";

	FileBuffer* controllerBuffer = FileManager::GetInstance( )->GetFile( scriptPath.str( ) );
	_controller = Script::CreateFromFileBuffer( controllerBuffer );
	_controller->Initialize( );

	module( _controller->GetState( ) )
	[
		class_< View >( "View" )
			.def( constructor< std::string >( ) )
			.def( "initialize", &View::Initialize )
			.def( "findControl", &View::FindControl )
	];

	_controller->CallFunction( "onLoad" );
}

Control::~Control( )
{
	delete _controller;
}






















/*if ( 0 == _gui )
{
NullReferenceException nullGuiE( "Control::Initialize - Cannot Initialize, the GUI System is NULL" );
Logger::GetInstance( )->Fatal( nullGuiE.what( )	);
throw nullGuiE;
}

std::stringstream layoutPath;
layoutPath << Paths::GetControlsPath( ) << _name << "/" << _name << ".layout";

if ( !FileManager::GetInstance( )->FileExists( layoutPath.str( ) ) )
{
FileNotFoundException fileNotFound( "Control::Initialize - Layout file does not exist" );
Logger::GetInstance( )->Fatal( fileNotFound.what( ) );
throw fileNotFound;
}

_gui->load( layoutPath.str( ) );*/