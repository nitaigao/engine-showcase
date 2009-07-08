/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   UXSystemComponent.h
*  @date   2009/04/26
*/
#ifndef UXSYSTEMCOMPONENT_H
#define UXSYSTEMCOMPONENT_H

#include <vector>

#include <MyGUI.h>
#include <luabind/luabind.hpp>

#include "Maths/MathVector3.hpp"
#include "Maths/MathQuaternion.hpp"

#include "IUXSystemComponent.hpp"
#include "IUXSystemScene.hpp"

namespace UX
{
	/*! 
	*  A UX System Scene Component
	*/
	class UXSystemComponent : public IUXSystemComponent
	{

	public:

		typedef std::map< std::string, luabind::object* > WidgetUserData;

		/*! Default Destructor
		*
		*  @return ()
		*/
		~UXSystemComponent( ) { };


		/*! Default Constructor
		*
		*  @param[in] const std::string & name
		*  @param[in] IUXSystemScene * scene
		*  @return ()
		*/
		UXSystemComponent( const std::string& name, IUXSystemScene* scene )
			: m_name( name )
			, m_scene( scene )
		{

		}

		/*! Posts a message to observers
		*
		*  @param[in] const std::string & message
		*  @param[in] AnyType::AnyValueMap parameters
		*  @return (AnyType)
		*/
		inline AnyType PushMessage( const System::Message& message, AnyType::AnyTypeMap parameters ) { return AnyType( ); };


		/*! Messages the Component to influence its internal state
		*
		*  @param[in] const std::string & message
		*  @return (AnyType)
		*/
		inline AnyType Message( const System::Message& message, AnyType::AnyTypeMap parameters ) { return AnyType( ); };

		/* Inherited from ISystemComponent */

		/*! Initializes the Component
		*
		*  @param[in] AnyType::AnyValueMap properties
		*  @return (void)
		*/
		inline void Initialize( ) { };


		/*! Steps the internal data of the Component
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		inline void Update( const float& deltaMilliseconds ) { };


		/*! Destroys the Component
		*
		*  @return (void)
		*/
		inline void Destroy( ) { };


		/*! Adds an Observer to the Component
		*
		*  @param[in] IObserver * observer
		*  @return (void)
		*/
		inline void AddObserver( IObserver* observer ) { };


		/*! Gets the properties of the Component
		*
		*  @return (AnyTypeKeyMap)
		*/
		AnyType::AnyTypeMap GetAttributes( ) const { return m_attributes; };


		/*! Sets an Attribute on the Component *
		*
		*  @param[in] const unsigned int attributeId
		*  @param[in] const AnyType & value
		*/
		inline void SetAttribute( const System::Attribute& attributeId, const AnyType& value ) { m_attributes[ attributeId ] = value; };


		/*! -- Script Helpers -- */
		
		/*! Loads a UI Component for Rendering
		 *
		 *  @param[in] const std::string componentName
		 *  @return (void)
		 */
		void LoadComponent( const std::string componentName );

		
		/*! Retrieves a widget from the UI
		 *
		 *  @param[in] const std::string widgetName
		 *  @return (MyGUI::WidgetPtr)
		 */
		MyGUI::WidgetPtr FindWidget( const std::string widgetName ) { return m_scene->GetGui( )->findWidgetT( widgetName ); };

		
		/*! Attaches an LUA function to a Widget Event
		 *
		 *  @param[in] MyGUI::Widget * widget
		 *  @param[in] const std::string eventName
		 *  @param[in] luabind::object function
		 *  @return (void)
		 */
		void ScriptWidget( MyGUI::Widget* widget, const std::string eventName, luabind::object function );


		/*! Removes an LUA function from a Widget Event
		*
		*  @param[in] MyGUI::Widget * widget
		*  @param[in] const std::string eventName
		*  @param[in] luabind::object function
		*  @return (void)
		*/
		void UnScriptWidget( MyGUI::Widget* widget, const std::string& eventName, luabind::object function );

		
		/*! Shows the mouse
		 *
		 *  @return (void)
		 */
		inline void ShowMouse( ) { m_scene->GetGui( )->showPointer( ); };

		
		/*! Hides the mouse
		 *
		 *  @return (void)
		 */
		inline void HideMouse( ) { m_scene->GetGui( )->hidePointer( ); };

		
		/*! Sets whether or not the Player controls can manipulate the scene
		 *
		 *  @param[in] bool inputAllowed
		 *  @return (void)
		 */
		void SetInputAllowed( bool inputAllowed );

		
		/*! Causes the Renderer to adjust the Resolution to match the Configuration
		 *
		 *  @param[in] int width
		 *  @param[in] int height
		 *  @param[in] bool isFullScreen
		 *  @return (void)
		 */
		void ChangeResolution( int width, int height, bool isFullScreen );

		
		/*! Sets the Far Clip Distance of the Camera
		 *
		 *  @param[in] const float & farClip
		 *  @return (void)
		 */
		void SetFarClip( const float& farClip );

		
		/*!  Returns the Screen Width in pixels
		 *
		 *  @return (int)
		 */
		int GetScreenWidth( ) { return m_scene->GetGui( )->getViewWidth( ); };

		
		/*! Returns the Screen Height in pixels
		 *
		 *  @return (int)
		 */
		int GetScreenHeight( ) { return m_scene->GetGui( )->getViewHeight( ); };

		
		/*! Returns the Average FPS from the renderer
		 *
		 *  @return (int)
		 */
		inline int GetFps( ) { return static_cast< int >( m_scene->GetGui( )->getRenderWindow( )->getAverageFPS( ) ); };




		/*! -- Widget Event Handlers -- */

		/*! Forwards Mouse Button Released Events to the subscribing Widgets in Script
		 *
		 *  @param[in] MyGUI::WidgetPtr widget
		 *  @param[in] int left
		 *  @param[in] int top
		 *  @param[in] MyGUI::MouseButton id
		 *  @return (void)
		 */
		static void OnMouseReleased( MyGUI::WidgetPtr widget, int left, int top, MyGUI::MouseButton id );


		static void OnMousePressed( MyGUI::WidgetPtr widget, int left, int top, MyGUI::MouseButton id );

		
		/*! Forwards Key Up Events to the subscribing Widgets in Script
		 *
		 *  @param[in] MyGUI::WidgetPtr widget
		 *  @param[in] MyGUI::KeyCode key
		 *  @return (void)
		 */
		static void OnKeyUp( MyGUI::WidgetPtr widget, MyGUI::KeyCode key );


		static void OnListSelectAccept( MyGUI::MultiListPtr widget, size_t index );

		static void OnEventScrollChangePosition( MyGUI::VScrollPtr sender, size_t position );

		
		/*! Returns a list of supported Video Resolutions
		 *
		 *  @return (std::vector< std::string >)
		 */
		std::vector< std::string > GetSupportedResolutions( );

		//Input::InputMessageBinding GetMessageBinding( const std::string& message );

		void SetMessageBinding( const std::string& message, const std::string& binding );

		void SetInvertYAxis( const bool& invert );

	private:

		std::string m_name;
		AnyType::AnyTypeMap m_attributes;

		IUXSystemScene* m_scene;

	};
}

#endif