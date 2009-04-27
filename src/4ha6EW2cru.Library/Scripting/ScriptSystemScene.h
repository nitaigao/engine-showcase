/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   ScriptSystemScene.h
*  @date   2009/04/27
*/
#ifndef __SCRIPTSYSTEMSCENE_H
#define __SCRIPTSYSTEMSCENE_H

#include <luabind/luabind.hpp>

#include "../Configuration/IConfiguration.hpp"
#include "../Events/IEvent.hpp"

#include "ScriptConfiguration.h"
#include "IScriptSystemScene.hpp"
#include "IScriptComponent.hpp"

namespace Script
{
	/*!
	 *  A Script System Scene 
	 */
	class ScriptSystemScene : public IScriptSystemScene
	{

		typedef std::pair< Events::EventType, luabind::object > EventHandler;
		typedef std::vector< EventHandler > EventHandlerList;
		typedef std::vector< IScriptComponent* > ScriptComponentList;

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		~ScriptSystemScene( );

		
		/*! Default Constructor
		 *
		 *  @param[in] Configuration::IConfiguration * configuration
		 *  @return ()
		 */
		ScriptSystemScene( Configuration::IConfiguration* configuration );


		/*! Initializes the System Scene
		*
		*  @return (void)
		*/
		void Initialize( );

		/*! Steps internal data of the SystemScene
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		inline void Update( const float& deltaMilliseconds );


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


		/*! Gets the System::Types::Type of the SystemScene
		*
		*  @return (System::Types::Type)
		*/
		inline System::Types::Type GetType( ) const { return System::Types::SCRIPT; };


		/*! Returns the Master LUA state of the Scene
		*
		*  @return (lua_State*)
		*/
		lua_State* GetState( ) const { return _state; };

		
		/*!  Prints the specified message to the console
		 *
		 *  @param[in] const std::string & message
		 *  @return (void)
		 */
		static void Print( const std::string& message );

		
		/*! Quits the game
		 *
		 *  @return (void)
		 */
		static void Quit( );

		
		/*! Loads the specified level
		 *
		 *  @param[in] const std::string & levelName
		 *  @return (void)
		 */
		static void LoadLevel( const std::string& levelName );

		
		/*! Ends the current Game
		 *
		 *  @return (void)
		 */
		static void EndGame( );

	private:

		ScriptSystemScene( ) { };
		ScriptSystemScene( const ScriptSystemScene & copy ) { };
		ScriptSystemScene & operator = ( const ScriptSystemScene & copy ) { return *this; };

		static int Script_PError( lua_State* luaState );
		static void Script_Error( lua_State* luaState );
		static void Script_CastError( lua_State* luaState, LUABIND_TYPE_INFO typeInfo );

		void OnEvent( const Events::IEvent* event );

		ScriptConfiguration* _scriptConfiguration;

		lua_State* _state;
		ScriptComponentList _components;
		EventHandlerList* _eventHandlers;

	};
};

#endif