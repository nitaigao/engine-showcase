/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IAISystemScene.hpp
*  @date   2009/04/27
*/

#ifndef IAISYSTEMSCENE_H
#define IAISYSTEMSCENE_H

#include "System/ISystemScene.hpp"

namespace AI
{
	/*! 
	 *  An AI System Scene
	 */
	class IAISystemScene : public ISystemScene
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IAISystemScene( ) { };


		/*! Returns all way points in the Scene
		*
		* @return ( ISystemComponent::SystemComponentList )
		*/
		virtual ISystemComponent::SystemComponentList GetWaypoints( ) = 0;

		/*! Returns the Navigation Mesh for the Scene
		*
		* @return ( INavigationMesh* )
		*/
		virtual ISystemComponent* GetNavigationMesh( ) = 0;

	};
};

#endif