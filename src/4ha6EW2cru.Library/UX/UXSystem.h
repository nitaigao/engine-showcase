/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   UXSystem.h
*  @date   2009/04/26
*/
#ifndef __UXSYSTEM_H
#define __UXSYSTEM_H

#include "IUXSystem.hpp"
#include "IUXSystemScene.hpp"

namespace UX
{
	/*! 
	 *  The System that controls the User Experience ( ie. GUI Elements )
	 */
	class UXSystem : public IUXSystem
	{

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		~UXSystem( ) { };


		/*! Default Constructor
		 *
		 *  @return ()
		 */
		UXSystem( )
			: _scene( 0 )
		{

		}


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
		inline System::Types::Type GetType( ) const { return System::Types::UX; };


		/*! Creates a System Scene
		*
		*  @return (ISystemScene*)
		*/
		ISystemScene* CreateScene( );


		/*! Gets the System's Properties
		*
		*  @return (AnyValueMap)
		*/
		inline AnyValue::AnyValueMap GetProperties( ) const { return _properties; };


		/*! Sets a System Property
		*
		*  @param[in] const std::string & name
		*  @param[in] AnyValue value
		*  @return (void)
		*/
		inline void SetProperty( const std::string& name, AnyValue value ) { };

	private:

		AnyValue::AnyValueMap _properties;

		IUXSystemScene* _scene;

	};
};

#endif