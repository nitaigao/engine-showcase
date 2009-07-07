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


		/*! DeSerializes the Component from the Supplied Element
		*
		* @param[in] const std::string entityName
		* @param[in] const ticpp::Element & componentElement
		* @param[in] const ISystemScene::SystemSceneMap & systemScenes
		* @return ( ISystemComponent* )
		*/
		ISystemComponent* DeSerialize( const std::string entityName, ticpp::Element* componentElement, const ISystemScene::SystemSceneMap& systemScenes );

	private:

		GraphicsComponentSerializer( const GraphicsComponentSerializer & copy ) { };
		GraphicsComponentSerializer & operator = ( const GraphicsComponentSerializer & copy ) { return *this; };

	};
};

#endif