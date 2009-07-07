/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   NetworkComponentSerializer.h
*  @date   2009/04/27
*/
#ifndef NETWORKCOMPONENTSERIALIZER_H
#define NETWORKCOMPONENTSERIALIZER_H

#include "IComponentSerializer.hpp"

namespace Serialization
{
	/*!
	*  AI Component Serializer 
	*/
	class NetworkComponentSerializer : public IComponentSerializer
	{

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		~NetworkComponentSerializer( ) { };


		/*! Default Constructor
		*
		*  @return ()
		*/
		NetworkComponentSerializer( ) { };


		/*! DeSerializes the Component from the Supplied Element
		*
		* @param[in] const std::string entityName
		* @param[in] const ticpp::Element & componentElement
		* @param[in] const ISystemScene::SystemSceneMap & systemScenes
		* @return ( ISystemComponent* )
		*/
		ISystemComponent* DeSerialize( const std::string entityName, ticpp::Element* componentElement, const ISystemScene::SystemSceneMap& systemScenes );

	private:

		NetworkComponentSerializer( const NetworkComponentSerializer & copy ) { };
		NetworkComponentSerializer & operator = ( const NetworkComponentSerializer & copy ) { return *this; };

	};
};

#endif