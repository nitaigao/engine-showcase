/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   GraphicsComponentSerializer.h
*  @date   2009/04/27
*/
#ifndef GRAPHICSCOMPONENTSERIALIZER_H
#define GRAPHICSCOMPONENTSERIALIZER_H

#include "IComponentSerializer.hpp"

namespace Serialization
{
	/*!
	 *  A Graphics Component Serializer 
	 */
	class GraphicsComponentSerializer : public IComponentSerializer
	{

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		~GraphicsComponentSerializer( ) { };


		/*! Default Constructor
		*
		*  @return ()
		*/
		GraphicsComponentSerializer( ) { };


		/*! De serializes the Component from the supplied Node
		*
		*  @param[in] const std::string & entityName
		*  @param[in] const YAML::Node & componentNode
		*  @param[in] const ISystemScene::SystemSceneMap & systemScenes
		*  @return (ISystemComponent*)
		*/
		ISystemComponent* DeSerialize( const std::string& entityName, const YAML::Node& componentNode, const ISystemScene::SystemSceneMap& systemScenes );

	private:

		GraphicsComponentSerializer( const GraphicsComponentSerializer & copy ) { };
		GraphicsComponentSerializer & operator = ( const GraphicsComponentSerializer & copy ) { return *this; };

	};
};

#endif