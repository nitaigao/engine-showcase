#ifndef __VECTOR3_H
#define __VECTOR3_H

class Vector3
{

public:

	Vector3( float x, float y, float z )
		: _x( x )
		, _y( y )
		, _z( z )
	{

	}

private:

	float _x, _y, _z;

};

#endif