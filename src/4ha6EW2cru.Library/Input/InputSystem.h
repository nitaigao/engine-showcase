/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   InputSystem.h
*  @date   2009/04/26
*/
#ifndef __INPUTMANAGER_H
#define __INPUTMANAGER_H

#include <vector>

#include "InputSystemScene.h"
#include "IInputSystem.hpp"

#include "../Service/IService.hpp"
#include "../Configuration/IConfiguration.hpp"

#include <OIS/OISInputManager.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>


namespace Input
{
	/*! 
	 *  Receives all Input from the User(s) and translates it into Game Events
	 */
	class InputSystem : public IInputSystem, public IService, public OIS::KeyListener, public OIS::MouseListener
	{
	
		typedef std::vector< InputSystemScene* > InputSystemSceneList;
	
	public:
	
		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~InputSystem( );
	
	
		/*! Default Constructor
		 *
		 *  @param[in] Configuration::IConfiguration * configuration
		 *  @return ()
		 */
		InputSystem( Configuration::IConfiguration* configuration )
			: _inputAllowed( true )
			, _configuration( configuration )
			, _inputManager( 0 )
			, _keyboard( 0 )
			, _mouse( 0 )
		{
	
		}
	
	
		/*! Initializes the System
		*
		*  @return (void)
		*/
		void Initialize( );
	
	
		/*! Performs an Input capture and steps internal data
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		inline void Update( const float& deltaMilliseconds );
	
	
		/*! Releases internal data of the System
		*
		*  @return (System::Types::Type)
		*/
		void Release( ) { };
	
	
		/*! Returns the type of the System
		*
		*  @return (System::Types::Type)
		*/
		System::Types::Type GetType( ) const { return System::Types::INPUT; };
	
	
		/*! Creates a System Scene
		*
		*  @return (ISystemScene*)
		*/
		ISystemScene* CreateScene( );
	
	
		/*! Gets the System's Properties
		*
		*  @return (AnyValueMap)
		*/
		inline AnyValue::AnyValueMap GetProperties( ) const { return _properties; };
	
	
		/*! Sets a System Property
		*
		*  @param[in] const std::string & name
		*  @param[in] AnyValue value
		*  @return (void)
		*/
		void SetProperty( const std::string& name, AnyValue value );
	

		/*! Creates a SystemComponent specific to the SystemScene
		*
		*  @param[in] const std::string & name
		*  @param[in] const std::string & type
		*  @return (ISystemComponent*)
		*/
		ISystemComponent* CreateComponent( const std::string& name, System::Types::Type systemType ) { return 0; };
	
		
		/*! Callback fired when the user presses a button on the keyboard
		 *
		 *  @param[in] const OIS::KeyEvent & arg
		 *  @return (bool)
		 */
		bool keyPressed( const OIS::KeyEvent &arg );
	
		
		/*! Callback fired when the user releases a button on the keyboard
		 *
		 *  @param[in] const OIS::KeyEvent & arg
		 *  @return (bool)
		 */
		bool keyReleased( const OIS::KeyEvent &arg );
	
		
		/*! Callback fired when the user moves the mouse
		 *
		 *  @param[in] const OIS::MouseEvent & arg
		 *  @return (bool)
		 */
		bool mouseMoved( const OIS::MouseEvent &arg );
	
	
		/*! Callback fired when the user presses a button on the mouse
		 *
		 *  @param[in] const OIS::MouseEvent & arg
		 *  @param[in] OIS::MouseButtonID id
		 *  @return (bool)
		 */
		bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	
		
		/*! Callback fired when the user releases a button on the mouse 
		 *
		 *  @param[in] const OIS::MouseEvent & arg
		 *  @param[in] OIS::MouseButtonID id
		 *  @return (bool)
		 */
		bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	
	
		/*! Service interface for Input Tasks
		 *
		 *  @param[in] const std::string & actionName
		 *  @param[in] AnyValue::AnyValueMap & parameters
		 *  @return (AnyValue::AnyValueMap)
		 */
		AnyValue::AnyValueMap Execute( const std::string& actionName, AnyValue::AnyValueMap& parameters );
	
	private:
	
		bool _inputAllowed;
	
		AnyValue::AnyValueMap _properties;
		Configuration::IConfiguration* _configuration;
		InputSystemSceneList _inputScenes;
	
		OIS::InputManager* _inputManager;
		OIS::Mouse* _mouse;
		OIS::Keyboard* _keyboard;
	
		InputSystem( ) { };
		InputSystem( const InputSystem & copy ) { };
		InputSystem & operator = ( const InputSystem & copy ) { return *this; };
	
	};
};

#endif
