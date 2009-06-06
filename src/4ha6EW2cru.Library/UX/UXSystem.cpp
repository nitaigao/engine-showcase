#include "UXSystem.h"

#include "UXSystemScene.h"
#include "UXSystemComponent.h"

#include "../Management/Management.h"

#include <MyGUI.h>
using namespace MyGUI;

#include <luabind/luabind.hpp>
#include <luabind/table_policy.hpp>
using namespace luabind;

namespace UX
{
	ISystemScene* UXSystem::CreateScene()
	{
		m_scene = new UXSystemScene( );
		m_scene->Initialize( ); 
		return m_scene;
	}

	AnyType::AnyTypeMap UXSystem::Execute( const std::string& actionName, AnyType::AnyTypeMap& parameters )
	{
		if ( actionName == System::Messages::RegisterScriptFunctions )
		{
			module( parameters[ System::Parameters::ScriptState ].As< lua_State* >( ) )
			[
				class_< UXSystemComponent >( "UXSystemComponent" )
					.def( "findWidget", &UXSystemComponent::FindWidget )
					.def( "loadComponent", &UXSystemComponent::LoadComponent )
					.def( "getScreenWidth", &UXSystemComponent::GetScreenWidth )
					.def( "getScreenHeight", &UXSystemComponent::GetScreenHeight )
					.def( "getSupportedResolutions", &UXSystemComponent::GetSupportedResolutions, copy_table( result ) )
					.def( "getMessageBinding", &UXSystemComponent::GetMessageBinding )
					.def( "setMessageBinding", &UXSystemComponent::SetMessageBinding )
					.def( "scriptWidget", &UXSystemComponent::ScriptWidget )
					.def( "unscriptWidget", &UXSystemComponent::UnScriptWidget) 
					.def( "showMouse", &UXSystemComponent::ShowMouse )
					.def( "hideMouse", &UXSystemComponent::HideMouse )
					.def( "setInputAllowed", &UXSystemComponent::SetInputAllowed )
					.def( "changeResolution", &UXSystemComponent::ChangeResolution )
					.def( "setFarClip", &UXSystemComponent::SetFarClip )
					.def( "getFps", &UXSystemComponent::GetFps ),

				class_< Widget >( "Widget" )
					.def( "getDimensions", &Widget::getClientCoord )
					.def( "setSize", ( void( Widget::* )( int, int ) ) &Widget::setSize )
					.def( "setPosition", ( void( Widget::* )( int, int ) ) &Widget::setPosition )
					.def( "getType", &Widget::getClassTypeName )
					.def( "setVisible", &Widget::setVisible )
					.def( "isVisible", &Widget::isVisible )
					.def( "getAlpha", &Widget::getAlpha )
					.def( "setAlpha", &Widget::setAlpha )
					.def( "getText", &UXSystemScene::GetText )
					.def( "setText", &UXSystemScene::SetText )
					.def( "asButton", &UXSystemScene::AsButton )
					.def( "asComboBox", &UXSystemScene::AsComboBox )
					.def( "asEditBox", &UXSystemScene::AsEditBox )
					.def( "asProgressBar", &UXSystemScene::AsProgressBar )
					.def( "asMultiList", &UXSystemScene::AsMultiList )
					.def( "asScrollbar", &UXSystemScene::AsScrollBar )
					.def( "setFocus", &UXSystemScene::SetFocus ),

				class_< Button >( "Button" )
					.def( "setChecked", &Button::setStateCheck )
					.def( "getChecked", &Button::getStateCheck ),

				class_< ComboBox, Widget >( "ComboBox" )
					.def( "addItem", ( void ( ComboBox::* ) ( const std::string&, const std::string& ) ) &ComboBox::addItem )
					.def( "getValueAt", ( const std::string& ( ComboBox::* ) ( int ) ) &ComboBox::getItemNameAt )
					.def( "getSelectedIndex", &ComboBox::getIndexSelected )
					.def( "setSelectedIndex", &ComboBox::setIndexSelected ),

				class_< Edit, Widget >( "EditBox" )
					.def( "addText", &UXSystemScene::AddText ),

				class_< Progress, Widget >( "ProgressBar" )
					.def( "setProgress", &Progress::setProgressPosition ),

				class_< MultiList, Widget >( "MultiList" )
					.def( "addColumn", &UXSystemScene::MultiList_AddColumn )
					.def( "removeAllColumns", &MultiList::removeAllColumns )
					.def( "removeAllItems", &MultiList::removeAllItems )
					.def( "addItem", &UXSystemScene::MultiList_AddItem )
					.def( "getItemName", &UXSystemScene::MultiList_GetItemName )
					.def( "getSubItemName", &UXSystemScene::MultiList_GetSubItemName )
					.def( "setSubItemName", &UXSystemScene::MultiList_SetSubItemName  ),

				class_< VScroll, Widget >( "Scrollbar" )
					.def( "getScrollPosition", &VScroll::getScrollPosition )
					.def( "setScrollPosition", &VScroll::setScrollPosition ),

				class_< IntCoord >( "IntCoord" )
					.def_readonly( "x" , &IntCoord::left )
					.def_readonly( "y" , &IntCoord::top )
					.def_readonly( "width" , &IntCoord::width )
					.def_readonly( "height" , &IntCoord::height ),

				class_< Any >( "Any" )
					.def( constructor<>( ) )
			];
		}

		return AnyType::AnyTypeMap( );
	}

	void UXSystem::Message( const std::string& message, AnyType::AnyTypeMap parameters )
	{
		if ( message == System::Messages::RegisterService )
		{
			Management::GetServiceManager( )->RegisterService( this );
		}
	}
};