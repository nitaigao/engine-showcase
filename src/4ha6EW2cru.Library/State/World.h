/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   World.h
*  @date   2009/04/27
*/
#ifndef WORLD_H
#define WORLD_H

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
		World( )
			: m_stepAccumulator( 0 )
			, m_lastEntityId( 0 )
		{

		}


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
		inline void AddSystemScene( ISystemScene* systemScene ) { m_systemScenes[ systemScene->GetType( ) ] = systemScene; }
		
		
		/*! Gets a list of internal system scenes
		*
		*  @return (const SystemSceneMap&)
		*/
		inline const SystemSceneMap& GetSystemScenes( ) const { return m_systemScenes; };


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

		std::string m_name;
		IWorldEntity::WorldEntityList m_entities;
		SystemSceneMap m_systemScenes;

		unsigned int m_lastEntityId;
		float m_stepAccumulator;
	};
};

#endif