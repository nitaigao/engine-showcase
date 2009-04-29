/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   AIComponentSerializer.h
*  @date   2009/04/27
*/
#ifndef __AICOMPONENTSERIALIZER_H
#define __AICOMPONENTSERIALIZER_H

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


		/*! De serializes the Component from the supplied Node
		*
		*  @param[in] const std::string & entityName
		*  @param[in] const YAML::Node & componentNode
		*  @param[in] const ISystemScene::SystemSceneMap & systemScenes
		*  @return (ISystemComponent*)
		*/
		ISystemComponent* DeSerialize( const std::string& entityName, const YAML::Node& componentNode, const ISystemScene::SystemSceneMap& systemScenes );

	private:

		AIComponentSerializer( const AIComponentSerializer & copy ) { };
		AIComponentSerializer & operator = ( const AIComponentSerializer & copy ) { return *this; };

	};
};

#endif