/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   GeometrySystemScene.h
*  @date   2009/04/26
*/
#ifndef GEOMETRYSYSTEMSCENE_H
#define GEOMETRYSYSTEMSCENE_H

#include "../System/ISystem.hpp"

namespace Geometry
{
	/*! 
	*  A Geometry Specific Scene
	*/
	class GeometrySystemScene : public ISystemScene
	{

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		~GeometrySystemScene( ) { };


		/*! Default Constructor
		 *
		 *  @param[in] ISystem * system
		 *  @return ()
		 */
		explicit GeometrySystemScene( ISystem* system )
			: m_system( system )
		{

		}


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
		inline void Update( const float& deltaMilliseconds ) { };


		/*! Destroys the System Scene
		*
		*  @return (void)
		*/
		inline void Destroy( ) { };


		/*! Gets the System::Types::Type of the SystemScene
		*
		*  @return (System::Types::Type)
		*/
		inline System::Types::Type GetType( ) const { return System::Types::GEOMETRY; };


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

	private:

		ISystem* m_system;

		GeometrySystemScene( ) { };
		GeometrySystemScene( const GeometrySystemScene & copy ) { };
		GeometrySystemScene & operator = ( const GeometrySystemScene & copy ) { return *this; };

	};
};

#endif