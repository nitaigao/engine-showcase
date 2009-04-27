/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   World.h
*  @date   2009/04/27
*/
#ifndef __WORLD_H
#define __WORLD_H

#include "IWorld.hpp"

namespace State
{
	/*!
	 *  The Container for all Entities 
	 */
	class World : public IWorld
	{

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		~World( );


		/*! Default Constructor
		 *
		 *  @return ()
		 */
		World( ) { };


		/*! Creates a World Entity Container
		*
		*  @param[in] const std::string & name
		*  @return (IWorldEntity*)
		*/
		IWorldEntity* CreateEntity( const std::string& name );


		/*! Finds a World Entity Container of the given name
		*
		*  @param[in] const std::string & name
		*  @return (IWorldEntity*)
		*/
		IWorldEntity* FindEntity( const std::string& name );


		/*! Adds a System Scene to the internal scene list
		*
		*  @param[in] ISystemScene * systemScene
		*  @return (void)
		*/
		inline void AddSystemScene( ISystemScene* systemScene ) { _systemScenes[ systemScene->GetType( ) ] = systemScene; }
		
		
		/*! Gets a list of internal system scenes
		*
		*  @return (const SystemSceneMap&)
		*/
		inline const SystemSceneMap& GetSystemScenes( ) const { return _systemScenes; };


		/*! Steps the world internal data
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		void Update( const float& deltaMilliseconds );


		/*! Destroys all entities within the world
		*
		*  @return (void)
		*/
		void Clear( );

	private:

		World( const World & copy ) { };
		World & operator = ( const World & copy ) { return *this; };

		std::string _name;
		IWorldEntity::WorldEntityList _entities;
		SystemSceneMap _systemScenes;

	};
};

#endif