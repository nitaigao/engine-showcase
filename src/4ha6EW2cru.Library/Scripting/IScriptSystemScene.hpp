/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IScriptSystemScene.hpp
*  @date   2009/04/27
*/
#ifndef ISCRIPTSYSTEMSCENE_HPP
#define ISCRIPTSYSTEMSCENE_HPP

#include "../System/ISystemScene.hpp"
#include "../System/ISystemComponent.hpp"

extern "C" 
{
#	include <lua.h>
}

namespace Script
{
	/*!
	 *  A Script System Scene 
	 */
	class IScriptSystemScene : public ISystemScene
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IScriptSystemScene( ) { };

		
		/*! Returns the Master LUA state of the Scene
		 *
		 *  @return (lua_State*)
		 */
		virtual lua_State* GetState( ) const = 0;


		/*! Finds a Component within the Scene
		 *
		 *  @param[in] const std::string & name
		 *  @return (IScriptComponent*)
		 */
		virtual ISystemComponent* FindComponent( const std::string& name ) const = 0;

	};
};

#endif