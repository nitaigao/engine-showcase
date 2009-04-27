/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   UXSystemScene.h
*  @date   2009/04/26
*/
#ifndef __UXSYSTEMSCENE_H
#define __UXSYSTEMSCENE_H

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

		typedef std::vector< IUXSystemComponent* > UXSystemComponentList;

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
			: _gui( new MyGUI::Gui( ) )
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
		void Update( const float& deltaMilliseconds ) { };

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

		/* UX System Specific */
		
		inline MyGUI::Gui* GetGui( ) const { return _gui; };

		/* -- Widget Script Helpers -- */

		/*! Sets the focus to the given Widget */
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

		/*! Sets a Widget Caption */
		static inline void SetText( MyGUI::WidgetPtr widget, const std::string& text ) { widget->setCaption( text ); };

		/*! Gets a WidgetCaption */
		static inline std::string GetText( MyGUI::WidgetPtr widget ) { return widget->getCaption( ).asUTF8( ); };

		static inline void AddText( MyGUI::EditPtr editWidget, const std::string& text ) { editWidget->addText( text ); };

		/*! Casts a widget to a Button */
		static inline MyGUI::ButtonPtr AsButton( MyGUI::WidgetPtr widget ) { return static_cast< MyGUI::ButtonPtr >( widget ); };

		/*! Casts a widget to a ComboBox */
		static inline MyGUI::ComboBoxPtr AsComboBox( MyGUI::WidgetPtr widget ) { return static_cast< MyGUI::ComboBoxPtr >( widget ); };

		/*! Casts a widget to an EditBox */
		static inline MyGUI::EditPtr AsEditBox( MyGUI::WidgetPtr widget ) { return static_cast< MyGUI::EditPtr >( widget ); };

		/*! Casts a widget to a ProgressBar */
		static inline MyGUI::ProgressPtr AsProgressBar( MyGUI::WidgetPtr widget ) { return static_cast< MyGUI::ProgressPtr >( widget ); };

		/* -- Internal Event Handlers --*/

		/*! Handler to intercept OnMouseMoved Events */
		void OnMouseMoved( const Events::IEvent* event );

		/*! Handler to intercept OnMouseMoved Events */
		void OnMousePressed( const Events::IEvent* event );

		/*! Handler to intercept OnMouseMoved Events */
		void OnMouseReleased( const Events::IEvent* event );

		/*! Handler to intercept OnKeyUp Events */
		void OnKeyUp( const Events::IEvent* event );

		/*! Handler to intercept OnKeyDown Events */
		void OnKeyDown( const Events::IEvent* event );

	private:

		void _unlinkWidget( MyGUI::WidgetPtr widget );

		MyGUI::Gui* _gui;
		UXSystemComponentList _components;
	};
};

#endif