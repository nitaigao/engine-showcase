#ifndef __MATHUNITS_H
#define __MATHUNITS_H

namespace Maths
{

	static const float PI = 4.0 * atan( 1.0 );

	template< class T >
	static inline  T Clamp( T x, T mi, T ma)
	{
		if ( x < mi ) return mi;
		if ( x > ma ) return ma;
		return x;
	}

}


#endif