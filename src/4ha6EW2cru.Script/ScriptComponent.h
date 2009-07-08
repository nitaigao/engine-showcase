/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   ScriptComponent.h
*  @date   2009/04/27
*/
#ifndef SCRIPTCOMPONENT_H
#define SCRIPTCOMPONENT_H

#include "IScriptComponent.hpp"
#include "Events/IEvent.hpp"

#include "IScriptFacade.hpp"
#include "IScriptFunctionHandler.hpp"

#include <luabind/luabind.hpp>

namespace Script
{
	/*!
	 *  A Script System Component 
	 */
	class ScriptComponent : public IScriptComponent
	{

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		~ScriptComponent( ) { };


		/*! Default Constructor
		*
		*  @param[in] const std::string & name
		*  @return ()
		*/
		ScriptComponent( lua_State* state )
			: m_state( state )
			, m_observer( 0 )
			, m_eventHandlers( 0 )
		{

		};


		/*! Initializes the Component
		*
		*  @param[in] AnyType::AnyValueMap properties
		*  @return (void)
		*/
		void Initialize( );


		/*! Steps the internal data of the Component
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		void Update( const float& deltaMilliseconds );


		/*! Destroys the Component
		*
		*  @return (void)
		*/
		void Destroy( );


		/*! Adds an Observer to the Component
		*
		*  @param[in] IObserver * observer
		*  @return (void)
		*/
		inline void AddObserver( IObserver* observer ) { m_observer = observer; };


		/*! Pushes any Changes to the Observers
		*
		*  @param[in] const unsigned int& systemChanges
		*  @return (void)
		*/
		void PushChanges( const unsigned int& systemChanges );


		/*! Gets the properties of the Component
		*
		*  @return (AnyTypeKeyMap)
		*/
		AnyType::AnyTypeMap GetAttributes( ) const { return m_attributes; };


		/*! Sets an Attribute on the Component *
		*
		*  @param[in] const unsigned int attributeId
		*  @param[in] const AnyType & value
		*/
		inline void SetAttribute( const System::Attribute& attributeId, const AnyType& value ) { m_attributes[ attributeId ] = value; };


		/*! Messages the Component to influence its internal state
		*
		*  @param[in] const std::string & message
		*  @return (AnyType)
		*/
		AnyType Message( const System::Message& message, AnyType::AnyTypeMap parameters );


		/*! Posts a message to the parent Entity
		 *
		 *  @param[in] const std::string & message
		 *  @param[in] AnyType::AnyValueMap parameters
		 *  @return (AnyType)
		 */
		AnyType PushMessage( const System::Message& message, AnyType::AnyTypeMap parameters ) { return m_observer->Message( message, parameters ); };


		/*! Returns the LUA state of the Component
		*
		*  @return (lua_State*)
		*/
		inline lua_State* GetState( ) const { return m_state; };


		/*! Generic Event Handler to Forward Game Events to the Script
		 *
		 *  @param[in] const Events::IEvent * event
		 *  @return (void)
		 */
		void OnEvent( const Events::IEvent* event );


		/* Script Handlers */

		/*! Runs the Loaded Script
		 *
		 *  @return (void)
		 */
		void RunScript( );


		/*! Loads a Script From the File System
		 *
		 *  @param[in] const std::string & scriptPath
		 *  @return (void)
		 */
		void LoadScript( const std::string& scriptPath );


		/*! Includes a script into the current LUA State
		 *
		 *  @param[in] const std::string & scriptPath
		 *  @return (void)
		 */
		void IncludeScript( const std::string& scriptPath );


		/*! Registers an LUA function to receive in Game Events
		 *
		 *  @param[in] luabind::object function
		 *  @return (void)
		 */
		void RegisterEvent( const luabind::object& function );


		/*! UnRegisters an LUA function from receiving in Game Events
		 *
		 *  @param[in] luabind::object function
		 *  @return (void)
		 */
		void UnRegisterEvent( const luabind::object& function );


