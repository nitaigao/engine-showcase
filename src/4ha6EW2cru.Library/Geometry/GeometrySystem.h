/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   GeometrySystem.h
*  @date   2009/04/26
*/
#ifndef __GEOMETRYSYSTEM_H
#define __GEOMETRYSYSTEM_H

#include "../System/SystemType.hpp"
#include "../System/ISystem.hpp"
#include "../System/ISystemScene.hpp"

namespace Geometry
{
	/*! 
	 *  The Geometry System
	 */
	class GeometrySystem : public ISystem
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~GeometrySystem( ) { };


		/*! Default Constructor
		 *
		 *  @return ()
		 */
		GeometrySystem( ) { };


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


		/*! Releases internal data of the System
		*
		*  @return (System::Types::Type)
		*/
		inline void Release( ) { };


		/*! Returns the type of the System
		*
		*  @return (System::Types::Type)
		*/
		inline System::Types::Type GetType( ) const { return System::Types::GEOMETRY; };


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

	private:

		GeometrySystem( const GeometrySystem & copy ) { };
		GeometrySystem & operator = ( const GeometrySystem & copy ) { return *this; };

	};
};


#endif