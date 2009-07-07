/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   AnimationSystem.h
*  @date   2009/04/25
*/
#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "IAnimationSystem.hpp"
#include "IAnimationSystemScene.hpp"

namespace Animation
{
	/*! 
	 *  The AnimationSystem
	 */
	class AnimationSystem : public IAnimationSystem
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~AnimationSystem( ) { };

		
		/*! Default Constructor
		*
		*  @return ()
		*/
		AnimationSystem( )
			: m_scene( 0 )
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
		inline System::Types::Type GetType( ) const { return System::Types::ANIMATION; };


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


		/*! Sound Service Interface
		*
		*  @param[in] const std::string & actionName
		*  @param[in] AnyType::AnyTypeMap & parameters
		*  @return (AnyType::AnyTypeMap)
		*/
		AnyType::AnyTypeMap Execute( const std::string& actionName, AnyType::AnyTypeMap& parameters );

	private:

		AnimationSystem( const AnimationSystem & copy ) { };
		AnimationSystem & operator = ( const AnimationSystem & copy ) { return *this; };

		IAnimationSystemScene* m_scene;
		
	};
};

#endif