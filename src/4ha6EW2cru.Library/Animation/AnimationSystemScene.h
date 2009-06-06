/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   AnimationSystemScene.h
*  @date   2009/04/25
*/
#ifndef ANIMATIONSYSTEMSCENE_H
#define ANIMATIONSYSTEMSCENE_H

#include "IAnimationSystemScene.hpp"

namespace Animation
{
	/*! 
	 *  The Animation System Scene
	 */
	class AnimationSystemScene : public IAnimationSystemScene
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ( )
		 */
		~AnimationSystemScene( ) { };


		/*! Default Constructor
		*
		* @return ( )
		*/
		AnimationSystemScene( ) { };


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
		void Update( const float& deltaMilliseconds );


		/*! Destroys the System Scene
		*
		*  @return (void)
		*/
		inline void Destroy( ) { };


		/*! Gets the System::Types::Type of the SystemScene
		*
		*  @return (System::Types::Type)
		*/
		inline System::Types::Type GetType( ) const { return System::Types::ANIMATION; };


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

		AnimationSystemScene( const AnimationSystemScene & copy ) { };
		AnimationSystemScene & operator = ( const AnimationSystemScene & copy ) { return *this; };

		ISystemComponent::SystemComponentMap m_components;
		
	};
};

#endif