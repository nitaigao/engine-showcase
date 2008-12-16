#include "GuiComponent.h"

#include <Ogre.h>

#include "GUIView.h"
#include "GuiModel.h"
#include "GuiController.h"

#include <luabind/luabind.hpp>
using namespace luabind;

#include "GuiTools.hpp"

#include "../../Logging/Logger.h"
#include "../../Common/Paths.hpp"

#include "../../IO/FileManager.h"
#include "../../Scripting/ScriptManager.h"

#include "../../Exceptions/NullReferenceException.hpp"
#include "../../Exceptions/FileNotFoundException.hpp"

void GuiComponent::Initialize( )
{
	if ( !_script ) 
	{	
		std::stringstream scriptPath;
		scriptPath << Paths::GetViewsPath( ) << _name << "/" << _name << ".lua";
	
		_script = ScriptManager::GetInstance( )->CreateScript( scriptPath.str( ) );
	}
		
	module( _script->GetState( ) )
	[
		class_< GuiModel >( "Model" )
		.def( constructor< luabind::object >( ) )
			.def( "addEventListener", &GuiModel::AddEventListener )
			.def( "executeCommand", &GuiModel::FromLua_ExecuteCommand ),

		class_< GuiView >( "View" )
			.def( constructor< std::string >( ) )
			.def( "initialize", &GuiView::Initialize )
			.def( "findControl", &GuiView::FindControl )
			.def( "addEventListener", &GuiView::AddEventListener ),

		class_< GuiController >( "Controller" )
			.def( constructor< std::string >( ) ),
			
		class_< MyGUI::IntCoord >( "IntCoord" )
			.def_readonly( "x" , &MyGUI::IntCoord::left )
			.def_readonly( "y" , &MyGUI::IntCoord::top )
			.def_readonly( "width" , &MyGUI::IntCoord::width )
			.def_readonly( "height" , &MyGUI::IntCoord::height ),

		class_< MyGUI::Widget >( "Widget" )
			.def( "getDimensions", &MyGUI::Widget::getClientCoord )
			.def( "setPosition", ( void( MyGUI::Widget::* )( int, int ) ) &MyGUI::Widget::setPosition )
			.def( "getType", &MyGUI::Widget::getClassTypeName )
			.def( "hide", &MyGUI::Widget::hide )
			.def( "show", &MyGUI::Widget::show )
			.def( "toWindow", &GuiTools::FromLua_ToWindow )
			.def( "toStaticText", &GuiTools::FromLua_ToStaticText ),
			
		class_< MyGUI::Window >( "Window" )
			.def( "getType", MyGUI::Window::getClassTypeName ),
			
		class_< MyGUI::StaticText, MyGUI::Widget >( "StaticText" )
			.def( "setText", &MyGUI::StaticText::setCaption )
			.def( "getText", &MyGUI::StaticText::getCaption ),

		class_< Ogre::UTFString >( "utfstring" ),
		def( "toString", &StringUtils::ToString ),
		def( "toUTFString", &StringUtils::ToUTFString )
	];

	_script->Initialize( );
	_script->CallFunction( "onLoad" );
}

GuiComponent::~GuiComponent( )
{
	ScriptManager::GetInstance( )->DestroyScript( _script );
}