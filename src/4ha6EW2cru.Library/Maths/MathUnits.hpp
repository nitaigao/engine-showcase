/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   MathUnits.hpp
*  @date   2009/04/26
*/
#pragma once
#ifndef MATHUNITS_H
#define MATHUNITS_H

#include <Rpc.h>

namespace Maths
{
	static const float PI = 4.0 * atan( 1.0 );

	/*! Clamps the input value to within the region of the min & max values
	 *
	 *  @param[in] T input
	 *  @param[in] T min
	 *  @param[in] T max
	 *  @return (T)
	 */
	template< class T >
	static inline T Clamp( const T& input, const T& min, const T& max )
	{
		if ( input < min ) return min;
		if ( input > max ) return max;
		return input;
	}


	/*! Rounds the given number to the nearest whole number
	*
	* @param[in] T value
	* @return ( T )
	*/
	template< class T >
	static inline T Round( T value )
	{
		T integral = 0;
		return ( modf( value, &integral ) > 0.5 ) ? ceil( value ) : floor( value );
	}


	/*! Generates a unique UUID
	*
	* @return ( std::string )
	*/
	static std::string GenUUID( )
	{
		UUID uuid;

		ZeroMemory( &uuid, sizeof( UUID ) );

		UuidCreate( &uuid );

		RPC_CSTR uuidChar;
		UuidToString( &uuid, &uuidChar );

		std::stringstream uuidString;
		uuidString << uuidChar;

		return uuidString.str( );
	}
}


#endif