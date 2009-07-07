#pragma once
#ifndef INSTRUMENTATIONFACADE_HPP
#define INSTRUMENTATIONFACADE_HPP

#include <luabind/luabind.hpp>
#include "../System/SystemType.hpp"
#include "../Management/Management.h"
#include "IScriptFacade.hpp"

namespace Script
{
	class InstrumentationFacade : public IScriptFacade
	{

	public:

		/*! Registers the Script functions with the given state
		*
		* @param[in] lua_State * state
		* @return ( void )
		*/
		static void RegisterFunctions( lua_State* state );


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

	};
};

#endif