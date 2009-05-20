/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   HavokPhysicsSystemScene.h
*  @date   2009/04/26
*/
#ifndef HAVOKPHYSICSSYSTEMSCENE_H
#define HAVOKPHYSICSSYSTEMSCENE_H

#include "../System/ISystemScene.hpp"

#include <Common/Base/hkBase.h>
#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Dynamics/World/Listener/hkpWorldPostSimulationListener.h>
#include <Physics/Dynamics/Entity/hkpEntityActivationListener.h>

#include <Physics/Collide/Filter/Group/hkpGroupFilter.h>

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
		hkpWorld* GetWorld( ) const { return m_world; };

	private:

		void postSimulationCallback( hkpWorld* world );
		void inactiveEntityMovedCallback( hkpEntity* entity );

		void entityDeactivatedCallback( hkpEntity* entity );
		void entityActivatedCallback( hkpEntity* entity );

		hkpWorld* m_world;
#ifdef _DEBUG
		hkVisualDebugger* m_vdb;
#endif
		hkpPhysicsContext* m_context;

		hkpGroupFilter* m_groupFilter;

		IPhysicsSystemComponent::PhysicsSystemComponentList m_components;

		HavokPhysicsSystemScene( ) { };
		HavokPhysicsSystemScene( const HavokPhysicsSystemScene & copy ) { };
		HavokPhysicsSystemScene & operator = ( const HavokPhysicsSystemScene & copy ) { return *this; };

	};
};

#endif