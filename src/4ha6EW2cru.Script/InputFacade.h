/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   InputFacade.h
*  @date   2009/07/14
*/
#pragma once
#ifndef INPUTFACADE_H
#define INPUTFACADE_H

#include "IInputFacade.hpp"

#include <luabind/luabind.hpp>

namespace Script
{
	/*! 
	 *  The Facade interface to communicate with the input system
	 */
	class InputFacade : public IInputFacade
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~InputFacade( ) { };


		/*! Default Constructor
		*
		* @return (  )
		*/
		InputFacade( ) { };


		/*! Registers the Script functions with the given state
		*
		* @return ( void )
		*/
		static luabind::scope  RegisterFunctions( );


		/*! Returns the key or mouse text associated with the given message
		*
		* @param[in] const std::string & message
		* @return ( std::string )
		*/
		std::string GetTextForMessage( const std::string& message );


		/*! Sets the key or mouse button to the given message
		*
		* @param[in] const std::string & message
		* @param[in] const std::string & binding
		* @return ( void )
		*/
		void SetMessageBinding( const std::string& message, const std::string& binding );


		/*! Sets a default binding for the given message which will be used if the binding isn't specified in the config file
		*
		* @param[in] const std::string & message
		* @param[in] const std::string & binding
		* @return ( void )
		*/
		void SetDefaultMessageBinding( const std::string& message, const std::string& binding );

	private:

		InputFacade( const InputFacade & copy ) { };
		InputFacade & operator = ( const InputFacade & copy ) { return *this; };
		
	};
};

#endif