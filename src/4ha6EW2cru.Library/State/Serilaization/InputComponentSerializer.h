/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   InputComponentSerializer.h
*  @date   2009/04/27
*/
#ifndef __INPUTCOMPONENTSERIALIZER_H
#define __INPUTCOMPONENTSERIALIZER_H

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


		/*! De serializes the Component from the supplied Node
		*
		*  @param[in] const std::string & entityName
		*  @param[in] const YAML::Node & componentNode
		*  @param[in] const ISystemScene::SystemSceneMap & systemScenes
		*  @return (ISystemComponent*)
		*/
		ISystemComponent* DeSerialize( const std::string& entityName, const YAML::Node& componentNode, const ISystemScene::SystemSceneMap& systemScenes );

	private:

		InputComponentSerializer( const InputComponentSerializer & copy ) { };
		InputComponentSerializer & operator = ( const InputComponentSerializer & copy ) { return *this; };

	};
};

#endif