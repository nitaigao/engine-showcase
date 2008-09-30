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
			class_< Ogre::UTFString >( "utfstring" ),

			class_< GuiModel, GuiModel_Wrapper >( "Model" )
				.def( constructor< >( ) )
				.def( "addEventListener", &Script::FromLua_AddEventListener ),

			class_< GuiView, GuiView_Wrapper >( "View" )
				.def( constructor< std::string >( ) )
				.def( "initialize", &GuiView::Initialize )
				.def( "findControl", &GuiView::FindControl ),

			class_< GuiController >( "Controller" )
				.def( constructor< std::string >( ) )
				.def( "initialize", &GuiController::Initialize ),

			class_< MyGUI::Widget >( "Widget" )
				.def( "setText", &MyGUI::StaticText::setCaption )
				.def( "getText", &MyGUI::StaticText::getCaption )
				.def( "addEventListener", &GuiView::FromLua_AddEventListener ),

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