#ifndef __MATHQUATERNION_H
#define __MATHQUATERNION_H

#include <OgreMatrix3.h>
#include <OgreQuaternion.h>

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
		Ogre::Quaternion rotationAmount( Ogre::Degree( 90 ), Ogre::Vector3( 1.0f, 0.0f, 0.0f ) );
		Ogre::Quaternion rotationResult = this->AsOgreQuaternion( ) * rotationAmount;

		return hkQuaternion( rotationResult.x, rotationResult.y, rotationResult.z, rotationResult.w ); 
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



private:

	float _x;
	float _y;
	float _z;
	float _w;

};

#endif