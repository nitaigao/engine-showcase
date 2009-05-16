/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   MathUnits.hpp
*  @date   2009/04/26
*/
#ifndef MATHUNITS_H
#define MATHUNITS_H

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
}


#endif