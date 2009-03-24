#ifndef __MATHVECTOR3_H
#define __MATHVECTOR3_H

#include <OgreVector3.h>
#include <Common/Base/hkBase.h>

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

	Ogre::Vector3 AsOgreVector3( ) { return Ogre::Vector3( _x, _y, _z ); };
	hkVector4 AshkVector4( ) { return hkVector4( _x, _y, _z ); };

	MathVector3 operator + ( const MathVector3& input )
	{
		return MathVector3(
			input._x + _x,
			input._y + _y,
			input._z + _z
			);
	};

	MathVector3 operator += ( const MathVector3& input )
	{
		return *this + input;
	}

	static MathVector3 Up( ) { return MathVector3( 0.0f, 1.0f, 0.0f ); };
	static MathVector3 Forward( ) { return MathVector3( 0.0f, 1.0f, 0.0f ); };

private:

	float _x, _y, _z;

};

#endif