/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IRenderSystemScene.h
*  @date   2009/04/27
*/
#ifndef __IRENDERSYSTEMSCENE_H
#define __IRENDERSYSTEMSCENE_H

#include "../System/ISystemScene.hpp"
#include "IRendererSystemComponent.hpp"

namespace Renderer
{
	/*!
	 *  A Render System Scene 
	 */
	class IRenderSystemScene : public ISystemScene
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IRenderSystemScene( ) { };


		/*! Returns the Ogre Scene Manager containing the Scene
		*
		*  @return (Ogre::SceneManager*)
		*/
		virtual Ogre::SceneManager* GetSceneManager( ) const = 0;


		/*! Gets the specified component from the scene
		 *
		 *  @param[in] const std::string & name
		 *  @return (IRendererSystemComponent*)
		 */
		virtual IRendererSystemComponent* GetComponent( const std::string& name ) const = 0;

	};
};

#endif