/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   src\4ha6EW2cru.Library\AI\AISystem.h
*  @date   2009/04/25
*/
#ifndef __AISYSTEM_H
#define __AISYSTEM_H

#include "../System/ISystem.hpp"

namespace AI
{
	/*! 
	 *  The Artificial Intelligence System
	 */
	class AISystem : public ISystem
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~AISystem( ) { };

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
		inline void Update( float deltaMilliseconds ) { };


		/*! Releases the System
		 *
		 *  @return (void)
		 */
		inline void Release( ) { };
		

		/*! Returns the type of the System
		 *
		 *  @return (System::Types::Type)
		 */
		inline System::Types::Type GetType( ) { return System::Types::AI; };


		/*! Creates the System Scene
		 *
		 *  @return (ISystemScene*)
		 */
		ISystemScene* CreateScene( );


		/*! Gets the System's Properties
		 *
		 *  @return (AnyValueMap)
		 */
		inline AnyValue::AnyValueMap GetProperties( ) { return _properties; };


		/*! Sets a System Property
		 *
		 *  @param[in] const std::string & name
		 *  @param[in] AnyValue value
		 *  @return (void)
		 */
		inline void SetProperty( const std::string& name, AnyValue value ) { };

	private:

		AnyValue::AnyValueMap _properties;
	};
};

#endif