/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   InputSystemScene.h
*  @date   2009/04/26
*/
#ifndef INPUTSYSTEMSCENE_H
#define INPUTSYSTEMSCENE_H

#include <vector>

#include "IInputSystemComponent.hpp"

#include "../System/ISystem.hpp"
#include "../System/ISystemScene.hpp"

#include <OIS/OISInputManager.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>

namespace Input
{
	/*! 
	*  An Input Specific Scene
	*/
	class InputSystemScene : public ISystemScene
	{
		typedef std::vector< IInputSystemComponent* > InputSystemComponentList;

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
		InputSystemScene( OIS::Mouse* mouse, OIS::Keyboard* keyboard )
			: m_inputAllowed( false )
			, m_mouse( mouse )
			, m_keyboard( keyboard )
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

	private:

		bool m_inputAllowed;

		InputSystemComponentList m_inputComponents;

		OIS::Mouse* m_mouse;
		OIS::Keyboard* m_keyboard;

		InputSystemScene( ) { };
		InputSystemScene( const InputSystemScene & copy ) { };
		InputSystemScene & operator = ( const InputSystemScene & copy ) { return *this; };

	};
};

#endif