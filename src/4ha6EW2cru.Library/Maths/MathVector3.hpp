#ifndef __MATHVECTOR3_H
#define __MATHVECTOR3_H

#include <OgreVector3.h>

class MathVector3
{

public:

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

private:

	float _x, _y, _z;

};

#endif