/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   PhysicsComponentSerializer.h
*  @date   2009/04/27
*/
#ifndef __PHYSICSCOMPONENTSERIALIZER_H
#define __PHYSICSCOMPONENTSERIALIZER_H

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


		/*! De serializes the Component from the supplied Node
		*
		*  @param[in] const std::string & entityName
		*  @param[in] const YAML::Node & componentNode
		*  @param[in] const ISystemScene::SystemSceneMap & systemScenes
		*  @return (ISystemComponent*)
		*/
		ISystemComponent* DeSerialize( const std::string& entityName, const YAML::Node& componentNode, const ISystemScene::SystemSceneMap& systemScenes );

	private:

		PhysicsComponentSerializer( const PhysicsComponentSerializer & copy ) { };
		PhysicsComponentSerializer & operator = ( const PhysicsComponentSerializer & copy ) { return *this; };

	};
};

#endif