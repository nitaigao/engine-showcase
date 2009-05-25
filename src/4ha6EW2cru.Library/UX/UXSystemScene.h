/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   UXSystemScene.h
*  @date   2009/04/26
*/
#ifndef UXSYSTEMSCENE_H
#define UXSYSTEMSCENE_H

#include "IUXSystemScene.hpp"
#include "IUXSystemComponent.hpp"

#include "../Events/IEvent.hpp"

#include <luabind/luabind.hpp>

namespace UX
{
	/*! 
	 *  A scene specific to the UX System
	 */
	class UXSystemScene : public IUXSystemScene, public MyGUI::IUnlinkWidget
	{

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		~UXSystemScene( );


		/*! Default Constructor
		*
		*  @return ()
		*/
		UXSystemScene( )
			: m_gui( new MyGUI::Gui( ) )
		{

		}


		/*!  Initializes the System Scene
		*
		*  @return (void)
		*/
		void Initialize( );


		/*! Steps internal data of the SystemScene
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		void Update( const float& deltaMilliseconds );


		/*! Destroys the System Scene
		*
		*  @return (void)
		*/
		inline void Destroy( );


		/*! Gets the System::Types::Type of the SystemScene
		*
		*  @return (System::Types::Type)
		*/
		inline System::Types::Type GetType( ) const { return System::Types::UX; };


		/*! Creates a SystemComponent specific to the SystemScene
		*
		*  @param[in] const std::string & name
		*  @param[in] const std::string & type
		*  @return (ISystemComponent*)
		*/
		ISystemComponent* CreateComponent( const std::string& name, const std::string& type );


		/*! Destroys a SystemComponent created by the SystemScene
		*
		*  @param[in] ISystemComponent * component
		*  @return (void)
		*/
		void DestroyComponent( ISystemComponent* component ) { };


		inline MyGUI::Gui* GetGui( ) const { return m_gui; };

		static inline void SetFocus( MyGUI::WidgetPtr widget, bool focus )
		{
			if ( focus )
			{
				MyGUI::InputManager::getInstancePtr( )->setKeyFocusWidget( widget );
			}
			else
			{
				MyGUI::InputManager::getInstancePtr( )->resetKeyFocusWidget( widget );
			}
		}

		static inline void SetText( MyGUI::WidgetPtr widget, const std::string& text ) { widget->setCaption( text ); };
		static inline std::string GetText( MyGUI::WidgetPtr widget ) { return widget->getCaption( ).asUTF8( ); };
		static inline void AddText( MyGUI::EditPtr editWidget, const std::string& text ) { editWidget->addText( text ); };
		static inline void MultiList_AddColumn( MyGUI::MultiListPtr widget, const std::string& name, int width ) { widget->addColumn( name, width ); };
		static inline void MultiList_AddItem( MyGUI::MultiListPtr widget, const std::string& name ) { widget->addItem( name ); };
		static inline std::string MultiList_GetItemName( MyGUI::MultiListPtr widget, const int& index ) { return widget->getItemNameAt( index ); };
		static inline std::string MultiList_GetSubItemName( MyGUI::MultiListPtr widget, const int& column, const int& index ) { return widget->getSubItemNameAt( column, index ); };
		static inline void MultiList_SetSubItemName( MyGUI::MultiListPtr widget, int column, int index, const std::string& name ) { widget->setSubItemNameAt( column,index, name ); };

		static inline MyGUI::ButtonPtr AsButton( MyGUI::WidgetPtr widget ) { return static_cast< MyGUI::ButtonPtr >( widget ); };
		static inline MyGUI::ComboBoxPtr AsComboBox( MyGUI::WidgetPtr widget ) { return static_cast< MyGUI::ComboBoxPtr >( widget ); };
		static inline MyGUI::EditPtr AsEditBox( MyGUI::WidgetPtr widget ) { return static_cast< MyGUI::EditPtr >( widget ); };
		static inline MyGUI::ProgressPtr AsProgressBar( MyGUI::WidgetPtr widget ) { return static_cast< MyGUI::ProgressPtr >( widget ); };
		static inline MyGUI::MultiListPtr AsMultiList( MyGUI::WidgetPtr widget ) { return static_cast< MyGUI::MultiListPtr >( widget ); };
		static inline MyGUI::VScrollPtr AsScrollBar( MyGUI::WidgetPtr widget ) { return static_cast< MyGUI::VScrollPtr> ( widget ); };

		void OnMouseMoved( const Events::IEvent* event );
		void OnMousePressed( const Events::IEvent* event );
		void OnMouseReleased( const Events::IEvent* event );
		void OnKeyUp( const Events::IEvent* event );
		void OnKeyDown( const Events::IEvent* event );

		void _unlinkWidget( MyGUI::WidgetPtr widget );

	private:

		MyGUI::Gui* m_gui;
		IUXSystemComponent::UXSystemComponentList m_components;
	};
};

#endif