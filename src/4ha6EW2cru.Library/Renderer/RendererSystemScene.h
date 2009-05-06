/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   RendererSystemScene.h
*  @date   2009/04/27
*/
#ifndef __RENDERERSYSTEMSCENE_H
#define __RENDERERSYSTEMSCENE_H

#include <Ogre.h>

#include "IRenderSystemScene.h"
#include "IRendererSystemComponent.hpp"

namespace Renderer
{
	/*!
	 *  A Render System Scene 
	 */
	class RendererSystemScene : public IRenderSystemScene
	{

		typedef std::vector< IRendererSystemComponent* > RendererSystemComponentList;

	public:


		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~RendererSystemScene( ) { };


		/*! Default Constructor
		 *
		 *  @param[in] Ogre::SceneManager * sceneManager
		 *  @return ()
		 */
		RendererSystemScene( Ogre::SceneManager* sceneManager )
			: _sceneManager( sceneManager )
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
		inline System::Types::Type GetType( ) const { return System::Types::RENDER; };


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


		/*! Returns the Ogre Scene Manager containing the Scene
		 *
		 *  @return (Ogre::SceneManager*)
		 */
		Ogre::SceneManager* GetSceneManager( ) const { return _sceneManager; };


		/*! Returns a Render Scene Component of the given name
		 *
		 *  @param[in] const std::string & name
		 *  @return (IRendererSystemComponent*)
		 */
		IRendererSystemComponent* GetComponent( const std::string& name ) const;

	private:

		Ogre::SceneManager* _sceneManager;
		RendererSystemComponentList _components;

		RendererSystemScene( ) { };
		RendererSystemScene( const RendererSystemScene & copy ) { };
		RendererSystemScene & operator = ( const RendererSystemScene & copy ) { return *this; };

	};
};

#endif