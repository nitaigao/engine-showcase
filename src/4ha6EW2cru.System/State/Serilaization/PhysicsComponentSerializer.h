/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   PhysicsComponentSerializer.h
*  @date   2009/04/27
*/
#ifndef PHYSICSCOMPONENTSERIALIZER_H
#define PHYSICSCOMPONENTSERIALIZER_H

#include "IComponentSerializer.hpp"

namespace Serialization
{
	/*!
	 *  A Physics Component Serializer 
	 */
	class PhysicsComponentSerializer : public IComponentSerializer
	{

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		~PhysicsComponentSerializer( ) { };


		/*! Default Constructor
		*
		*  @return ()
		*/
		PhysicsComponentSerializer( ) { };


		/*! DeSerializes the Component from the Supplied Element
		*
		* @param[in] const std::string entityName
		* @param[in] const ticpp::Element & componentElement
		* @param[in] const ISystemScene::SystemSceneMap & systemScenes
		* @return ( ISystemComponent* )
		*/
		ISystemComponent* DeSerialize( const std::string entityName, ticpp::Element* componentElement, const ISystemScene::SystemSceneMap& systemScenes );

	private:

		PhysicsComponentSerializer( const PhysicsComponentSerializer & copy ) { };
		PhysicsComponentSerializer & operator = ( const PhysicsComponentSerializer & copy ) { return *this; };

	};
};

#endif