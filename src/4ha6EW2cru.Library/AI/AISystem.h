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
		AISystem( ) { };


		/*! Initializes the System
		 *
		 *  @return (void)
		 */
		inline void Initialize( ) { };


		/*! Steps the System's internal data
		 *
		 *  @param[in] float deltaMilliseconds
		 *  @return (void)
		 */
		inline void Update( const float& deltaMilliseconds ) { };


		/*! Releases the System
		 *
		 *  @return (void)
		 */
		inline void Release( ) { };
		

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
		 *  @return (AnyValueMap)
		 */
		inline AnyValue::AnyValueMap GetAttributes( ) const { return m_attributes; };


		/*! Sets a System Property
		 *
		 *  @param[in] const std::string & name
		 *  @param[in] AnyValue value
		 *  @return (void)
		 */
		inline void SetAttribute( const std::string& name, AnyValue value ) { };

	private:

		AnyValue::AnyValueMap m_attributes;
	};
};

#endif