#ifndef __IANIMATIONBLENDER_H
#define __IANIMATIONBLENDER_H

#include <string>

class IAnimationBlender
{

public:

	virtual void Update( float deltaMilliseconds ) = 0;
	virtual void Blend( const std::string animationName, float blendDuration ) = 0;

};


#endif