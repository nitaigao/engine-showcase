#include "UIComponent.h"

UIComponent::~UIComponent( )
{
	if ( _script != 0 )
	{
		delete _script;
	}
}

void UIComponent::Initialize( )
{
	std::stringstream layoutPath;
	layoutPath << "/data/gui/components/" << _name << "/" << _name << ".layout";

	_gui->load( layoutPath.str( ) );

	/*std::stringstream scriptPath;
	scriptPath << "/data/gui/components/" << _name << "/" << _name << ".lua";
	
	_script = ScriptManager::GetInstance( )->CreateScript( scriptPath.str( ) );

	module( _script->GetState( ) )
	[
		class_< UIController >( "UIController" )
	];

	_script->Initialize( );

	luabind::call_function< void >( _script->GetState( ), "onLoad", _parentController );*/
}