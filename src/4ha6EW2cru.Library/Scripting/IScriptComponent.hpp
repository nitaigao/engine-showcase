/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IScriptComponent.hpp
*  @date   2009/04/27
*/
#ifndef ISCRIPTCOMPONENT_HPP
#define ISCRIPTCOMPONENT_HPP

#include "../System/ISystemComponent.hpp"

extern "C" 
{
#	include <lua.h>
#	include <lualib.h>
}

namespace Script
{
	/*!
	 *  A Script System Component 
	 */
	class IScriptComponent : public ISystemComponent
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IScriptComponent( ) { };


		/*! Steps the internal data of the Component
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		virtual void Update( const float& deltaMilliseconds ) = 0;


		/*! Returns the LUA state of the Component
		 *
		 *  @return (lua_State*)
		 */
		virtual lua_State* GetState( ) const = 0;

		/*! Executes the Loaded Script
		*
		*  @return (void)
		*/
		virtual void Execute( ) = 0;

		/*! Posts a message to the parent Entity
		*
		*  @param[in] const std::string & message
		*  @param[in] AnyValue::AnyValueMap parameters
		*  @return (AnyValue)
		*/
		virtual AnyValue PostMessage( const std::string& message, AnyValue::AnyValueMap parameters ) = 0;

	};
};

#endif