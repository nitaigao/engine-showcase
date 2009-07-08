/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IRendererSystem.hpp
*  @date   2009/04/27
*/
#ifndef IRENDERERSYSTEM_HPP
#define IRENDERERSYSTEM_HPP

#include "System/ISystem.hpp"

namespace Renderer
{
	/*!
	 *  The Render System 
	 */
	class IRendererSystem : public ISystem
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IRendererSystem( ) { };

	};
};

#endif