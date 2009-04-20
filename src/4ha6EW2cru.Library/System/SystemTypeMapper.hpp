#ifndef __SYSTEMTYPEMAPPER_H
#define __SYSTEMTYPEMAPPER_H

#include <string>
#include "../Exceptions/OutOfRangeException.hpp"

namespace System
{
	class TypeMapper
	{

	public:

		static SystemType StringToType( std::string systemType )
		{
			SystemType returnType = Max_SystemTypes;

			if( systemType == "graphics" )
			{
				return RenderSystemType;
			}
			else if ( systemType == "script" )
			{
				return ScriptSystemType;
			}
			else if ( systemType == "geometry" )
			{
				return GeometrySystemType;
			}
			else if ( systemType == "physics" )
			{
				return PhysicsSystemType;
			}
			else if ( systemType == "input" )
			{
				return InputSystemType;
			}
			else if ( systemType == "ai" )
			{
				return AISystemType;
			}

			if ( returnType == Max_SystemTypes )
			{
				throw OutOfRangeException( "Couldn't match string to SystemType" );
			}
		}
	};
};

#endif