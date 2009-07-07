/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IComponentSerializer.hpp
*  @date   2009/04/27
*/
#ifndef ICOMPONENTSERIALIZER_HPP
#define ICOMPONENTSERIALIZER_HPP

#include <string>
#include "../../Utility/tinyxml/ticpp.h"

#include "../../System/ISystemComponent.hpp"
#include "../../System/ISystemScene.hpp"

namespace Serialization
{
	/*!
	 *  A Component Serializer 
	 */
	class IComponentSerializer
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IComponentSerializer( ) { };


		/*! DeSerializes the Component from the Supplied Element
		*
		* @param[in] const std::string entityName
		* @param[in] const ticpp::Element & componentElement
		* @param[in] const ISystemScene::SystemSceneMap & systemScenes
		* @return ( ISystemComponent* )
		*/
		virtual ISystemComponent* DeSerialize( const std::string entityName, ticpp::Element* componentElement, const ISystemScene::SystemSceneMap& systemScenes ) = 0;

	};
};

#endif