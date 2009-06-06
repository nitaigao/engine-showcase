#include "InstrumentationFacade.h"

#include <luabind/luabind.hpp>
using namespace luabind;

namespace Script
{
	void InstrumentationFacade::RegisterFunctions( lua_State* state )
	{
		module( state )
		[
			class_< InstrumentationFacade >( "InstrumentationFacade" )
				.def( "getFps", &InstrumentationFacade::GetFPS )
				.def( "getRoundTime", &InstrumentationFacade::GetRoundTime ),

			class_< System::Queues::Queue >( "Queues" )
				.enum_( "constants" )
				[
					value( "LOGIC", System::Queues::LOGIC ),
					value( "RENDER", System::Queues::RENDER ),
					value( "HOUSE", System::Queues::HOUSE )
				]
		];
	}
}