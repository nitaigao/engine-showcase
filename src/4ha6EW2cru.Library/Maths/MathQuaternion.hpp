#ifndef __MATHQUATERNION_H
#define __MATHQUATERNION_H

#include <OgreMatrix3.h>
#include <OgreQuaternion.h>

#include <Common/Base/hkBase.h>

class MathQuaternion
{

public:

	virtual ~MathQuaternion( ) { };

	MathQuaternion( )
		: _x( 0 )
		, _y( 0 )
		, _z( 0 )
		, _w( 1 )
	{

	}

	MathQuaternion( float x, float y, float z, float w )
		: _x( x )
		, _y( y )
		, _z( z )
		, _w( w )
	{

	}

	Ogre::Quaternion AsOgreQuaternion( ) { return Ogre::Quaternion( _w, _x, _y, _z ); };

	hkQuaternion AshkQuaternion( ) 
	{ 
		return hkQuaternion( _x, _y, _z, _w ); 
	};
	
	MathQuaternion Normalize( )
	{
		float length = sqrt( 
			_x * _x + _y * _y +
			_z * _z + _w * _w
			);

		return MathQuaternion(
			_x /= length,
			_y /= length,
			_z /= length,
			_w /= length
			);
	};

	MathQuaternion operator * ( const MathQuaternion& input )
	{
		return MathQuaternion(
			_w * input._x + _x * input._w + _y * input._z - _z * input._y,
			_w * input._y + _y * input._w + _z * input._x - _x * input._z,
			_w * input._z + _z * input._w + _x * input._y - _y * input._x,
			_w * input._w - _x * input._x - _y * input._y - _z * input._z
		);
	}



private:

	float _x;
	float _y;
	float _z;
	float _w;

};

#endif