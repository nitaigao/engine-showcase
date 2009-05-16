/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   SoundComponentSerializer.h
*  @date   2009/05/15
*/
#ifndef __SOUNDCOMPONENTSERIALIZER_H
#define __SOUNDCOMPONENTSERIALIZER_H

#include "IComponentSerializer.hpp"

namespace Serialization
{
	/*!
	 *  The Sound Component Serializer 
	 */
	class SoundComponentSerializer : public IComponentSerializer
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~SoundComponentSerializer( ) { };


		/*! Default Constructor
		 *
		 *  @return ()
		 */
		SoundComponentSerializer( )
		{

		}

		/*! De serializes the Component from the supplied Node
		*
		*  @param[in] const std::string & entityName
		*  @param[in] const YAML::Node & componentNode
		*  @param[in] const ISystemScene::SystemSceneMap & systemScenes
		*  @return (ISystemComponent*)
		*/
		ISystemComponent* DeSerialize( const std::string& entityName, const YAML::Node& componentNode, const ISystemScene::SystemSceneMap& systemScenes );
		

	private:

		
		SoundComponentSerializer( const SoundComponentSerializer & copy ) { };
		SoundComponentSerializer & operator = ( const SoundComponentSerializer & copy ) { return *this; };

	};
};

#endif