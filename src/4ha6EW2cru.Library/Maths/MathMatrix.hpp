#ifndef __MATHMATRIX_H
#define __MATHMATRIX_H

#include <OgreMatrix3.h>

class MathVector3;

class MathMatrix : public Ogre::Matrix3
{

public:

	MathMatrix( float angle, const MathVector3& axis );

};

#endif