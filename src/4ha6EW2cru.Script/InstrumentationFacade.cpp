#include "InstrumentationFacade.h"

using namespace luabind;

namespace Script
{
	scope InstrumentationFacade::RegisterFunctions( )
	{
		return (
			class_< InstrumentationFacade >( "InstrumentationFacade" )
				.def( constructor< >( ) )
				.def( "getFps", &InstrumentationFacade::GetFPS )
				.def( "getRoundTime", &InstrumentationFacade::GetRoundTime ),

			class_< System::Queues::Queue >( "Queues" )
				.def( constructor< >( ) )
				.enum_( "constants" )
				[
					value( "LOGIC", System::Queues::LOGIC ),
					value( "RENDER", System::Queues::RENDER ),
					value( "HOUSE", System::Queues::HOUSE )
				]
		);
	}
}