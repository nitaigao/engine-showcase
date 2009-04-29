/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   ScriptFunctionHandler.hpp
*  @date   2009/04/28
*/
#ifndef __SCRIPTGENERICHANDLER_HPP
#define __SCRIPTGENERICHANDLER_HPP

#include "IScriptFunctionHandler.hpp"

namespace Script
{
	/*!
	 *  A Container for handling Updates or Events from the Game 
	 */
	class ScriptFunctionHandler : public IScriptFunctionHandler
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~ScriptFunctionHandler( ) { };

		/*! Default Constructor
		 *
		 *  @return ()
		 */
		ScriptFunctionHandler( const luabind::object& functionHandler )
			: _functionHandler( functionHandler )
			, _isMarkedForDeletion( false )
		{

		}


		/*! Marks the Handler for deletion on the next UX Update or Event Tick
		 *
		 *  @return (void)
		 */
		inline void MarkForDeletion( ) { _isMarkedForDeletion = true; };


		/*! Returns whether or not the handler is marked for deletion on the next UX Update or Event Tick
		 *
		 *  @return (bool)
		 */
		inline bool IsMarkedForDeletion( ) const { return _isMarkedForDeletion; };


		/*! Returns the LUA function
		*
		*  @return (luabind::object)
		*/
		inline luabind::object GetFunction( ) const { return _functionHandler; };

	private:

		bool _isMarkedForDeletion;
		luabind::object _functionHandler;

		ScriptFunctionHandler( const ScriptFunctionHandler & copy ) { };
		ScriptFunctionHandler & operator = ( const ScriptFunctionHandler & copy ) { return *this; };

	};
};

#endif