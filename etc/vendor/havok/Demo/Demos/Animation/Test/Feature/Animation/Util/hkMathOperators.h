/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_VECTOR4_OPERATIONS_H
#define HK_VECTOR4_OPERATIONS_H

inline static hkVector4 operator + ( const hkVector4& a, const hkVector4& b )
{
	hkVector4 tmp;
	tmp.setAdd4( a, b );
	return tmp;
}

inline static hkVector4 operator - ( const hkVector4& a, const hkVector4& b )
{
	hkVector4 tmp;
	tmp.setSub4( a, b );
	return tmp;
}

inline static hkVector4 operator + ( const hkVector4& a )
{
	return a;
}

inline static hkVector4 operator - ( const hkVector4& a )
{
	return hkVector4( -a(0), -a(1), -a(2), -a(3) );
}

inline static hkVector4 operator * ( const hkVector4& a, const hkVector4& b )
{
	hkVector4 tmp;
	tmp.setMul4( a, b );
	return tmp;
}

inline static hkVector4 operator * ( const hkVector4& a, hkReal b )
{
	hkVector4 tmp;
	tmp.setMul4( b, a );
	return tmp;
}

inline static hkVector4 operator / ( const hkVector4& a, hkReal b )
{
	hkVector4 tmp;
	tmp.setMul4( 1.0f / b, a );
	return tmp;
}

inline static hkVector4 operator * ( const hkReal& b, const hkVector4& a )
{
	hkVector4 tmp;
	tmp.setMul4( b, a );
	return tmp;
}

inline static hkVector4 operator / ( const hkReal& b, const hkVector4& a )
{
	hkVector4 tmp;
	tmp.setMul4( 1.0f / b, a );
	return tmp;
}

inline static hkReal dot( const hkVector4& a, const hkVector4& b )
{
	return a.dot3( b );
}

inline static hkVector4 operator * ( const hkQsTransform& transform, const hkVector4& vec )
{
	hkVector4 tmp;
	tmp.setTransformedPos( transform, vec );
	return tmp;
}

inline static hkQsTransform operator * ( const hkQsTransform& a, const hkQsTransform& b )
{
	hkQsTransform tmp;
	tmp.setMul( a, b );
	return tmp;
}

inline static hkVector4 cross( const hkVector4& a, const hkVector4& b )
{
	hkVector4 tmp;
	tmp.setCross( a, b );
	return tmp;
}

inline static hkQsTransform inverse( const hkQsTransform& T )
{
	hkQsTransform tmp;
	tmp.setInverse( T );
	return tmp;
}

inline static hkMatrix3 inverse( const hkMatrix3& A )
{
	hkMatrix3 B;
	B = A;
	B.invert( 0.0f );
	return B;
}

inline static hkVector4 operator * ( const hkMatrix3& A, const hkVector4& v )
{
	hkVector4 tmp;
	tmp.setMul3( A, v );
	return tmp;
}

inline static hkVector4 fmod( const hkVector4& v, hkReal r )
{
	return hkVector4( fmod( v( 0 ), r ), fmod( v( 1 ), r ), fmod( v( 2 ), r ), fmod( v( 3 ), r ) );
}

inline static hkVector4 fdiv( const hkVector4& v, hkReal r )
{
	return v - fmod( v, r );
}

inline static hkVector4& operator += ( hkVector4& u, const hkVector4& v )
{
	u = u + v;
	return u;
}

inline static hkVector4& operator -= ( hkVector4& u, const hkVector4& v )
{
	u = u - v;
	return u;
}

inline static hkVector4& operator *= ( hkVector4& u, const hkVector4& v )
{
	u = u * v;
	return u;
}

#endif // HK_VECTOR4_OPERATIONS_H

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20090216)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2009
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available at www.havok.com/tryhavok.
* 
*/
