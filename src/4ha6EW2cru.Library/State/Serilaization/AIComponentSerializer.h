/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   AIComponentSerializer.h
*  @date   2009/04/27
*/
#ifndef AICOMPONENTSERIALIZER_H
#define AICOMPONENTSERIALIZER_H

#include "IComponentSerializer.hpp"

namespace Serialization
{
	/*!
	 *  AI Component Serializer 
	 */
	class AIComponentSerializer : public IComponentSerializer
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~AIComponentSerializer( ) { };


		/*! Default Constructor
		 *
		 *  @return ()
		 */
		AIComponentSerializer( ) { };


		/*! DeSerializes the Component from the Supplied Element
		*
		* @param[in] const std::string entityName
		* @param[in] const ticpp::Element & componentElement
		* @param[in] const ISystemScene::SystemSceneMap & systemScenes
		* @return ( ISystemComponent* )
		*/
		ISystemComponent* DeSerialize( const std::string entityName, ticpp::Element* componentElement, const ISystemScene::SystemSceneMap& systemScenes );

	private:

		AIComponentSerializer( const AIComponentSerializer & copy ) { };
		AIComponentSerializer & operator = ( const AIComponentSerializer & copy ) { return *this; };

	};
};

#endif