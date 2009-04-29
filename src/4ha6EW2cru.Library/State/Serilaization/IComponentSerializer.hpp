/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IComponentSerializer.hpp
*  @date   2009/04/27
*/
#ifndef __ICOMPONENTSERIALIZER_HPP
#define __ICOMPONENTSERIALIZER_HPP

#include <string>
#include <yaml.h>

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


		/*! De serializes the Component from the supplied Node
		 *
		 *  @param[in] const std::string & entityName
		 *  @param[in] const YAML::Node & componentNode
		 *  @param[in] const ISystemScene::SystemSceneMap & systemScenes
		 *  @return (ISystemComponent*)
		 */
		virtual ISystemComponent* DeSerialize( const std::string& entityName, const YAML::Node& componentNode, const ISystemScene::SystemSceneMap& systemScenes ) = 0;

	};
};

#endif