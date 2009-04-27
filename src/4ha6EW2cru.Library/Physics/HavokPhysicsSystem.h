/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   HavokPhysicsSystem.h
*  @date   2009/04/26
*/
#ifndef __HAVOKPHYSICS_SYSTEM
#define __HAVOKPHYSICS_SYSTEM

#include "../Service/IService.hpp"
#include "../System/ISystem.hpp"
#include "HavokPhysicsSystemScene.h"

namespace Physics
{
	/*! 
	 *  The Physics System
	 */
	class HavokPhysicsSystem : public ISystem, public IService
	{

		typedef std::vector< HavokPhysicsSystemScene* > SceneList;

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
			: _threadMemory( 0 )
			, _stackBuffer( 0 )
		{

		}


		/*! Initializes the System
		*
		*  @return (void)
		*/
		void Initialize( );


		/*! Steps the System's internal data
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		inline void Update( const float& deltaMilliseconds ) { };


		/*! Releases internal data of the System
		*
		*  @return (System::Types::Type)
		*/
		void Release( );


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
		*  @return (AnyValueMap)
		*/
		inline AnyValue::AnyValueMap GetProperties( ) const { return AnyValue::AnyValueMap( ); };
		

		/*! Sets a System Property
		*
		*  @param[in] const std::string & name
		*  @param[in] AnyValue value
		*  @return (void)
		*/
		inline void SetProperty( const std::string& name, AnyValue value ) { };


		/*! Executes a command on the Exposed Physics Service
		*
		*  @param[in] const std::string & actionName
		*  @param[in] AnyValue::AnyValueMap & parameters
		*  @return (AnyValue::AnyValueMap)
		*/
		AnyValue::AnyValueMap Execute( const std::string& actionName, AnyValue::AnyValueMap& parameters );
		
		
		/*! Performs a Ray Query on the Scene, destination is not direction, its the end point of the Ray
		 *
		 *  @param[in] Maths::MathVector3 origin
		 *  @param[in] Maths::MathVector3 destination
		 *  @param[in] bool sortByDistance
		 *  @return (std::vector< std::string >)
		 */
		std::vector< std::string > RayQuery( const Maths::MathVector3& origin, const Maths::MathVector3& destination, const bool& sortByDistance );

	private:

		static void errorReportFunction( const char* str, void* errorOutputObject );

		hkThreadMemory* _threadMemory;
		char* _stackBuffer;

		SceneList _scenes;

		HavokPhysicsSystem( const HavokPhysicsSystem & copy ) { };
		HavokPhysicsSystem & operator = ( const HavokPhysicsSystem & copy ) { return *this; };

	};
};

#endif