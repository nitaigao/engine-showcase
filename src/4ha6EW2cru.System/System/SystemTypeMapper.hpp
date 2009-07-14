/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   SystemTypeMapper.hpp
*  @date   2009/04/25
*/
#pragma once
#ifndef SYSTEMTYPEMAPPER_H
#define SYSTEMTYPEMAPPER_H

#include <string>
#include "../Exceptions/OutOfRangeException.hpp"

namespace System
{
	/*! 
	 *  Maps Strings to System::Types::Type's
	 */
	class SystemTypeMapper
	{

	public:

		/*! Maps the given string to a System::Types::Type
		 *
		 *  @param[in] std::string systemType
		 *  @return (System::Types::Type)
		 */
		inline static System::Types::Type StringToType( const std::string& systemType )
		{
			if( systemType == System::TypeStrings::RENDERER )
			{
				return System::Types::RENDER;
			}
			else if ( systemType == System::TypeStrings::SCRIPT )
			{
				return System::Types::SCRIPT;
			}
			else if ( systemType == System::TypeStrings::GEOMETRY )
			{
				return System::Types::GEOMETRY;
			}
			else if ( systemType == System::TypeStrings::PHYSICS )
			{
				return System::Types::PHYSICS;
			}
			else if ( systemType == System::TypeStrings::INPUT )
			{
				return System::Types::INPUT;
			}
			else if ( systemType == System::TypeStrings::AI )
			{
				return System::Types::AI;
			}
			else if( systemType == System::TypeStrings::SOUND )
			{
				return System::Types::SOUND;
			}
			else if ( systemType == System::TypeStrings::ANIMATION )
			{
				return System::Types::ANIMATION;
			}
			else if ( systemType == System::TypeStrings::NETWORK )
			{
				return System::Types::NETWORK;
			}

			throw OutOfRangeException( "StringToType - Couldn't match string to System::Types::Type" );
		}

		/*! Maps the given System::Types::Type to a string
		*
		*  @param[in] System::Types::Type systemType
		*  @return (std::string)
		*/
		inline static std::string TypeToString( const System::Types::Type& systemType)
		{
			if( systemType == System::Types::RENDER )
			{
				return System::TypeStrings::RENDERER;
			}
			else if ( systemType == System::Types::SCRIPT )
			{
				return System::TypeStrings::SCRIPT;
			}
			else if ( systemType == System::Types::GEOMETRY )
			{
				return System::TypeStrings::GEOMETRY;
			}
			else if ( systemType == System::Types::PHYSICS )
			{
				return System::TypeStrings::PHYSICS;
			}
			else if ( systemType == System::Types::INPUT )
			{
				return System::TypeStrings::INPUT;
			}
			else if ( systemType == System::Types::AI )
			{
				return System::TypeStrings::AI;
			}
			else if( systemType == System::Types::SOUND )
			{
				return System::TypeStrings::SOUND;
			}
			else if ( systemType == System::Types::ANIMATION )
			{
				return System::TypeStrings::ANIMATION;
			}
			else if ( systemType == System::Types::NETWORK )
			{
				return System::TypeStrings::NETWORK;
			}
				
			throw OutOfRangeException( "Couldn't match string to System::Types::Type" );
		}
	};
};

#endif