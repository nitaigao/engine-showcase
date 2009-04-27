/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   GeometryComponentSerializer.h
*  @date   2009/04/27
*/
#ifndef __GEOMETRYCOMPONENTSERIALIZER_H
#define __GEOMETRYCOMPONENTSERIALIZER_H

#include "IComponentSerializer.hpp"

namespace Serialization
{
	/*!
	 *  A Geometry Component Serializer 
	 */
	class GeometryComponentSerializer : public IComponentSerializer
	{

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		~GeometryComponentSerializer( ) { };


		/*! Default Constructor
		*
		*  @return ()
		*/
		GeometryComponentSerializer( ) { };


		/*! De serializes the Component from the supplied Node
		*
		*  @param[in] const std::string & entityName
		*  @param[in] const YAML::Node & componentNode
		*  @param[in] const ISystemScene::SystemSceneMap & systemScenes
		*  @return (ISystemComponent*)
		*/
		ISystemComponent* Deserialize( const std::string& entityName, const YAML::Node& componentNode, const ISystemScene::SystemSceneMap& systemScenes );

	private:

		GeometryComponentSerializer( const GeometryComponentSerializer & copy ) { };
		GeometryComponentSerializer & operator = ( const GeometryComponentSerializer & copy ) { return *this; };

	};
};

#endif