#ifndef INPUTMESSAGEBINDING_HPP
#define INPUTMESSAGEBINDING_HPP

#include "InputMessageBindingType.hpp"

#include <vector>

namespace Input
{
	class InputMessageBinding
	{

	public:

		typedef std::deque< InputMessageBinding > InputMessageBindingList;

		/*! Default Destructor
		*
		* @return (  )
		*/
		~InputMessageBinding( ) { };
		
		InputMessageBinding( const InputMessageBindingType& type, const unsigned int& code, const std::string& text, const System::Message& message )
			: m_type( type )
			, m_code( code )
			, m_text( text )
			, m_message( message ) 
		{

		}


		/*! Returns a string name of the type
		*
		* @return ( const std::string )
		*/
		static const std::string TypeName( ) { return "InputMessageBinding"; };


		/*! Returns the Type of the Message Binding
		*
		* @return ( InputMessageBindingType )
		*/
		InputMessageBindingType GetType( ) { return m_type; };


		/*! Gets the Input Code
		*
		* @return ( unsigned int )
		*/
		unsigned int GetCode( ) { return m_code; };


		/*! Returns the text of the Input
		*
		* @return ( std::string )
		*/
		std::string GetText( ) { return m_text; };


		/*! Gets the Message Associated with the Binding
		*
		* @return ( unsigned int )
		*/
		System::Message GetMessage( ) { return m_message; };

	private:

		InputMessageBindingType m_type;
		unsigned int m_code;
		std::string m_text;
		System::Message m_message;

	};
};

#endif