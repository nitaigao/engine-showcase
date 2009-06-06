/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   AnimationComponentSerializer.h
*  @date   2009/04/25
*/
#ifndef ANIMATIONCOMPONENTSERIALIZER_H
#define ANIMATIONCOMPONENTSERIALIZER_H

#include "IComponentSerializer.hpp"

namespace Serialization
{
	/*! 
	 *  The AnimationComponentSerializer
	 */
	class AnimationComponentSerializer : public IComponentSerializer
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~AnimationComponentSerializer( ) { };


		/*! Default Constructor
		*
		* @return (  )
		*/
		AnimationComponentSerializer( ) { };


		/*! De serializes the Component from the supplied Node
		*
		*  @param[in] const std::string & entityName
		*  @param[in] const YAML::Node & componentNode
		*  @param[in] const ISystemScene::SystemSceneMap & systemScenes
		*  @return (ISystemComponent*)
		*/
		ISystemComponent* DeSerialize( const std::string& entityName, const YAML::Node& componentNode, const ISystemScene::SystemSceneMap& systemScenes );

	private:

		AnimationComponentSerializer( const AnimationComponentSerializer & copy ) { };
		AnimationComponentSerializer & operator = ( const AnimationComponentSerializer & copy ) { return *this; };
		
	};
};

#endif