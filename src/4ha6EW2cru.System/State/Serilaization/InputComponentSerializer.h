/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   InputComponentSerializer.h
*  @date   2009/04/27
*/
#ifndef INPUTCOMPONENTSERIALIZER_H
#define INPUTCOMPONENTSERIALIZER_H

#include "IComponentSerializer.hpp"

namespace Serialization
{
	/*!
	 *  A Input Component Serializer 
	 */
	class InputComponentSerializer : public IComponentSerializer
	{

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		~InputComponentSerializer( ) { };


		/*! Default Constructor
		*
		*  @return ()
		*/
		InputComponentSerializer( ) { };


		/*! DeSerializes the Component from the Supplied Element
		*
		* @param[in] const std::string entityName
		* @param[in] const ticpp::Element & componentElement
		* @param[in] const ISystemScene::SystemSceneMap & systemScenes
		* @return ( ISystemComponent* )
		*/
		ISystemComponent* DeSerialize( const std::string entityName, ticpp::Element* componentElement, const ISystemScene::SystemSceneMap& systemScenes );

	private:

		InputComponentSerializer( const InputComponentSerializer & copy ) { };
		InputComponentSerializer & operator = ( const InputComponentSerializer & copy ) { return *this; };

	};
};

#endif