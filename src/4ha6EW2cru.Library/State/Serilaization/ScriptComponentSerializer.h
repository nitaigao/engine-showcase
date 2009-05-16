/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   ScriptComponentSerializer.h
*  @date   2009/04/27
*/
#ifndef SCRIPTCOMPONENTSERIALIZER_H
#define SCRIPTCOMPONENTSERIALIZER_H

#include "IComponentSerializer.hpp"

namespace Serialization
{
	/*!
	 *  A Script Component Serializer 
	 */
	class ScriptComponentSerializer : public IComponentSerializer
	{

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		~ScriptComponentSerializer( ) { };


		/*! Default Constructor
		*
		*  @return ()
		*/
		ScriptComponentSerializer( ) { };


		/*! De serializes the Component from the supplied Node
		*
		*  @param[in] const std::string & entityName
		*  @param[in] const YAML::Node & componentNode
		*  @param[in] const ISystemScene::SystemSceneMap & systemScenes
		*  @return (ISystemComponent*)
		*/
		ISystemComponent* DeSerialize( const std::string& entityName, const YAML::Node& componentNode, const ISystemScene::SystemSceneMap& systemScenes );

	private:

		ScriptComponentSerializer( const ScriptComponentSerializer & copy ) { };
		ScriptComponentSerializer & operator = ( const ScriptComponentSerializer & copy ) { return *this; };

	};
};

#endif