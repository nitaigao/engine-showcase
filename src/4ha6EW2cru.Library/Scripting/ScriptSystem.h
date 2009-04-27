/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   ScriptSystem.h
*  @date   2009/04/27
*/
#ifndef __SCRIPTSYSTEM_H
#define __SCRIPTSYSTEM_H

#include "../Service/IService.hpp"
#include "../Configuration/IConfiguration.hpp"

#include "IScriptSystem.hpp"
#include "IScriptSystemScene.hpp"

namespace Script
{
	/*!
	 *  The Scripting System 
	 */
	class ScriptSystem : public IScriptSystem, public IService
	{

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		~ScriptSystem( ) { };


		/*! Default Constructor
		*
		*  @return ()
		*/
		ScriptSystem( Configuration::IConfiguration* configuration )
			: _configuration( configuration )
		{

		}


		/*! Initializes the System
		*
		*  @return (void)
		*/
		void Initialize( );


		/*! Steps the System's internal data
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		void Update( const float& deltaMilliseconds );


		/*! Releases internal data of the System
		*
		*  @return (System::Types::Type)
		*/
		void Release( ) { };


		/*! Returns the type of the System
		*
		*  @return (System::Types::Type)
		*/
		inline System::Types::Type GetType( ) const { return System::Types::SCRIPT; };


		/*! Creates a System Scene
		*
		*  @return (ISystemScene*)
		*/
		ISystemScene* CreateScene( );


		/*! Gets the System's Properties
		*
		*  @return (AnyValueMap)
		*/
		inline AnyValue::AnyValueMap GetProperties( ) const { return _properties; };


		/*! Sets a System Property
		*
		*  @param[in] const std::string & name
		*  @param[in] AnyValue value
		*  @return (void)
		*/
		inline void SetProperty( const std::string& name, AnyValue value ) { };


		/*! Scripting Service Interface
		*
		*  @param[in] const std::string & actionName
		*  @param[in] AnyValue::AnyValueMap & parameters
		*  @return (AnyValue::AnyValueMap)
		*/
		AnyValue::AnyValueMap Execute( const std::string& actionName, AnyValue::AnyValueMap& parameters );

	private:

		ScriptSystem( const ScriptSystem & copy ) { };
		ScriptSystem & operator = ( const ScriptSystem & copy ) { return *this; };

		AnyValue::AnyValueMap _properties;
		Configuration::IConfiguration* _configuration;

		IScriptSystemScene* _scene;

	};
};

#endif