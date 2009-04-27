#ifndef __MATHMATRIX_H
#define __MATHMATRIX_H

#include <OgreMatrix3.h>

namespace Maths
{
	class MathVector3;

	class MathMatrix : public Ogre::Matrix3
	{

	public:

		MathMatrix( const float& angle, const MathVector3& axis );

	};
};

#endif