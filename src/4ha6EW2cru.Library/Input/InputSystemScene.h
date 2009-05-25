/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   InputSystemScene.h
*  @date   2009/04/26
*/
#ifndef INPUTSYSTEMSCENE_H
#define INPUTSYSTEMSCENE_H

#include <vector>

#include "IInputSystemScene.hpp"
#include "IInputSystemComponent.hpp"

#include "../System/ISystem.hpp"

#include <OIS/OISInputManager.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>

#include "../Configuration/IConfiguration.hpp"

namespace Input
{
	/*! 
	*  An Input Specific Scene
	*/
	class InputSystemScene : public IInputSystemScene
	{
	
	public:


		/*! Default Destructor
		*
		*  @return ()
		*/
		~InputSystemScene( ) { };


		/*! Default Constructor
		*
		*  @param[in] OIS::Mouse * mouse
		*  @param[in] OIS::Keyboard * keyboard
		*  @return ()
		*/
		explicit InputSystemScene( Configuration::IConfiguration* configuration, IInputSystem* system )
			: m_configuration( configuration ) 
			, m_system( system )
			, m_inputAllowed( false )
		{

		}


		/*! Initializes the System Scene
		*
		*  @return (void)
		*/
		void Initialize( ) { };


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
		inline void Destroy( ) { };


		/*! Gets the System::Types::Type of the SystemScene
		*
		*  @return (System::Types::Type)
		*/
		inline System::Types::Type GetType( ) const { return System::Types::INPUT; }


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
		void DestroyComponent( ISystemComponent* component );


		/*! Sets whether the game will receive input from the player, the interface is unaffected
		*
		*  @param[in] const bool & inputAllowed
		*  @return (void)
		*/
		void SetInputAllowed( const bool& inputAllowed ) { m_inputAllowed = inputAllowed; };


		/*! Called by the System when a player presses a key on the keyboard
		*
		*  @param[in] const OIS::KeyEvent & arg
		*  @return (bool)
		*/
		bool KeyPressed( const OIS::KeyEvent &arg );


		/*! Called by the System when a player releases a key on the keyboard
		*
		*  @param[in] const OIS::KeyEvent & arg
		*  @return (bool)
		*/
		bool KeyReleased( const OIS::KeyEvent &arg );


		/*! Called by the System when a player moves the mouse
		*
		*  @param[in] const OIS::MouseEvent & arg
		*  @return (bool)
		*/
		bool MouseMoved( const OIS::MouseEvent &arg );


		/*! Called by the System when a player presses a mouse button
		*
		*  @param[in] const OIS::MouseEvent & arg
		*  @param[in] OIS::MouseButtonID id
		*  @return (bool)
		*/
		bool MousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );


		/*! Called by the System when a player releases a mouse button
		*
		*  @param[in] const OIS::MouseEvent & arg
		*  @param[in] OIS::MouseButtonID id
		*  @return (bool)
		*/
		bool MouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );


		/*! Returns the System controlling the Scene
		*
		* @return ( IInputSystem* )
		*/
		IInputSystem* GetSystem( ) { return m_system; };

	private:

		bool m_inputAllowed;

		IInputSystemComponent::InputSystemComponentList m_inputComponents;

		IInputSystem* m_system;
		Configuration::IConfiguration* m_configuration;

		InputSystemScene( ) { };
		InputSystemScene( const InputSystemScene & copy ) { };
		InputSystemScene & operator = ( const InputSystemScene & copy ) { return *this; };

	};
};

#endif