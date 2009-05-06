/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   UXSystemComponent.h
*  @date   2009/04/26
*/

#include <vector>

#include <MyGUI.h>
#include <luabind/luabind.hpp>

#include "../Maths/MathVector3.hpp"
#include "../Maths/MathQuaternion.hpp"

#include "IUXSystemScene.hpp"
#include "IUXSystemComponent.hpp"

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
		*  @param[in] IScriptComponent * scriptComponent
		*  @return ()
		*/
		UXSystemComponent( const std::string& name, IUXSystemScene* uxSystemScene )
			: _name( name )
			, _scene( uxSystemScene )
		{

		}

		/* Inherited from ISystemComponent */

		/*! Initializes the Component
		*
		*  @param[in] AnyValue::AnyValueMap properties
		*  @return (void)
		*/
		void Initialize( AnyValue::AnyValueMap& properties ) { };


		/*! Steps the internal data of the Component
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		void Update( const float& deltaMilliseconds ) { };


		/*! Destroys the Component
		*
		*  @return (void)
		*/
		void Destroy( ) { };


		/*! Adds an Observer to the Component
		*
		*  @param[in] IObserver * observer
		*  @return (void)
		*/
		void AddObserver( IObserver* observer ) { };


		/*! Observes a change in the Subject
		*
		*  @param[in] ISubject * subject
		*  @param[in] const unsigned int& systemChanges
		*  @return (void)
		*/
		void Observe( ISubject* subject, const unsigned int& systemChanges ) { };


		/*! Pushes any Changes to the Observers
		*
		*  @param[in] const unsigned int& systemChanges
		*  @return (void)
		*/
		void PushChanges( const unsigned int& systemChanges ) { };


		/*! Gets the Name of the Component
		*
		*  @return (const std::string&)
		*/
		inline const std::string& GetName( ) const { return _name; };


		/*! Gets the System::Types::Type of the Component
		*
		*  @return (System::Types::Type)
		*/
		inline System::Types::Type GetType( ) const { return System::Types::AI; };


		/*! Gets the types of Changes this component is interested in
		*
		*  @return (unsigned int)
		*/
		inline unsigned int GetRequestedChanges( ) const { return System::Changes::None; };


		/*! Gets the properties of the Component
		*
		*  @return (AnyValueMap)
		*/
		inline AnyValue::AnyValueMap GetProperties( ) const { return AnyValue::AnyValueMap( ); };


		/*! Sets the Properties of the Component
		*
		*  @param[in] AnyValue::AnyValueMap systemProperties
		*  @return (void)
		*/
		inline void SetProperties( AnyValue::AnyValueMap& properties ) { };


		/*! Gets the Position of the Component
		*
		*  @return (MathVector3)
		*/
		inline Maths::MathVector3 GetPosition( ) const { return _position; };


		/*! Gets the Scale of the Component
		*
		*  @return (MathVector3)
		*/
		inline Maths::MathVector3 GetScale( ) const { return Maths::MathVector3::Zero( ); };


		/*! Gets the Orientation of the Component
		*
		*  @return (MathQuaternion)
		*/
		inline Maths::MathQuaternion GetOrientation( ) const { return _orientation; };


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
		MyGUI::WidgetPtr FindWidget( const std::string widgetName ) { return _scene->GetGui( )->findWidgetT( widgetName ); };

		
		/*! Attaches an LUA function to a Widget Event
		 *
		 *  @param[in] MyGUI::Widget * widget
		 *  @param[in] const std::string eventName
		 *  @param[in] luabind::object function
		 *  @return (void)
		 */
		void ScriptWidget( MyGUI::Widget* widget, const std::string eventName, luabind::object function );

		
		/*! Shows the mouse
		 *
		 *  @return (void)
		 */
		inline void ShowMouse( ) { _scene->GetGui( )->showPointer( ); };

		
		/*! Hides the mouse
		 *
		 *  @return (void)
		 */
		inline void HideMouse( ) { _scene->GetGui( )->hidePointer( ); };

		
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
		int GetScreenWidth( ) { return _scene->GetGui( )->getViewWidth( ); };

		
		/*! Returns the Screen Height in pixels
		 *
		 *  @return (int)
		 */
		int GetScreenHeight( ) { return _scene->GetGui( )->getViewHeight( ); };

		
		/*! Returns the Average FPS from the renderer
		 *
		 *  @return (int)
		 */
		inline int GetFps( ) { return static_cast< int >( _scene->GetGui( )->getRenderWindow( )->getAverageFPS( ) ); };


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

		
		/*! Forwards Key Up Events to the subscribing Widgets in Script
		 *
		 *  @param[in] MyGUI::WidgetPtr widget
		 *  @param[in] MyGUI::KeyCode key
		 *  @return (void)
		 */
		static void OnKeyUp( MyGUI::WidgetPtr widget, MyGUI::KeyCode key );

		
		/*! Returns a list of supported Video Resolutions
		 *
		 *  @return (std::vector< std::string >)
		 */
		std::vector< std::string > GetSupportedResolutions( );

	private:

		std::string _name;
		IUXSystemScene* _scene;

		Maths::MathVector3 _position;
		Maths::MathVector3 _scale;
		Maths::MathQuaternion _orientation;

	};
}