#ifndef __MATHVECTOR3_H
#define __MATHVECTOR3_H

#include <OgreVector3.h>
#include <Common/Base/hkBase.h>

#include "MathMatrix.hpp"

class MathVector3
{

public:

	~MathVector3( ) { };

	MathVector3( )
		: _x( 0 )
		, _y( 0 )
		, _z( 0 )
	{

	}

	MathVector3( float x, float y, float z )
		: _x( x )
		, _y( y )
		, _z( z )
	{

	}

	MathVector3( Ogre::Vector3 vector )
		: _x( vector.x )
		, _y( vector.y )
		, _z( vector.z )
	{

	}

	inline float GetX( ) { return _x; };
	inline float GetY( ) { return _y; };
	inline float GetZ( ) { return _z; };

	inline Ogre::Vector3 AsOgreVector3( ) { return Ogre::Vector3( _x, _y, _z ); };
	inline hkVector4 AshkVector4( ) { return hkVector4( _x, _y, _z ); };

	inline float DotProduct( const MathVector3& input )
	{
		return _x * input._x + _y * input._y + _z * input._z;
	}

	inline float Length( )
	{
		return sqrt( _x * _x + _y * _y + _z * _z );
	}

	inline MathVector3 CrossProduct( const MathVector3& input )
	{
		return MathVector3(
			_y * input._z - _z * input._y,
			_z * input._x - _x * input._z,
			_x * input._y - _y * input._x
			);
	}

	inline MathVector3 Normalize( )
	{
		float length = sqrt( _x * _x + _y * _y + _z * _z  );

		MathVector3 result( _x, _y, _z );
		
		if ( length > 0.0f )
		{
			result = result / length;
		}

		return result;
	}

	inline MathVector3 operator / ( const float& real )
	{
		return MathVector3(
			_x /= real,
			_y /= real,
			_z /= real
			);
	}

	inline MathVector3 operator + ( const MathVector3& input )
	{
		return MathVector3(
			input._x + _x,
			input._y + _y,
			input._z + _z
			);
	};

	inline MathVector3 operator += ( const MathVector3& input )
	{
		return *this + input;
	};

	inline MathVector3 operator - ( const MathVector3& input )
	{
		return MathVector3(
			input._x - _x,
			input._y - _y,
			input._z - _z
			);
	};

	inline MathVector3 operator * ( const float& scalar )
	{
		return MathVector3(
			scalar * _x,
			scalar * _y,
			scalar * _z
			);
	};

	inline MathVector3 operator * ( const MathVector3& input )
	{
		return MathVector3(
			_x * input._x,
			_y * input._y,
			_z * input._z
			);
	};

	inline MathVector3 operator * ( const MathMatrix& input )
	{
		return this->AsOgreVector3( ) * input;
	}

	static inline MathVector3 Up( ) { return MathVector3( 0.0f, 1.0f, 0.0f ); };
	static inline MathVector3 Forward( ) { return MathVector3( 0.0f, 0.0f, 1.0f ); };
	static inline MathVector3 Zero( ) { return MathVector3( 0.0f, 0.0f, 0.0f ); };

private:

	float _x, _y, _z;

};

#endif