#ifndef INPUTMESSAGEBINDING_HPP
#define INPUTMESSAGEBINDING_HPP

#include "InputMessageBindingType.hpp"

#include <ois/OIS.h>

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



		/*! Default Constructor
		*
		* @param[in] const std::string & message
		* @param[in] const std::string & fullCode
		* @return (  )
		*/
		InputMessageBinding( const std::string& message, const std::string& fullCode )
		{
			m_message = message;
			m_fullCode = fullCode;

			std::string keyIdentifier = "k_";

			if( fullCode.find( keyIdentifier ) != std::string::npos )
			{
				std::string codeText = fullCode.substr( fullCode.find( keyIdentifier ) + keyIdentifier.size( ) );
				std::stringstream codeStream( codeText );

				int code;
				codeStream >> m_code;

				m_type = BINDING_KEYBOARD;
			}

			std::string mouseIdentifier = "m_";

			if ( fullCode.find( mouseIdentifier ) != std::string::npos )
			{
				std::string codeText = fullCode.substr( fullCode.find( mouseIdentifier ) + mouseIdentifier.size( ) );
				
				std::stringstream codeStream( codeText );
				codeStream >> m_code;

				m_type = BINDING_MOUSE;

				switch( static_cast< OIS::MouseButtonID >( m_code ) )
				{

				case OIS::MB_Left: m_text = "MOUSE1"; break;
				case OIS::MB_Right: m_text = "MOUSE2";	break;
				case OIS::MB_Middle: m_text = "MOUSE3"; break;
				case OIS::MB_Button3: m_text = "MOUSE3"; break;
				case OIS::MB_Button4: m_text = "MOUSE4"; break;
				case OIS::MB_Button5: m_text = "MOUSE5"; break;
				case OIS::MB_Button6: m_text = "MOUSE6"; break;
				case OIS::MB_Button7: m_text = "MOUSE7"; break;

				}
			}
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
		InputMessageBindingType GetType( ) const  { return m_type; };


		/*! Gets the Input Code
		*
		* @return ( unsigned int )
		*/
		unsigned int GetCode( ) const { return m_code; };


		/*! Returns the Full Code used to create the binding
		*
		* @return ( std::string )
		*/
		std::string GetFullCode( ) const { return m_fullCode; };


		/*! Returns the text of the Input
		*
		* @return ( std::string )
		*/
		std::string GetText( ) const { return m_text; };


		/*! Sets the text representation of the binding
		*
		* @param[in] const std::string & text
		* @return ( void )
		*/
		void SetText( const std::string& text ) { m_text = text; };


		/*! Gets the Message Associated with the Binding
		*
		* @return ( unsigned int )
		*/
		System::Message GetMessage( ) const { return m_message; };

	private:

		InputMessageBindingType m_type;
		unsigned int m_code;
		std::string m_text;
		std::string m_fullCode;
		System::Message m_message;

	};
};

#endif