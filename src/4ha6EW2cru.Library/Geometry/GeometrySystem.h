/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   GeometrySystem.h
*  @date   2009/04/26
*/
#ifndef GEOMETRYSYSTEM_H
#define GEOMETRYSYSTEM_H

#include "../System/SystemType.hpp"
#include "../System/ISystem.hpp"
#include "../System/ISystemScene.hpp"

#include "../Service/IService.hpp"

namespace Geometry
{
	/*! 
	 *  The Geometry System
	 */
	class GeometrySystem : public ISystem, public IService
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


		/*! Messages the system with a command
		*
		* @param[in] const std::string & message
		* @param[in] AnyType::AnyTypeMap parameters
		* @return ( void )
		*/
		void Message( const std::string& message, AnyType::AnyTypeMap parameters );


		/*! Executes a command on the Service
		*
		*  @param[in] const std::string & actionName
		*  @param[in] AnyType::AnyTypeMap & parameters
		*  @return (AnyType::AnyTypeMap)
		*/
		AnyType::AnyTypeMap Execute( const std::string& actionName, AnyType::AnyTypeMap& parameters );


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

	private:

		GeometrySystem( const GeometrySystem & copy ) { };
		GeometrySystem & operator = ( const GeometrySystem & copy ) { return *this; };

	};
};


#endif