/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IAISystem.hpp
*  @date   2009/04/27
*/
#ifndef IAISYSTEM_H
#define IAISYSTEM_H

#include "../System/ISystem.hpp"

namespace AI
{
	/*! 
	 *  The AI System
	 */
	class IAISystem : public ISystem
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IAISystem( ) { };

	};
};

#endif
