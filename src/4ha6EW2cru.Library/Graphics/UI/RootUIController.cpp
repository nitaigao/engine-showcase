#include "RootUIController.h"

#include "../../Scripting/ScriptManager.h"

#include "UIView.h"
#include "UIController.h"

RootUIController::~RootUIController( )
{
	this->DestroyAllComponents( );
}

void RootUIController::Initialize( )
{
	
}

void RootUIController::Update( int deltaMilliseconds )
{
	while( _eventQueue.size( ) > 0 )
	{
		luabind::call_member< void >( _luaObject, "onMessage", _eventQueue.front( ) );

		_eventQueue.pop( );
	}
}

void RootUIController::LoadComponent( const std::string componentName )
{
	std::stringstream scriptPath;
	scriptPath << "/data/gui/components/" << componentName << "/" << componentName << ".lua";

	Script* script = ScriptManager::GetInstance( )->CreateScript( scriptPath.str( ) );
	_components.push_back( script );

	module( script->GetState( ) )
	[
		class_< RootUIController >( "RootUIController" )
		.def( constructor< luabind::object, Gui* >( ) )
			.def( "dispatchMessage", &RootUIController::QueueEvent )
			.def( "getScreenWidth", &RootUIController::GetScreenWidth )
			.def( "getScreenHeight", &RootUIController::GetScreenHeight )
			.def( "showMouse", &RootUIController::ShowMouse )
			.def( "hideMouse", &RootUIController::HideMouse ),

		class_< UIController >( "UIController" )
			.def( constructor< luabind::object, std::string, Gui* >( ) )
			.def( "setWidgetScript", &UIController::SetWidgetScript ),

		class_< UIView >( "UIView" )
			.def( constructor< std::string, Gui* >( ) )
			.def( "initialize", &UIView::Initialize )
			.def( "findControl", &UIView::FindControl ),

		class_< Gui >( "Gui" ),

		class_< Widget >( "Widget" )
			.def( "getDimensions", &MyGUI::Widget::getClientCoord )
			.def( "setPosition", ( void( MyGUI::Widget::* )( int, int ) ) &MyGUI::Widget::setPosition )
			.def( "getType", &MyGUI::Widget::getClassTypeName )
			.def( "hide", &MyGUI::Widget::hide )
			.def( "show", &MyGUI::Widget::show )
			.def( "setScript", &UIController::SetWidgetScript ),

		class_< IntCoord >( "IntCoord" )
			.def_readonly( "x" , &MyGUI::IntCoord::left )
			.def_readonly( "y" , &MyGUI::IntCoord::top )
			.def_readonly( "width" , &MyGUI::IntCoord::width )
			.def_readonly( "height" , &MyGUI::IntCoord::height )
	];

	script->Initialize( );

	luabind::globals( script->GetState( ) )[ "root" ] = this;

	luabind::call_function< void >( script->GetState( ), "onLoad", this, componentName, _gui );
}

void RootUIController::DestroyAllComponents( )
{
	for( UIComponentList::iterator i = _components.begin( ); i != _components.end( ); ++i )
	{
		delete ( *i );
	}

	_components.clear( );
	_gui->destroyAllChildWidget( );
}