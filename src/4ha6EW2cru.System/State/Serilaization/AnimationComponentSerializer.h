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


		/*! DeSerializes the Component from the Supplied Element
		*
		* @param[in] const std::string entityName
		* @param[in] const ticpp::Element & componentElement
		* @param[in] const ISystemScene::SystemSceneMap & systemScenes
		* @return ( ISystemComponent* )
		*/
		ISystemComponent* DeSerialize( const std::string entityName, ticpp::Element* componentElement, const ISystemScene::SystemSceneMap& systemScenes );

	private:

		AnimationComponentSerializer( const AnimationComponentSerializer & copy ) { };
		AnimationComponentSerializer & operator = ( const AnimationComponentSerializer & copy ) { return *this; };
		
	};
};

#endif