/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   InstrumentationFacade.h
*  @date   2009/07/08
*/
#pragma once
#ifndef INSTRUMENTATIONFACADE_H
#define INSTRUMENTATIONFACADE_H

#include <luabind/luabind.hpp>
#include "System/SystemType.hpp"
#include "Management/Management.h"
#include "IScriptFacade.hpp"

namespace Script
{
	/*! 
	 *  A Facade interface to the Instrumentation Provider
	 */
	class InstrumentationFacade : public IScriptFacade
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~InstrumentationFacade( ) { };


		/*! Default Constructor
		*
		* @return (  )
		*/
		InstrumentationFacade( ) { };


		/*! Registers the Script functions with the given state
		*
		* @param[in] lua_State * state
		* @return ( void )
		*/
		static luabind::scope  RegisterFunctions( );


		/*! Gets the FPS Statistic for the Game
		*
		* @return ( int )
		*/
		inline int GetFPS( ) { return Management::Get( )->GetInstrumentation( )->GetFPS( ); };


		/*! Returns the Round Time for the given Queue
		*
		* @param[in] const System::Queues::Queue & queue
		* @return ( float )
		*/
		inline float GetRoundTime( const System::Queues::Queue& queue ) { return Management::Get( )->GetInstrumentation( )->GetRoundTime( queue ); };

	private:

		InstrumentationFacade( const InstrumentationFacade & copy ) { };
		InstrumentationFacade & operator = ( const InstrumentationFacade & copy ) { return *this; };
		
	};
};

#endif