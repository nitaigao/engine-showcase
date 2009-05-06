/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   WorldSerializer.h
*  @date   2009/04/27
*/
#ifndef __WORLDLOADER_H
#define __WORLDLOADER_H

#include <queue>
#include <yaml.h>

#include "IWorldLoader.hpp"
#include "../IWorld.hpp"

namespace Serialization
{
	/*!
	 *  De serializes a world from storage  
	 */
	class WorldSerializer : public IWorldSerializer
	{
		typedef std::queue< YAML::Node* > NodeList;

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		~WorldSerializer( ) { };


		/*! Default Constructor
		 *
		 *  @param[in] IWorld * world
		 *  @return ()
		 */
		WorldSerializer( State::IWorld* world )
			: _world( world )
			, _activeNodeIndex( 0 )
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

	private:

		WorldSerializer( ) { };
		WorldSerializer( const WorldSerializer & copy ) { };
		WorldSerializer & operator = ( const WorldSerializer & copy ) { return *this; };

		void LoadNode( const YAML::Node& node );
		void LoadColor( const YAML::Node& node );
		void LoadSkyBox( const YAML::Node& node );
		void LoadFog( const YAML::Node& node );
		void LoadEntity( const YAML::Node& node );
		void LoadLink( const YAML::Node& node );

		void LoadEntityComponents( const YAML::Node& node, State::IWorldEntity* entity );

		State::IWorld* _world;

		NodeList _loadQueue;

		unsigned int _activeNodeIndex;
		int _loadProgress;
		int _loadTotal;

	};
};

#endif