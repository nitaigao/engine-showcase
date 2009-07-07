/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   XMLSerializer.h
*  @date   2009/04/27
*/
#ifndef WORLDLOADER_H
#define WORLDLOADER_H

#include <queue>
#include <yaml.h>

#include "../../Utility/tinyxml/ticpp.h"

#include "IWorldLoader.hpp"
#include "../IWorld.hpp"

namespace Serialization
{
	/*!
	 *  De serializes a world from storage  
	 */
	class XMLSerializer : public IWorldSerializer
	{
		typedef std::queue< ticpp::Node* > NodePtrList;
		typedef std::map< System::Types::Type, ticpp::Node* > NodePtrMap;

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		~XMLSerializer( );


		/*! Default Constructor
		 *
		 *  @param[in] IWorld * world
		 *  @return ()
		 */
		XMLSerializer( State::IWorld* world )
			: _world( world )
			, _loadTotal( 0 )
			, _loadProgress( 0 )
		{

		}

		/*! Loads a Serialized world from the File System
		*
		*  @param[in] const std::string & levelPath
		*  @return (void)
		*/
		void Load( const std::string& levelPath );


		/*! Steps the loading process
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		void Update( const float& deltaMilliseconds );


		/*! Returns whether or no the Serializer has finished its loading task
		*
		*  @return (bool)
		*/
		inline bool IsFinishedLoading( ) const { return ( _loadProgress == _loadTotal ); };

	private:

		XMLSerializer( ) { };
		XMLSerializer( const XMLSerializer & copy ) { };
		XMLSerializer & operator = ( const XMLSerializer & copy ) { return *this; };

		void LoadElement( ticpp::Element* element );
		void DeserializeElement( ticpp::Element* element );

		void LoadColor( ticpp::Element* element );
		void LoadEntity( ticpp::Element* element );
		void LoadEntityComponents( ticpp::Element* element, NodePtrMap& components );

		void LoadNode( const YAML::Node& node );
		void LoadSkyBox( const YAML::Node& node );
		void LoadFog( const YAML::Node& node );
		void LoadLink( const YAML::Node& node );

		State::IWorld* _world;

		NodePtrList _loadQueueEl;

		int _loadProgress;
		int _loadTotal;

	};
};

#endif