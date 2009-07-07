/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   GeometryComponentSerializer.h
*  @date   2009/04/27
*/
#ifndef GEOMETRYCOMPONENTSERIALIZER_H
#define GEOMETRYCOMPONENTSERIALIZER_H

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


		/*! DeSerializes the Component from the Supplied Element
		*
		* @param[in] const std::string entityName
		* @param[in] const ticpp::Element & componentElement
		* @param[in] const ISystemScene::SystemSceneMap & systemScenes
		* @return ( ISystemComponent* )
		*/
		ISystemComponent* DeSerialize( const std::string entityName, ticpp::Element* componentElement, const ISystemScene::SystemSceneMap& systemScenes );

	private:

		GeometryComponentSerializer( const GeometryComponentSerializer & copy ) { };
		GeometryComponentSerializer & operator = ( const GeometryComponentSerializer & copy ) { return *this; };

	};
};

#endif