#include "MathMatrix.hpp"

#include "MathVector3.hpp"

MathMatrix::MathMatrix( float angle, const MathVector3& axis )
{
	MathVector3 ax = const_cast< MathVector3& >( axis );
	this->FromAxisAngle( ax.AsOgreVector3( ), Ogre::Radian( angle ) );
}