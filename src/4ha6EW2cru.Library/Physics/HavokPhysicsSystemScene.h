/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   HavokPhysicsSystemScene.h
*  @date   2009/04/26
*/
#ifndef __HAVOKPHYSICSSYSTEMSCENE_H
#define __HAVOKPHYSICSSYSTEMSCENE_H

#include "../System/ISystemScene.hpp"

#include <Common/Base/hkBase.h>
#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Dynamics/World/Listener/hkpWorldPostSimulationListener.h>
#include <Physics/Dynamics/Entity/hkpEntityActivationListener.h>

#include <Common/Visualize/hkVisualDebugger.h>
#include <Physics/Utilities/VisualDebugger/hkpPhysicsContext.h>			

#include "IPhysicsSystemComponent.hpp"

namespace Physics
{
	/*! 
	*  A Physics Specific Scene
	*/
	class HavokPhysicsSystemScene : public ISystemScene
	{

		typedef std::map< std::string, IPhysicsSystemComponent* > PhysicsSystemComponentList;

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~HavokPhysicsSystemScene( );


		/*! Default Constructor
		 *
		 *  @param[in] const hkpWorldCinfo & worldInfo
		 *  @return ()
		 */
		HavokPhysicsSystemScene( const hkpWorldCinfo& worldInfo );


		/*! Initializes the System Scene
		*
		*  @return (void)
		*/
		void Initialize( ) { };


		/*! Steps internal data of the SystemScene
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		void Update( const float& deltaMilliseconds );


		/*! Destroys the System Scene
		*
		*  @return (void)
		*/
		inline void Destroy( ) { };


		/*! Gets the System::Types::Type of the SystemScene
		*
		*  @return (System::Types::Type)
		*/
		inline System::Types::Type GetType( ) const { return System::Types::PHYSICS; };


		/*! Creates a SystemComponent specific to the SystemScene
		*
		*  @param[in] const std::string & name
		*  @param[in] const std::string & type
		*  @return (ISystemComponent*)
		*/
		ISystemComponent* CreateComponent( const std::string& name, const std::string& type );
		
		
		/*! Destroys a SystemComponent created by the SystemScene
		*
		*  @param[in] ISystemComponent * component
		*  @return (void)
		*/
		void DestroyComponent( ISystemComponent* component );


		/*! Returns the Physics World from within Havok
		 *
		 *  @return (hkpWorld*)
		 */
		hkpWorld* GetWorld( ) const { return _world; };

	private:

		void postSimulationCallback( hkpWorld* world );
		void inactiveEntityMovedCallback( hkpEntity* entity );


		void entityDeactivatedCallback( hkpEntity* entity );
		void entityActivatedCallback( hkpEntity* entity );

		hkpWorld* _world;
		hkVisualDebugger* _vdb;
		hkpPhysicsContext* _context;

		PhysicsSystemComponentList _components;

		HavokPhysicsSystemScene( ) { };
		HavokPhysicsSystemScene( const HavokPhysicsSystemScene & copy ) { };
		HavokPhysicsSystemScene & operator = ( const HavokPhysicsSystemScene & copy ) { return *this; };

	};
};

#endif