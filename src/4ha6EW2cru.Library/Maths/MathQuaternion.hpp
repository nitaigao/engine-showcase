#ifndef __MATHQUATERNION_H
#define __MATHQUATERNION_H

#include <OgreMatrix3.h>
#include <OgreQuaternion.h>

#include "MathVector3.hpp"

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

	MathQuaternion( MathVector3 axis, float angle )
	{
		_w = cos( 0.5 * angle );
		_x = sin( 0.5 * angle ) * axis.GetX( ); 
		_y = sin( 0.5 * angle ) * axis.GetY( ); 
		_z = sin( 0.5 * angle ) * axis.GetZ( ); 

	}

	MathQuaternion( float x, float y, float z, float w )
		: _x( x )
		, _y( y )
		, _z( z )
		, _w( w )
	{

	}

	MathQuaternion( const Ogre::Quaternion& quaternion )
		: _x( quaternion.x )
		, _y( quaternion.y )
		, _z( quaternion.z )
		, _w( quaternion.w )
	{

	}

	Ogre::Quaternion AsOgreQuaternion( ) { return Ogre::Quaternion( _w, _x, _y, _z ); };
	hkQuaternion AshkQuaternion( ) { return hkQuaternion( _x, _y, _z, _w ); };
	
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
	};

	static MathQuaternion Identity( ) { return MathQuaternion( ); }; 

private:

	float _x;
	float _y;
	float _z;
	float _w;

};

#endif