#include "UXSystemScene.h"

using namespace MyGUI;

#include "../Service/IService.hpp"
#include "../Management/Management.h"

#include <luabind/table_policy.hpp>
using namespace luabind;

#include "../Events/EventType.hpp"
#include "../Events/EventData.hpp"
using namespace Events;

#include "UXSystemComponent.h"

namespace UX
{
	UXSystemScene::~UXSystemScene()
	{
		delete m_gui;
		m_gui = 0;
	}

	void UXSystemScene::Initialize( )
	{
		IService* renderService = Management::GetInstance( )->GetServiceManager( )->FindService( System::Types::RENDER );
		Ogre::RenderWindow* renderWindow = renderService->Execute( "getRenderWindow", AnyValue::AnyValueMap( ) )[ "renderWindow" ].GetValue< Ogre::RenderWindow* >( );
		m_gui->initialise( renderWindow, "/data/interface/core/core.xml" );
		m_gui->hidePointer( );

		Management::GetInstance( )->GetEventManager( )->AddEventListener( INPUT_MOUSE_PRESSED, this, &UXSystemScene::OnMousePressed );
		Management::GetInstance( )->GetEventManager( )->AddEventListener( INPUT_MOUSE_MOVED, this, &UXSystemScene::OnMouseMoved );
		Management::GetInstance( )->GetEventManager( )->AddEventListener( INPUT_MOUSE_RELEASED, this, &UXSystemScene::OnMouseReleased );
		Management::GetInstance( )->GetEventManager( )->AddEventListener( INPUT_KEY_DOWN, this, &UXSystemScene::OnKeyDown );
		Management::GetInstance( )->GetEventManager( )->AddEventListener( INPUT_KEY_UP, this, &UXSystemScene::OnKeyUp );

		std::string scriptPath = "/data/interface/interface.lua";

		UXSystemComponent* component = new UXSystemComponent( scriptPath, this );
		component->SetAttribute( System::Attributes::Name, scriptPath );
		m_components.push_back( component );

		AnyValue::AnyValueMap scriptParameters;
		scriptParameters[ "scriptPath" ] = scriptPath;
		scriptParameters[ "name" ] = scriptPath;

		IService* scriptService = Management::GetInstance( )->GetServiceManager( )->FindService( System::Types::SCRIPT );
		lua_State* state = scriptService->Execute( "loadScript", scriptParameters )[ "state" ].GetValue< lua_State* >( );

		module( state )
		[
			class_< UXSystemComponent >( "UXSystemComponent" )
				.def( "findWidget", &UXSystemComponent::FindWidget )
				.def( "loadComponent", &UXSystemComponent::LoadComponent )
				.def( "getScreenWidth", &UXSystemComponent::GetScreenWidth )
				.def( "getScreenHeight", &UXSystemComponent::GetScreenHeight )
				.def( "getSupportedResolutions", &UXSystemComponent::GetSupportedResolutions, copy_table( result ) )
				.def( "scriptWidget", &UXSystemComponent::ScriptWidget )
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

			class_< IntCoord >( "IntCoord" )
				.def_readonly( "x" , &IntCoord::left )
				.def_readonly( "y" , &IntCoord::top )
				.def_readonly( "width" , &IntCoord::width )
				.def_readonly( "height" , &IntCoord::height ),

			class_< Any >( "Any" )
				.def( constructor<>( ) )
		];

		
		luabind::globals( state )[ "ux" ] = component;
		lua_resume( state, 0 );

		/*IService* soundService = Management::GetInstance( )->GetServiceManager( )->FindService( System::Types::SOUND );
		AnyValue::AnyValueMap soundParameters;
		soundParameters[ "filePath" ] = "/data/sound/music/ux.fev";
		soundService->Execute( "load", soundParameters );*/

		WidgetManager::getInstancePtr( )->registerUnlinker( this );
	}

