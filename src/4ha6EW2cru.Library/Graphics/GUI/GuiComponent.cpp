#include "GuiComponent.h"

#include <Ogre.h>

#include "GUIView.h"
#include "GuiModel.h"
#include "GuiController.h"

#include <luabind/luabind.hpp>
using namespace luabind;

#include "../../Logging/Logger.h"
#include "../../Common/Paths.hpp"

#include "../../IO/FileManager.h"

#include "../../Exceptions/NullReferenceException.hpp"
#include "../../Exceptions/FileNotFoundException.hpp"

void GuiComponent::Initialize( )
{
	std::stringstream scriptPath;
	scriptPath << Paths::GetControlsPath( ) << _name << "/" << _name << ".lua";

	FileBuffer* controllerBuffer = FileManager::GetInstance( )->GetFile( scriptPath.str( ) );
	_script = Script::CreateFromFileBuffer( controllerBuffer );

	module( _script->GetState( ) )
	[
		class_< GuiModel >( "Model" )
			.def( constructor< >( ) )
			.def( "addEventListener", &GuiModel::FromLua_AddEventListener ),

		class_< GuiView >( "View" )
			.def( constructor< std::string >( ) )
			.def( "initialize", &GuiView::Initialize )
			.def( "findControl", &GuiView::FindControl ),

		class_< GuiController >( "Controller" )
			.def( constructor< std::string >( ) ),

		class_< MyGUI::Widget >( "Widget" )
			.def( "setText", &MyGUI::StaticText::setCaption )
			.def( "getText", &MyGUI::StaticText::getCaption )
			.def( "addEventListener", &GuiController::FromLua_AddEventListener ),

		class_< Ogre::UTFString >( "utfstring" ),
		def( "toString", &StringUtils::ToString ),
		def( "toUTFString", &StringUtils::ToUTFString )
	];

	_script->Initialize( );
	_script->CallFunction( "onLoad" );
}

GuiComponent::~GuiComponent( )
{
	delete _script;
}