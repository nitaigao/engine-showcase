/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IRendererSystemComponent.hpp
*  @date   2009/04/27
*/
#ifndef IRENDERERSYSTEMCOMPONENT_HPP
#define IRENDERERSYSTEMCOMPONENT_HPP

#include "System/ISystemComponent.hpp"

namespace Renderer
{
	/*!
	 *  A Render System Component 
	 */
	class IRendererSystemComponent : public ISystemComponent
	{

	public:

		typedef std::map< std::string, IRendererSystemComponent* > RendererSystemComponentList;

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IRendererSystemComponent( ) { };

	};
};

#endif