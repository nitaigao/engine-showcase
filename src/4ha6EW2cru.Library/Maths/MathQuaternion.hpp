#ifndef __MATHQUATERNION_H
#define __MATHQUATERNION_H

#include <OgreQuaternion.h>

class MathQuaternion
{

public:

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


private:

	float _x;
	float _y;
	float _z;
	float _w;

};

#endif