	void UXSystemScene::Destroy()
	{
		IService* scriptService = Management::GetInstance( )->GetServiceManager( )->FindService( System::Types::SCRIPT );

		for ( IUXSystemComponent::UXSystemComponentList::iterator i = m_components.begin( ); i != m_components.end( ); ++i )
		{
			AnyValue::AnyValueMap parameters;
			parameters[ "name" ] = ( *i )->GetAttributes( )[ System::Attributes::Name ].GetValue< std::string >( );
			scriptService->Execute( "unloadComponent", parameters );
			delete ( *i );
		}

		Management::GetInstance( )->GetEventManager( )->RemoveEventListener( INPUT_MOUSE_PRESSED, this, &UXSystemScene::OnMousePressed );
		Management::GetInstance( )->GetEventManager( )->RemoveEventListener( INPUT_MOUSE_MOVED, this, &UXSystemScene::OnMouseMoved );
		Management::GetInstance( )->GetEventManager( )->RemoveEventListener( INPUT_MOUSE_RELEASED, this, &UXSystemScene::OnMouseReleased );
		Management::GetInstance( )->GetEventManager( )->RemoveEventListener( INPUT_KEY_DOWN, this, &UXSystemScene::OnKeyDown );
		Management::GetInstance( )->GetEventManager( )->RemoveEventListener( INPUT_KEY_UP, this, &UXSystemScene::OnKeyUp );

		m_gui->shutdown( );
	}

	ISystemComponent* UXSystemScene::CreateComponent( const std::string& name, const std::string& type )
	{
		std::stringstream layoutPath;
		layoutPath << "/data/interface/components/" << name << ".layout";
		m_gui->load( layoutPath.str( ) );

		std::stringstream scriptPath;
		scriptPath << "/data/interface/components/" << name << ".lua";

		AnyValue::AnyValueMap parameters;
		parameters[ "scriptPath" ] = scriptPath.str( );
		parameters[ "name" ] = name;

		IService* scriptService = Management::GetInstance( )->GetServiceManager( )->FindService( System::Types::SCRIPT );
		lua_State* state = scriptService->Execute( "loadScript", parameters )[ "state" ].GetValue< lua_State* >( );

		UXSystemComponent* component = new UXSystemComponent( name, this );

		luabind::globals( state )[ "ux" ] = component;
		lua_resume( state, 0 );

		m_components.push_back( component );

		m_gui->windowResized( m_gui->getRenderWindow( ) );

		component->SetAttribute( System::Attributes::Name, name );
		component->SetAttribute( System::Attributes::Type, System::Types::UX );
		component->SetAttribute( System::Attributes::Parent, this );

		return component;
	}

	void UXSystemScene::OnMouseMoved( const IEvent* event )
	{
		MouseEventData* eventData = static_cast< MouseEventData* >( event->GetEventData( ) );
		m_gui->injectMouseMove( eventData->GetMouseState( ).X.abs, eventData->GetMouseState( ).Y.abs, eventData->GetMouseState( ).Z.abs );
	}

	void UXSystemScene::OnMousePressed( const IEvent* event )
	{
		MouseEventData* eventData = static_cast< MouseEventData* >( event->GetEventData( ) );
		m_gui->injectMousePress( eventData->GetMouseState( ).X.abs, eventData->GetMouseState( ).Y.abs, MouseButton::Enum( eventData->GetMouseButtonId( ) ) );
	}

	void UXSystemScene::OnMouseReleased( const IEvent* event )
	{
		MouseEventData* eventData = static_cast< MouseEventData* >( event->GetEventData( ) );
		m_gui->injectMouseRelease( eventData->GetMouseState( ).X.abs, eventData->GetMouseState( ).Y.abs, MouseButton::Enum( eventData->GetMouseButtonId( ) ) );
	}

	void UXSystemScene::OnKeyUp( const IEvent* event )
	{
		KeyEventData* eventData = static_cast< KeyEventData* >( event->GetEventData( ) );
		m_gui->injectKeyRelease( KeyCode::Enum( eventData->GetKeyCode( ) ) );
	}

	void UXSystemScene::OnKeyDown( const IEvent* event )
	{
		KeyEventData* eventData = static_cast< KeyEventData* >( event->GetEventData( ) );
		m_gui->injectKeyPress( KeyCode::Enum( eventData->GetKeyCode( ) ) );
	}

	void UXSystemScene::_unlinkWidget( WidgetPtr widget )
	{
		void* userData = widget->getUserData( );
		UXSystemComponent::WidgetUserData* widgetUserData = static_cast< UXSystemComponent::WidgetUserData* >( userData );

		if ( 0 != widgetUserData )
		{
			for ( UXSystemComponent::WidgetUserData::iterator i = widgetUserData->begin( ); i != widgetUserData->end( ); ++i )
			{
				delete ( *i ).second;
			}

			delete widgetUserData;
		}
	}

	void UXSystemScene::Update( const float& deltaMilliseconds )
	{
		m_gui->injectFrameEntered( deltaMilliseconds );
	}
}