		/*!  Registers an LUA function to be included in the Game Update Loop
		 *
		 *  @param[in] luabind::object function
		 *  @return (void)
		 */
		void RegisterUpdate( const luabind::object& function );


		/*! UnRegisters an LUA function from being included in the Game Update Loop
		*
		*  @param[in] luabind::object function
		*  @return (void)
		*/
		void UnRegisterUpdate( const luabind::object& function );


		/*! Executed the given string as LUA code against the components LUA State
		 *
		 *  @param[in] const std::string & input
		 *  @return (void)
		 */
		void ExecuteString( const std::string& input );

		/*! Gets the Name of the Component
		*
		*  @return (const std::string&)
		*/
		inline std::string GetName( ) { return m_attributes[ System::Attributes::Name ].As< std::string >( ); };

		/*! Broadcasts an Event to the LUA State with no parameters
		 *
		 *  @param[in] const std::string & eventName
		 *  @return (void)
		 */
		void BroadcastEvent( const std::string& eventName );


		/*! Broadcasts an Event to the LUA State with parameters
		 *
		 *  @param[in] const std::string & eventName
		 *  @param[in] const std::string & var1
		 *  @return (void)
		 */
		void BroadcastEvent( const std::string& eventName, const std::string& var1 );


		/*! Broadcasts an Event to the LUA State with parameters
		 *
		 *  @param[in] const std::string & eventName
		 *  @param[in] const int & var1
		 *  @return (void)
		 */
		void BroadcastEvent( const std::string& eventName, const int& var1 );



		/*! Broadcasts an Event to the LUA State with parameters
		 *
		 *  @param[in] const std::string & eventName
		 *  @param[in] const std::string & var1
		 *  @param[in] const std::string & var2
		 *  @return (void)
		 */
		void BroadcastEvent( const std::string& eventName, const std::string& var1, const std::string& var2 );
		
		
		
		/*! Broadcasts an Event to the LUA State with parameters
		 *
		 *  @param[in] const std::string & eventName
		 *  @param[in] const std::string & var1
		 *  @param[in] const int & var2
		 *  @return (void)
		 */
		void BroadcastEvent( const std::string& eventName, const std::string& var1, const int& var2 );
		
		
		/*! Broadcasts an Event to the LUA State with parameters
		 *
		 *  @param[in] const std::string & eventName
		 *  @param[in] const int & var1
		 *  @param[in] const std::string & var2
		 *  @return (void)
		 */
		void BroadcastEvent( const std::string& eventName, const int& var1, const std::string& var2 );
		
		
		/*! Broadcasts an Event to the LUA State with parameters
		 *
		 *  @param[in] const std::string & eventName
		 *  @param[in] const int & var1
		 *  @param[in] const int & var2
		 *  @return (void)
		 */
		void BroadcastEvent( const std::string& eventName, const int& var1, const int& var2 );


		/*! Performs a Ray Query
		 *
		 *  @param[in] bool sortByDistance
		 *  @param[in] int maxResults
		 *  @return (std::vector< std::string >)
		 */
		std::vector< std::string > RayQuery( Maths::MathVector3 origin, Maths::MathVector3 direction, const float& length, const bool& sortByDistance, const int& maxResults );


		/*! Returns the system time in Milliseconds
		 *
		 *  @return (float)
		 */
		float GetTime( ) const;

		inline Maths::MathVector3 GetLookAt( ) const { return m_lookAt; };

		inline Maths::MathVector3 GetPosition( ) { return m_attributes[ System::Attributes::Position ].As< Maths::MathVector3 >( ); };

	private:

		ScriptComponent( const ScriptComponent & copy ) { };
		ScriptComponent & operator = ( const ScriptComponent & copy ) { return *this; };

		lua_State* m_state;

		IScriptFunctionHandler::FunctionList m_eventHandlers;
		IScriptFunctionHandler::FunctionList m_updateHandlers;

		IScriptFacade::ScriptControllerList m_facades;

		IObserver* m_observer;

		AnyType::AnyTypeMap m_attributes;

		Maths::MathVector3 m_lookAt;

	};
};

#endif