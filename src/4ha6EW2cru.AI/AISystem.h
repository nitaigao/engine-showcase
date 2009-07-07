/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   AISystem.h
*  @date   2009/04/25
*/
#ifndef AISYSTEM_H
#define AISYSTEM_H

#include "IAISystem.hpp"

namespace AI
{
	/*! 
	 *  The Artificial Intelligence System
	 */
	class AISystem : public IAISystem
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~AISystem( ) { };


		/*! Default Constructor
		 *
		 *  @return ()
		 */
		AISystem( )
			: m_scene( 0 )
		{

		}


		/*! Initializes the System
		 *
		 *  @return (void)
		 */
		inline void Initialize( Configuration::IConfiguration* configuration ) { };


		/*! Steps the System's internal data
		 *
		 *  @param[in] float deltaMilliseconds
		 *  @return (void)
		 */
		void Update( const float& deltaMilliseconds );


		/*! Releases the System
		 *
		 *  @return (void)
		 */
		inline void Release( ) { };


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
		inline System::Types::Type GetType( ) const { return System::Types::AI; };


		/*! Creates a System Scene
		 *
		 *  @return (ISystemScene*)
		 */
		ISystemScene* CreateScene( );


		/*! Gets the System's Properties
		 *
		 *  @return (AnyTypeMap)
		 */
		inline AnyType::AnyTypeMap GetAttributes( ) const { return m_attributes; };


		/*! Sets a System Property
		 *
		 *  @param[in] const std::string & name
		 *  @param[in] AnyType value
		 *  @return (void)
		 */
		inline void SetAttribute( const std::string& name, AnyType value ) { };

	private:

		AnyType::AnyTypeMap m_attributes;
		ISystemScene* m_scene;
	};
};

#endif