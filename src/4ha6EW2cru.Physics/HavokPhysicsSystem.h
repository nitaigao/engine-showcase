/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   HavokPhysicsSystem.h
*  @date   2009/04/26
*/
#ifndef HAVOKPHYSICS_SYSTEM
#define HAVOKPHYSICS_SYSTEM

#include "Service/IService.hpp"
#include "System/ISystem.hpp"
#include "HavokPhysicsSystemScene.h"

#include "IPhysicsSystem.hpp"

namespace Physics
{
	/*! 
	 *  The Physics System
	 */
	class HavokPhysicsSystem : public IPhysicsSystem, public Services::IService
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~HavokPhysicsSystem( ) { };


		/*! Default Constructor
		 *
		 *  @return ()
		 */
		HavokPhysicsSystem( )
			: m_threadMemory( 0 )
			, m_stackBuffer( 0 )
			, m_scene( 0 )
		{

		}


		/*! Initializes the System
		*
		*  @return (void)
		*/
		void Initialize( Configuration::IConfiguration* configuration );


		/*! Steps the System's internal data
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		void Update( const float& deltaMilliseconds );


		/*! Releases internal data of the System
		*
		*  @return (System::Types::Type)
		*/
		void Release( );


		/*! Messages the system with a command
		*
		* @param[in] const std::string & message
		* @param[in] AnyType::AnyTypeMap parameters
		* @return ( void )
		*/
		inline void Message( const std::string& message, AnyType::AnyTypeMap parameters ) { };


		/*! Returns the type of the System
		*
		*  @return (System::Types::Type)
		*/
		inline System::Types::Type GetType( ) const { return System::Types::PHYSICS; };


		/*! Creates a System Scene
		*
		*  @return (ISystemScene*)
		*/
		ISystemScene* CreateScene( );


		/*! Gets the System's Properties
		*
		*  @return (AnyTypeMap)
		*/
		inline AnyType::AnyTypeMap GetAttributes( ) const { return AnyType::AnyTypeMap( ); };
		

		/*! Sets a System Property
		*
		*  @param[in] const std::string & name
		*  @param[in] AnyType value
		*  @return (void)
		*/
		inline void SetAttribute( const std::string& name, AnyType value ) { };


		/*! Executes a command on the Exposed Physics Service
		*
		*  @param[in] const std::string & actionName
		*  @param[in] AnyType::AnyTypeMap & parameters
		*  @return (AnyType::AnyTypeMap)
		*/
		AnyType::AnyTypeMap Execute( const std::string& actionName, AnyType::AnyTypeMap& parameters );
		
		
		/*! Performs a Ray Query on the Scene, destination is not direction, its the end point of the Ray
		 *
		 *  @param[in] Maths::MathVector3 origin
		 *  @param[in] Maths::MathVector3 destination
		 *  @param[in] bool sortByDistance
		 *  @return (std::vector< std::string >)
		 */
		std::vector< std::string > RayQuery( const Maths::MathVector3& origin, const Maths::MathVector3& destination, const bool& sortByDistance, const unsigned int& maxResults );


		/*! Returns the Physics World from within Havok
		*
		*  @return (hkpWorld*)
		*/
		hkpWorld* GetWorld( ) const { return m_world; };

	private:

		static void errorReportFunction( const char* str, void* errorOutputObject );

		HavokPhysicsSystemScene* m_scene;

		hkThreadMemory* m_threadMemory;
		char* m_stackBuffer;

		hkpPhysicsContext* m_context;
		hkpGroupFilter* m_groupFilter;

		hkpWorld* m_world;
#ifdef _DEBUG
		hkVisualDebugger* m_vdb;
#endif

		HavokPhysicsSystem( const HavokPhysicsSystem & copy ) { };
		HavokPhysicsSystem & operator = ( const HavokPhysicsSystem & copy ) { return *this; };

	};
};

#endif